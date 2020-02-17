#include "isrdleditoroutline.hpp"

#include "isrdleditor.hpp"
#include "isrdleditorwidget.hpp"
#include "isrdloutlinemodel.hpp"

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

using namespace isrdleditor;
using namespace isrdleditor::Internal;

IsrdlEditorOutlineTreeView::IsrdlEditorOutlineTreeView(QWidget *parent)
  : Utils::NavigationTreeView(parent)
{}

void IsrdlEditorOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  if (!event)
    return;

  QMenu contextMenu;

  contextMenu.addAction(tr("Expand All"), this, &IsrdlEditorOutlineTreeView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &IsrdlEditorOutlineTreeView::collapseAll);

  contextMenu.exec(event->globalPos());

  event->accept();

  return;
}

void IsrdlEditorOutlineTreeView::currentChanged(const QModelIndex& current,
                                                       const QModelIndex& previous)
{
  emit this->currentModelItemChanged(current, previous);
}

class IsrdlEditorOutlineWidget::Private
{
  friend class IsrdlEditorOutlineWidget;

  IsrdlEditor*                 _isrdlEditor;
  IsrdlEditorOutlineTreeView*  _treeView;
  bool                          _enableCursorSync;
  bool                          _blockCursorSync;

  Private(IsrdlEditor* isrdlEditor)
    : _isrdlEditor(isrdlEditor),
      _enableCursorSync(true),
      _blockCursorSync(false)
  {}
};

IsrdlEditorOutlineWidget::IsrdlEditorOutlineWidget(IsrdlEditor* isrdlEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(isrdlEditor))
{
  Q_ASSERT(this->_d->_isrdlEditor);

  this->_d->_treeView = new IsrdlEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  IsrdlOutlineModel const* isrdlOutlineModel = this->_d->_isrdlEditor->isrdlOutlineModel();
  Q_ASSERT(isrdlOutlineModel);
  this->_d->_treeView->setModel(const_cast<IsrdlOutlineModel*>(isrdlOutlineModel));
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(isrdlOutlineModel->invisibleRootItem()->index());

  IsrdlEditorWidget* isrdlEditorWidget = qobject_cast<IsrdlEditorWidget*>(this->_d->_isrdlEditor->widget());
  Q_ASSERT(isrdlEditorWidget);

  //    connect(m_model, SIGNAL(modelReset()), this, SLOT(modelUpdated()));
  //    modelUpdated();

  connect(isrdlEditorWidget, &IsrdlEditorWidget::outlineModelIndexChanged,
          this, &IsrdlEditorOutlineWidget::updateSelectionInTree);
  connect(this->_d->_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &IsrdlEditorOutlineWidget::updateSelectionInText);
  connect(this->_d->_treeView, &IsrdlEditorOutlineTreeView::doubleClicked,
          this, &IsrdlEditorOutlineWidget::updateTextCursor);
  connect(this->_d->_treeView, &IsrdlEditorOutlineTreeView::currentModelItemChanged,
          this, &IsrdlEditorOutlineWidget::currentModelItemChangedSlot);
}

IsrdlEditorOutlineWidget::IsrdlEditorOutlineWidget(IsrdlGraphicEditor* isrdlGraphicEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(0))
{
  Q_ASSERT(isrdlGraphicEditor);

  this->_d->_treeView = new IsrdlEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

//  IsrdlOutlineModel const* isrdlOutlineModel = isrdlGraphicEditor->isrdlOutlineModel();
//  Q_ASSERT(isrdlOutlineModel);
//  this->_d->_treeView->setModel(const_cast<IsrdlOutlineModel*>(isrdlOutlineModel));
//  this->_d->_treeView->expandAll();

//  this->updateSelectionInTree(isrdlOutlineModel->invisibleRootItem()->index());
}

QList<QAction*> IsrdlEditorOutlineWidget::filterMenuActions() const
{
  return QList<QAction*>();
}

void IsrdlEditorOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
  if(!this->_d->_isrdlEditor)
    return;

  this->_d->_enableCursorSync = syncWithCursor;

  if (this->_d->_enableCursorSync)
  {
    IsrdlEditorWidget* isrdlEditorWidget = qobject_cast<IsrdlEditorWidget*>(this->_d->_isrdlEditor->widget());
    Q_ASSERT(isrdlEditorWidget);
    this->updateSelectionInTree(isrdlEditorWidget->outlineModelIndex());
  }

  return;
}

void IsrdlEditorOutlineWidget::modelUpdated()
{
  this->_d->_treeView->expandAll();
}

void IsrdlEditorOutlineWidget::updateSelectionInTree(const QModelIndex &index)
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

void IsrdlEditorOutlineWidget::updateSelectionInText(const QItemSelection &selection)
{
  if(!this->_d->_isrdlEditor)
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

void IsrdlEditorOutlineWidget::updateTextCursor(const QModelIndex& modelIndex)
{
  if(!this->_d->_isrdlEditor)
    return;

  using namespace dslparser;

  IsrdlOutlineModel const* isrdlOutlineModel = this->_d->_isrdlEditor->isrdlOutlineModel();
  Q_ASSERT(isrdlOutlineModel);

  TokenTextLocation const* tokTextLoc = isrdlOutlineModel->modelTextLocation(modelIndex);
  if (tokTextLoc)
  {
    this->_d->_blockCursorSync = true;

    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    IsrdlEditorWidget* isrdlEditorWidget = qobject_cast<IsrdlEditorWidget*>(this->_d->_isrdlEditor->widget());
    Q_ASSERT(isrdlEditorWidget);

    QTextCursor editCursor = isrdlEditorWidget->textCursor();
    editCursor.setPosition(tokTextLoc->start());
    isrdlEditorWidget->setTextCursor(editCursor);

    isrdlEditorWidget->setFocus();
    this->_d->_blockCursorSync = false;
  }

  return;
}

bool IsrdlEditorOutlineWidget::syncCursor()
{
  return this->_d->_enableCursorSync && !this->_d->_blockCursorSync;
}

void IsrdlEditorOutlineWidget::currentModelItemChangedSlot(const QModelIndex& current,
                                                                  const QModelIndex& previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
  //  if(!this->_d->_isrdlEditor)
  //    return;

  //  IsrdlOutlineModel const* isrdlOutlineModel = this->_d->_isrdlEditor->isrdlOutlineModel();
  //  Q_ASSERT(isrdlOutlineModel);
  //  IsrdlEditorWidget* isrdlEditorWidget = qobject_cast<IsrdlEditorWidget*>(this->_d->_isrdlEditor->widget());
  //  Q_ASSERT(isrdlEditorWidget);

  //  //unhighlight previous
  //  if(model::isrdl::PlaceItem const* selectedPlace = isrdlOutlineModel->placeFromIndex(previous))
  //    isrdlEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), false);
  //  else if(model::isrdl::TransitionItem const* selectedTransition = isrdlOutlineModel->transitionFromIndex(previous))
  //    isrdlEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), false);

  //  //highlight current
  //  if(model::isrdl::PlaceItem const* selectedPlace = isrdlOutlineModel->placeFromIndex(current))
  //    isrdlEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), true);
  //  else if(model::isrdl::TransitionItem const* selectedTransition = isrdlOutlineModel->transitionFromIndex(current))
  //    isrdlEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), true);

  //  return;
}

bool IsrdlEditorOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
  if(!editor)
    return false;

  if(!editor->document())
    return false;

  if(qobject_cast<IsrdlEditor*>(editor))
    return true;
//  else if(qobject_cast<IsrdlGraphicEditor*>(editor))
//    return true;

  return false;
}

TextEditor::IOutlineWidget*
IsrdlEditorOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
  if(!editor)
    return 0;

  if(!editor->document())
    return 0;

  IsrdlEditorOutlineWidget* outlineWidget=0;
  if(IsrdlEditor* isrdlEditor=qobject_cast<IsrdlEditor*>(editor))
  {
    if(isrdlEditor->isrdlOutlineModel())
      outlineWidget=new IsrdlEditorOutlineWidget(isrdlEditor);
  }
//  else if(IsrdlGraphicEditor* isrdlGraphicEditor=qobject_cast<IsrdlGraphicEditor*>(editor))
//  {
//    if(isrdlGraphicEditor->isrdlOutlineModel())
//      outlineWidget=new IsrdlEditorOutlineWidget(isrdlGraphicEditor);
//  }

  return outlineWidget;
}
