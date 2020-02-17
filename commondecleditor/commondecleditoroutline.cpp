#include "commondecleditoroutline.hpp"

#include "commondecleditor.hpp"
#include "commondecleditorwidget.hpp"
#include "commondecloutlinemodel.hpp"

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

using namespace commondecleditor;
using namespace commondecleditor::Internal;

CommonDeclEditorOutlineTreeView::CommonDeclEditorOutlineTreeView(QWidget *parent)
  : Utils::NavigationTreeView(parent)
{}

void CommonDeclEditorOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  if (!event)
    return;

  QMenu contextMenu;

  contextMenu.addAction(tr("Expand All"), this, &CommonDeclEditorOutlineTreeView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &CommonDeclEditorOutlineTreeView::collapseAll);

  contextMenu.exec(event->globalPos());

  event->accept();

  return;
}

void CommonDeclEditorOutlineTreeView::currentChanged(const QModelIndex& current,
                                                       const QModelIndex& previous)
{
  emit this->currentModelItemChanged(current, previous);
}

class CommonDeclEditorOutlineWidget::Private
{
  friend class CommonDeclEditorOutlineWidget;

  CommonDeclEditor*                 _commondeclEditor;
  CommonDeclEditorOutlineTreeView*  _treeView;
  bool                          _enableCursorSync;
  bool                          _blockCursorSync;

  Private(CommonDeclEditor* commondeclEditor)
    : _commondeclEditor(commondeclEditor),
      _enableCursorSync(true),
      _blockCursorSync(false)
  {}
};

CommonDeclEditorOutlineWidget::CommonDeclEditorOutlineWidget(CommonDeclEditor* commondeclEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(commondeclEditor))
{
  Q_ASSERT(this->_d->_commondeclEditor);

  this->_d->_treeView = new CommonDeclEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  CommonDeclOutlineModel const* commondeclOutlineModel = this->_d->_commondeclEditor->commondeclOutlineModel();
  Q_ASSERT(commondeclOutlineModel);
  this->_d->_treeView->setModel(const_cast<CommonDeclOutlineModel*>(commondeclOutlineModel));
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(commondeclOutlineModel->invisibleRootItem()->index());

  CommonDeclEditorWidget* commondeclEditorWidget = qobject_cast<CommonDeclEditorWidget*>(this->_d->_commondeclEditor->widget());
  Q_ASSERT(commondeclEditorWidget);

  //    connect(m_model, SIGNAL(modelReset()), this, SLOT(modelUpdated()));
  //    modelUpdated();

  connect(commondeclEditorWidget, &CommonDeclEditorWidget::outlineModelIndexChanged,
          this, &CommonDeclEditorOutlineWidget::updateSelectionInTree);
  connect(this->_d->_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &CommonDeclEditorOutlineWidget::updateSelectionInText);
  connect(this->_d->_treeView, &CommonDeclEditorOutlineTreeView::doubleClicked,
          this, &CommonDeclEditorOutlineWidget::updateTextCursor);
  connect(this->_d->_treeView, &CommonDeclEditorOutlineTreeView::currentModelItemChanged,
          this, &CommonDeclEditorOutlineWidget::currentModelItemChangedSlot);
}

QList<QAction*> CommonDeclEditorOutlineWidget::filterMenuActions() const
{
  return QList<QAction*>();
}

void CommonDeclEditorOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
  if(!this->_d->_commondeclEditor)
    return;

  this->_d->_enableCursorSync = syncWithCursor;

  if (this->_d->_enableCursorSync)
  {
    CommonDeclEditorWidget* commondeclEditorWidget = qobject_cast<CommonDeclEditorWidget*>(this->_d->_commondeclEditor->widget());
    Q_ASSERT(commondeclEditorWidget);
    this->updateSelectionInTree(commondeclEditorWidget->outlineModelIndex());
  }

  return;
}

void CommonDeclEditorOutlineWidget::modelUpdated()
{
  this->_d->_treeView->expandAll();
}

void CommonDeclEditorOutlineWidget::updateSelectionInTree(const QModelIndex &index)
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

void CommonDeclEditorOutlineWidget::updateSelectionInText(const QItemSelection &selection)
{
  if(!this->_d->_commondeclEditor)
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

void CommonDeclEditorOutlineWidget::updateTextCursor(const QModelIndex& modelIndex)
{
  if(!this->_d->_commondeclEditor)
    return;

  using namespace dslparser;

  CommonDeclOutlineModel const* commondeclOutlineModel = this->_d->_commondeclEditor->commondeclOutlineModel();
  Q_ASSERT(commondeclOutlineModel);

  TokenTextLocation const* tokTextLoc = commondeclOutlineModel->modelTextLocation(modelIndex);
  if (tokTextLoc)
  {
    this->_d->_blockCursorSync = true;

    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    CommonDeclEditorWidget* commondeclEditorWidget = qobject_cast<CommonDeclEditorWidget*>(this->_d->_commondeclEditor->widget());
    Q_ASSERT(commondeclEditorWidget);

    QTextCursor editCursor = commondeclEditorWidget->textCursor();
    editCursor.setPosition(tokTextLoc->start());
    commondeclEditorWidget->setTextCursor(editCursor);

    commondeclEditorWidget->setFocus();
    this->_d->_blockCursorSync = false;
  }

  return;
}

bool CommonDeclEditorOutlineWidget::syncCursor()
{
  return this->_d->_enableCursorSync && !this->_d->_blockCursorSync;
}

void CommonDeclEditorOutlineWidget::currentModelItemChangedSlot(const QModelIndex& current,
                                                                  const QModelIndex& previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
  //  if(!this->_d->_commondeclEditor)
  //    return;

  //  CommonDeclOutlineModel const* commondeclOutlineModel = this->_d->_commondeclEditor->commondeclOutlineModel();
  //  Q_ASSERT(commondeclOutlineModel);
  //  CommonDeclEditorWidget* commondeclEditorWidget = qobject_cast<CommonDeclEditorWidget*>(this->_d->_commondeclEditor->widget());
  //  Q_ASSERT(commondeclEditorWidget);

  //  //unhighlight previous
  //  if(model::irdl::PlaceItem const* selectedPlace = commondeclOutlineModel->placeFromIndex(previous))
  //    commondeclEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), false);
  //  else if(model::irdl::TransitionItem const* selectedTransition = commondeclOutlineModel->transitionFromIndex(previous))
  //    commondeclEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), false);

  //  //highlight current
  //  if(model::irdl::PlaceItem const* selectedPlace = commondeclOutlineModel->placeFromIndex(current))
  //    commondeclEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), true);
  //  else if(model::irdl::TransitionItem const* selectedTransition = commondeclOutlineModel->transitionFromIndex(current))
  //    commondeclEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), true);

  //  return;
}

bool CommonDeclEditorOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
  if(!editor)
    return false;

  if(!editor->document())
    return false;

  if(qobject_cast<CommonDeclEditor*>(editor))
    return true;

  return false;
}

TextEditor::IOutlineWidget*
CommonDeclEditorOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
  if(!editor)
    return 0;

  if(!editor->document())
    return 0;

  CommonDeclEditorOutlineWidget* outlineWidget=0;
  if(CommonDeclEditor* commondeclEditor=qobject_cast<CommonDeclEditor*>(editor))
  {
    if(commondeclEditor->commondeclOutlineModel())
      outlineWidget=new CommonDeclEditorOutlineWidget(commondeclEditor);
  }

  return outlineWidget;
}
