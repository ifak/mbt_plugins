/*
*  This file is part of statemachine Plugin for statemachine Creator.
*
*  Copyright (C) 2013 Jan Krause <jan.krause.no19@gmail.com>
*
*  This library is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as
*  published by the Free Software Foundation, either version 3 of the
*  License, or (at your option) any later version.
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library.
*  If not, see <http://www.gnu.org/licenses/>.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*/

#include "statemachineeditoroutline.hpp"

#include "statemachineeditor.hpp"
#include "statemachineeditorwidget.hpp"
#include "statemachineoutlinemodel.hpp"

#include <mobata/model/statemachine/statemachine.hpp>

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

using namespace statemachineeditor;
using namespace statemachineeditor::Internal;

StateMachineEditorOutlineTreeView::StateMachineEditorOutlineTreeView(QWidget *parent)
  : Utils::NavigationTreeView(parent)
{}

void StateMachineEditorOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  if (!event)
    return;

  QMenu contextMenu;

  contextMenu.addAction(tr("Expand All"), this, &StateMachineEditorOutlineTreeView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &StateMachineEditorOutlineTreeView::collapseAll);

  contextMenu.exec(event->globalPos());

  event->accept();

  return;
}
void StateMachineEditorOutlineTreeView::currentChanged(const QModelIndex& current,
                                                 const QModelIndex& previous)
{
  emit this->currentModelItemChanged(current, previous);
}

class StateMachineEditorOutlineWidget::Private
{
  friend class StateMachineEditorOutlineWidget;

  StateMachineEditor*                 _stateMachineEditor;
  StateMachineEditorOutlineTreeView*  _treeView;
  bool                          _enableCursorSync;
  bool                          _blockCursorSync;

  Private(StateMachineEditor* StateMachineEditor)
    : _stateMachineEditor(StateMachineEditor),
      _enableCursorSync(true),
      _blockCursorSync(false)
  {}
};

StateMachineEditorOutlineWidget::StateMachineEditorOutlineWidget(StateMachineEditor* StateMachineEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(StateMachineEditor))
{
  Q_ASSERT(this->_d->_stateMachineEditor);

  this->_d->_treeView = new StateMachineEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  StateMachineOutlineModelPtr statemachineOutlineModel = this->_d->_stateMachineEditor->statemachineOutlineModel();
  Q_ASSERT(statemachineOutlineModel);
  this->_d->_treeView->setModel(statemachineOutlineModel.data());
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(statemachineOutlineModel->invisibleRootItem()->index());

  StateMachineEditorWidget* statemachineEditorWidget = qobject_cast<StateMachineEditorWidget*>(this->_d->_stateMachineEditor->widget());
  Q_ASSERT(statemachineEditorWidget);

  connect(statemachineEditorWidget, &StateMachineEditorWidget::outlineModelIndexChanged,
          this, &StateMachineEditorOutlineWidget::updateSelectionInTree);
  connect(this->_d->_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &StateMachineEditorOutlineWidget::updateSelectionInText);
  connect(this->_d->_treeView, &StateMachineEditorOutlineTreeView::doubleClicked,
          this, &StateMachineEditorOutlineWidget::updateTextCursor);
  connect(this->_d->_treeView, &StateMachineEditorOutlineTreeView::currentModelItemChanged,
          this, &StateMachineEditorOutlineWidget::currentModelItemChangedSlot);
  connect(statemachineEditorWidget, &StateMachineEditorWidget::outlineModelIndexChanged,
          this, &StateMachineEditorOutlineWidget::currentModelItemChangedSlotC);
}

QList<QAction*> StateMachineEditorOutlineWidget::filterMenuActions() const
{
  return QList<QAction*>();
}

void StateMachineEditorOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
  this->_d->_enableCursorSync = syncWithCursor;
  if (this->_d->_enableCursorSync)
  {
    StateMachineEditorWidget* statemachineEditorWidget = qobject_cast<StateMachineEditorWidget*>(this->_d->_stateMachineEditor->widget());
    Q_ASSERT(statemachineEditorWidget);
    this->updateSelectionInTree(statemachineEditorWidget->outlineModelIndex());
  }

  return;
}

void StateMachineEditorOutlineWidget::modelUpdated()
{
  this->_d->_treeView->expandAll();
}

void StateMachineEditorOutlineWidget::updateSelectionInTree(const QModelIndex &index)
{
  if (!syncCursor())
    return;
  if(!index.isValid())
    return;

  //  QModelIndex proxyIndex = m_proxyModel->mapFromSource(index);

  this->_d->_blockCursorSync = true;

  this->_d->_treeView->selectionModel()->select(/*proxyIndex*/index,
                                                QItemSelectionModel::ClearAndSelect);
  this->_d->_treeView->scrollTo(/*proxyIndex*/index);
  this->_d->_blockCursorSync = false;

  return;
}

void StateMachineEditorOutlineWidget::updateSelectionInText(const QItemSelection &selection)
{
  if (!syncCursor())
    return;

  if (!selection.indexes().isEmpty())
  {
    QModelIndex proxyIndex = selection.indexes().first();
    this->updateTextCursor(proxyIndex);
  }

  return;
}

void StateMachineEditorOutlineWidget::updateTextCursor(const QModelIndex &modelIndex)
{
  if(!this->_d->_stateMachineEditor)
    return;

  using namespace dslparser;

  const StateMachineOutlineModelPtr stateMachineOutlineModel = this->_d->_stateMachineEditor->statemachineOutlineModel();
  Q_ASSERT(stateMachineOutlineModel);

  TokenTextLocation const* tokTextLoc = stateMachineOutlineModel->modelTextLocation(modelIndex);
  if (tokTextLoc)
  {
    this->_d->_blockCursorSync = true;

    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    StateMachineEditorWidget* stateMachineEditorWidget = qobject_cast<StateMachineEditorWidget*>(this->_d->_stateMachineEditor->widget());
    Q_ASSERT(stateMachineEditorWidget);

    QTextCursor editCursor = stateMachineEditorWidget->textCursor();
    editCursor.setPosition(tokTextLoc->start());
    stateMachineEditorWidget->setTextCursor(editCursor);

    stateMachineEditorWidget->setFocus();
    this->_d->_blockCursorSync = false;
  }

  return;
}

bool StateMachineEditorOutlineWidget::syncCursor()
{
  return this->_d->_enableCursorSync && !this->_d->_blockCursorSync;
}

void StateMachineEditorOutlineWidget::currentModelItemChangedSlot(const QModelIndex& current,
                                                            const QModelIndex& previous)
{
  Q_UNUSED(previous);

  if(!this->_d->_stateMachineEditor)
    return;

  const StateMachineOutlineModelPtr stateMachineOutlineModel = this->_d->_stateMachineEditor->statemachineOutlineModel();
  Q_ASSERT(stateMachineOutlineModel);
  StateMachineEditorWidget* stateMachineEditorWidget = qobject_cast<StateMachineEditorWidget*>(this->_d->_stateMachineEditor->widget());
  Q_ASSERT(stateMachineEditorWidget);

  //highlight current
  if(model::statemachine::AbstractStateItem const* selectedPlace = stateMachineOutlineModel->stateFromIndex(current))
    stateMachineEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), true);
  else if(model::statemachine::TransitionItem const* selectedTransition = stateMachineOutlineModel->transitionFromIndex(current))
    stateMachineEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), true);

  return;
}

void StateMachineEditorOutlineWidget::currentModelItemChangedSlotC(const QModelIndex& current)
{
  if(!this->_d->_stateMachineEditor)
    return;

  const StateMachineOutlineModelPtr stateMachineOutlineModel = this->_d->_stateMachineEditor->statemachineOutlineModel();
  Q_ASSERT(stateMachineOutlineModel);
  StateMachineEditorWidget* stateMachineEditorWidget = qobject_cast<StateMachineEditorWidget*>(this->_d->_stateMachineEditor->widget());
  Q_ASSERT(stateMachineEditorWidget);

  //highlight current
  if(model::statemachine::AbstractStateItem const* selectedPlace = stateMachineOutlineModel->stateFromIndex(current))
    stateMachineEditorWidget->highlightGraphicItemByUuid(selectedPlace->uuid(), true);
  else if(model::statemachine::TransitionItem const* selectedTransition = stateMachineOutlineModel->transitionFromIndex(current))
    stateMachineEditorWidget->highlightGraphicItemByUuid(selectedTransition->uuid(), true);

  return;
}

bool StateMachineEditorOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
  if(!editor)
    return false;

  if(!editor->document())
    return false;

  if(qobject_cast<StateMachineEditor*>(editor))
    return true;

  return false;
}

TextEditor::IOutlineWidget*
StateMachineEditorOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
  if(!editor)
    return 0;

  if(!editor->document())
    return 0;

  StateMachineEditorOutlineWidget* outlineWidget=0;
  if(StateMachineEditor* statemachineEditor=qobject_cast<StateMachineEditor*>(editor))
  {
    if(statemachineEditor->statemachineOutlineModel())
      outlineWidget=new StateMachineEditorOutlineWidget(statemachineEditor);
  }

  return outlineWidget;
}
