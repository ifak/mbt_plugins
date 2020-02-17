#include "irdleditoroutline.hpp"

#include "irdleditor.hpp"
#include "irdleditorwidget.hpp"
#include "irdlgraphiceditor.hpp"
#include "irdloutlinemodel.hpp"

//#include <mobata/model/requirement/placeitem.hpp>
//#include <mobata/model/requirement/transitionitem.hpp>

#include <projectexplorer/projectexplorer.h>

#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/editormanager.h>
#include <utils/qtcassert.h>

#include <QDebug>
#include <QTimer>
#include <QVBoxLayout>
#include <QMenu>
#include <QContextMenuEvent>

#include <mobata/memory_leak_start.hpp>

using namespace irdleditor;
using namespace irdleditor::Internal;

IrdlEditorOutlineTreeView::IrdlEditorOutlineTreeView(QWidget *parent)
  : Utils::NavigationTreeView(parent)
{}

void IrdlEditorOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  if (!event)
    return;

  QMenu contextMenu;

  contextMenu.addAction(tr("Expand All"), this, &IrdlEditorOutlineTreeView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &IrdlEditorOutlineTreeView::collapseAll);

  contextMenu.exec(event->globalPos());

  event->accept();

  return;
}

void IrdlEditorOutlineTreeView::currentChanged(const QModelIndex& current,
                                                       const QModelIndex& previous)
{
  emit this->currentModelItemChanged(current, previous);
}

class IrdlEditorOutlineWidget::Private
{
  friend class IrdlEditorOutlineWidget;

  IrdlEditor*                 _irdlEditor;
  IrdlEditorOutlineTreeView*  _treeView;
  bool                          _enableCursorSync;
  bool                          _blockCursorSync;

  Private(IrdlEditor* irdlEditor)
    : _irdlEditor(irdlEditor),
      _enableCursorSync(true),
      _blockCursorSync(false)
  {}
};

IrdlEditorOutlineWidget::IrdlEditorOutlineWidget(IrdlEditor* irdlEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(irdlEditor))
{
  Q_ASSERT(this->_d->_irdlEditor);

  this->_d->_treeView = new IrdlEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  IrdlOutlineModel const* irdlOutlineModel = this->_d->_irdlEditor->irdlOutlineModel();
  Q_ASSERT(irdlOutlineModel);
  this->_d->_treeView->setModel(const_cast<IrdlOutlineModel*>(irdlOutlineModel));
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(irdlOutlineModel->invisibleRootItem()->index());

  IrdlEditorWidget* irdlEditorWidget = qobject_cast<IrdlEditorWidget*>(this->_d->_irdlEditor->widget());
  Q_ASSERT(irdlEditorWidget);

  //    connect(m_model, SIGNAL(modelReset()), this, SLOT(modelUpdated()));
  //    modelUpdated();

  connect(irdlEditorWidget, &IrdlEditorWidget::outlineModelIndexChanged,
          this, &IrdlEditorOutlineWidget::updateSelectionInTree);
  connect(this->_d->_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &IrdlEditorOutlineWidget::updateSelectionInText);
  connect(this->_d->_treeView, &IrdlEditorOutlineTreeView::doubleClicked,
          this, &IrdlEditorOutlineWidget::updateTextCursor);
  connect(this->_d->_treeView, &IrdlEditorOutlineTreeView::currentModelItemChanged,
          this, &IrdlEditorOutlineWidget::currentModelItemChangedSlot);
}

IrdlEditorOutlineWidget::IrdlEditorOutlineWidget(IrdlGraphicEditor* irdlGraphicEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(0))
{
  Q_ASSERT(irdlGraphicEditor);

  this->_d->_treeView = new IrdlEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  IrdlOutlineModel const* irdlOutlineModel = irdlGraphicEditor->irdlOutlineModel();
  Q_ASSERT(irdlOutlineModel);
  this->_d->_treeView->setModel(const_cast<IrdlOutlineModel*>(irdlOutlineModel));
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(irdlOutlineModel->invisibleRootItem()->index());
}

QList<QAction*> IrdlEditorOutlineWidget::filterMenuActions() const
{
  return QList<QAction*>();
}

void IrdlEditorOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
  if(!this->_d->_irdlEditor)
    return;

  this->_d->_enableCursorSync = syncWithCursor;

  if (this->_d->_enableCursorSync)
  {
    IrdlEditorWidget* irdlEditorWidget = qobject_cast<IrdlEditorWidget*>(this->_d->_irdlEditor->widget());
    Q_ASSERT(irdlEditorWidget);
    this->updateSelectionInTree(irdlEditorWidget->outlineModelIndex());
  }

  return;
}

void IrdlEditorOutlineWidget::modelUpdated()
{
  this->_d->_treeView->expandAll();
}

void IrdlEditorOutlineWidget::updateSelectionInTree(const QModelIndex &index)
{
  if (!syncCursor())
    return;
  if(!index.isValid())
    return;

  this->_d->_blockCursorSync = true;

  this->_d->_treeView->setCurrentIndex(index);
  this->_d->_treeView->scrollTo(index);
  this->_d->_blockCursorSync = false;

  return;
}

void IrdlEditorOutlineWidget::updateSelectionInText(const QItemSelection &selection)
{
  if(!this->_d->_irdlEditor)
    return;
  if (!syncCursor())
    return;

  if (!selection.indexes().isEmpty())
  {
    QModelIndex proxyIndex = selection.indexes().first();
    this->updateTextCursor(proxyIndex);
  }

  return;
}

void IrdlEditorOutlineWidget::updateTextCursor(const QModelIndex& modelIndex)
{
  if(!this->_d->_irdlEditor)
    return;

  using namespace dslparser;

  IrdlOutlineModel const* irdlOutlineModel = this->_d->_irdlEditor->irdlOutlineModel();
  Q_ASSERT(irdlOutlineModel);

  TokenTextLocation const* tokTextLoc = irdlOutlineModel->modelTextLocation(modelIndex);
  if (tokTextLoc)
  {
    this->_d->_blockCursorSync = true;

    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    IrdlEditorWidget* irdlEditorWidget = qobject_cast<IrdlEditorWidget*>(this->_d->_irdlEditor->widget());
    Q_ASSERT(irdlEditorWidget);

    QTextCursor editCursor = irdlEditorWidget->textCursor();
    editCursor.setPosition(tokTextLoc->start());
    irdlEditorWidget->setTextCursor(editCursor);

    irdlEditorWidget->setFocus();
    this->_d->_blockCursorSync = false;
  }

  return;
}

bool IrdlEditorOutlineWidget::syncCursor()
{
  return this->_d->_enableCursorSync && !this->_d->_blockCursorSync;
}

void IrdlEditorOutlineWidget::currentModelItemChangedSlot(const QModelIndex& current,
                                                                  const QModelIndex& previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
  //  if(!this->_d->_irdlEditor)
  //    return;

  //  IrdlOutlineModel const* irdlOutlineModel = this->_d->_irdlEditor->irdlOutlineModel();
  //  Q_ASSERT(irdlOutlineModel);
  //  IrdlEditorWidget* irdlEditorWidget = qobject_cast<IrdlEditorWidget*>(this->_d->_irdlEditor->widget());
  //  Q_ASSERT(irdlEditorWidget);

  //  //unhighlight previous
  //  if(model::irdl::PlaceItem const* selectedPlace = irdlOutlineModel->placeFromIndex(previous))
  //    irdlEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), false);
  //  else if(model::irdl::TransitionItem const* selectedTransition = irdlOutlineModel->transitionFromIndex(previous))
  //    irdlEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), false);

  //  //highlight current
  //  if(model::irdl::PlaceItem const* selectedPlace = irdlOutlineModel->placeFromIndex(current))
  //    irdlEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), true);
  //  else if(model::irdl::TransitionItem const* selectedTransition = irdlOutlineModel->transitionFromIndex(current))
  //    irdlEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), true);

  //  return;
}

bool IrdlEditorOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
  if(!editor)
    return false;

  if(!editor->document())
    return false;

  if(qobject_cast<IrdlEditor*>(editor))
    return true;
  else if(qobject_cast<IrdlGraphicEditor*>(editor))
    return true;

  return false;
}

TextEditor::IOutlineWidget*
IrdlEditorOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
  if(!editor)
    return 0;

  if(!editor->document())
    return 0;

  IrdlEditorOutlineWidget* outlineWidget=0;
  if(IrdlEditor* irdlEditor=qobject_cast<IrdlEditor*>(editor))
  {
    if(irdlEditor->irdlOutlineModel())
      outlineWidget=new IrdlEditorOutlineWidget(irdlEditor);
  }
  else if(IrdlGraphicEditor* irdlGraphicEditor=qobject_cast<IrdlGraphicEditor*>(editor))
  {
    if(irdlGraphicEditor->irdlOutlineModel())
      outlineWidget=new IrdlEditorOutlineWidget(irdlGraphicEditor);
  }

  return outlineWidget;
}
