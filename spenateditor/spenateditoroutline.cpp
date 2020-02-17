#include "spenateditoroutline.hpp"

#include "spenateditor.hpp"
#include "spenateditorwidget.hpp"
#include "spenatgraphiceditor.hpp"
#include "spenatoutlinemodel.hpp"

#include <mobata/model/spenat/placeitem.hpp>
#include <mobata/model/spenat/transitionitem.hpp>

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

using namespace spenateditor;
using namespace spenateditor::Internal;

SpenatEditorOutlineTreeView::SpenatEditorOutlineTreeView(QWidget *parent)
  : Utils::NavigationTreeView(parent)
{}

void SpenatEditorOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  if (!event)
    return;

  QMenu contextMenu;

  contextMenu.addAction(tr("Expand All"), this, &SpenatEditorOutlineTreeView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &SpenatEditorOutlineTreeView::collapseAll);

  contextMenu.exec(event->globalPos());

  event->accept();

  return;
}

void SpenatEditorOutlineTreeView::currentChanged(const QModelIndex& current,
                                                 const QModelIndex& previous)
{
  emit this->currentModelItemChanged(current, previous);
}

class SpenatEditorOutlineWidget::Private
{
  friend class SpenatEditorOutlineWidget;

  SpenatEditor*                 _spenatEditor;
  SpenatEditorOutlineTreeView*  _treeView;
  bool                          _enableCursorSync;
  bool                          _blockCursorSync;

  Private(SpenatEditor* spenatEditor)
    : _spenatEditor(spenatEditor),
      _enableCursorSync(true),
      _blockCursorSync(false)
  {}
};

SpenatEditorOutlineWidget::SpenatEditorOutlineWidget(SpenatEditor* spenatEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(spenatEditor))
{
  Q_ASSERT(this->_d->_spenatEditor);

  this->_d->_treeView = new SpenatEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  const SpenatOutlineModelPtr spenatOutlineModel = this->_d->_spenatEditor->spenatOutlineModel();
  Q_ASSERT(spenatOutlineModel);
  this->_d->_treeView->setModel(const_cast<SpenatOutlineModel*>(spenatOutlineModel.data()));
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(spenatOutlineModel->invisibleRootItem()->index());

  SpenatEditorWidget* spenatEditorWidget = qobject_cast<SpenatEditorWidget*>(this->_d->_spenatEditor->widget());
  Q_ASSERT(spenatEditorWidget);

  connect(spenatEditorWidget, &SpenatEditorWidget::outlineModelIndexChanged,
          this, &SpenatEditorOutlineWidget::updateSelectionInTree);
  connect(this->_d->_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &SpenatEditorOutlineWidget::updateSelectionInText);
  connect(this->_d->_treeView, &SpenatEditorOutlineTreeView::doubleClicked,
          this, &SpenatEditorOutlineWidget::updateTextCursor);
  connect(this->_d->_treeView, &SpenatEditorOutlineTreeView::currentModelItemChanged,
          this, &SpenatEditorOutlineWidget::currentModelItemChangedSlot);
}

SpenatEditorOutlineWidget::SpenatEditorOutlineWidget(SpenatGraphicEditor* spenatGraphicEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(0))
{
  Q_ASSERT(spenatGraphicEditor);

  this->_d->_treeView = new SpenatEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  const SpenatDeclModelPtr spenatDeclModel=spenatGraphicEditor->spenatDeclModel();
  Q_ASSERT(spenatDeclModel);
  this->_d->_treeView->setModel((spenatDeclModel.data()));
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(spenatDeclModel->invisibleRootItem()->index());
}

QList<QAction*> SpenatEditorOutlineWidget::filterMenuActions() const
{
  return QList<QAction*>();
}

void SpenatEditorOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
  if(!this->_d->_spenatEditor)
    return;

  this->_d->_enableCursorSync = syncWithCursor;

  if (this->_d->_enableCursorSync)
  {
    SpenatEditorWidget* spenatEditorWidget = qobject_cast<SpenatEditorWidget*>(this->_d->_spenatEditor->widget());
    Q_ASSERT(spenatEditorWidget);
    this->updateSelectionInTree(spenatEditorWidget->outlineModelIndex());
  }

  return;
}

void SpenatEditorOutlineWidget::modelUpdated()
{
  this->_d->_treeView->expandAll();
}

void SpenatEditorOutlineWidget::updateSelectionInTree(const QModelIndex &index)
{
  if (!syncCursor())
    return;
  if(!index.isValid())
    return;

  this->_d->_blockCursorSync = true;

  this->_d->_treeView->selectionModel()->select(/*proxyIndex*/index,
                                                QItemSelectionModel::ClearAndSelect);
  this->_d->_treeView->scrollTo(index);
  this->_d->_blockCursorSync = false;

  return;
}

void SpenatEditorOutlineWidget::updateSelectionInText(const QItemSelection &selection)
{
  if(!this->_d->_spenatEditor)
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

void SpenatEditorOutlineWidget::updateTextCursor(const QModelIndex& modelIndex)
{
  if(!this->_d->_spenatEditor)
    return;

  using namespace dslparser;

  const SpenatOutlineModelPtr spenatOutlineModel = this->_d->_spenatEditor->spenatOutlineModel();
  Q_ASSERT(spenatOutlineModel);

  TokenTextLocation const* tokTextLoc = spenatOutlineModel->modelTextLocation(modelIndex);
  if (tokTextLoc)
  {
    this->_d->_blockCursorSync = true;

    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    SpenatEditorWidget* spenatEditorWidget = qobject_cast<SpenatEditorWidget*>(this->_d->_spenatEditor->widget());
    Q_ASSERT(spenatEditorWidget);

    QTextCursor editCursor = spenatEditorWidget->textCursor();
    editCursor.setPosition(tokTextLoc->start());
    spenatEditorWidget->setTextCursor(editCursor);

    spenatEditorWidget->setFocus();
    this->_d->_blockCursorSync = false;
  }

  return;
}

bool SpenatEditorOutlineWidget::syncCursor()
{
  return this->_d->_enableCursorSync && !this->_d->_blockCursorSync;
}

void SpenatEditorOutlineWidget::currentModelItemChangedSlot(const QModelIndex& current,
                                                            const QModelIndex& previous)
{
  if(!this->_d->_spenatEditor)
    return;

  const SpenatOutlineModelPtr spenatOutlineModel = this->_d->_spenatEditor->spenatOutlineModel();
  Q_ASSERT(spenatOutlineModel);
  SpenatEditorWidget* spenatEditorWidget = qobject_cast<SpenatEditorWidget*>(this->_d->_spenatEditor->widget());
  Q_ASSERT(spenatEditorWidget);

  //unhighlight previous
  if(model::spenat::PlaceItem const* selectedPlace = spenatOutlineModel->placeFromIndex(previous))
    spenatEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), false);
  else if(model::spenat::TransitionItem const* selectedTransition = spenatOutlineModel->transitionFromIndex(previous))
    spenatEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), false);

  //highlight current
  if(model::spenat::PlaceItem const* selectedPlace = spenatOutlineModel->placeFromIndex(current))
    spenatEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), true);
  else if(model::spenat::TransitionItem const* selectedTransition = spenatOutlineModel->transitionFromIndex(current))
    spenatEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), true);

  return;
}

bool SpenatEditorOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
  if(!editor)
    return false;

  if(!editor->document())
    return false;

  if(qobject_cast<SpenatEditor*>(editor))
    return true;
  else if(qobject_cast<SpenatGraphicEditor*>(editor))
    return true;

  return false;
}

TextEditor::IOutlineWidget*
SpenatEditorOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
  if(!editor)
    return 0;

  if(!editor->document())
    return 0;

  SpenatEditorOutlineWidget* outlineWidget=0;
  if(SpenatEditor* spenatEditor=qobject_cast<SpenatEditor*>(editor))
  {
    if(spenatEditor->spenatOutlineModel())
      outlineWidget=new SpenatEditorOutlineWidget(spenatEditor);
  }
  else if(SpenatGraphicEditor* spenatGraphicEditor=qobject_cast<SpenatGraphicEditor*>(editor))
  {
    if(spenatGraphicEditor->spenatDeclModel())
      outlineWidget=new SpenatEditorOutlineWidget(spenatGraphicEditor);
  }

  return outlineWidget;
}
