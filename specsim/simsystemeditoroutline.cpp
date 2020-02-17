/*
*  This file is part of simsystem Plugin for simsystem Creator.
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

#include "simsystemeditoroutline.hpp"
#include "simsystemeditorwidget.hpp"

#include "simsystemeditor.hpp"
#include "simsystemoutlinemodel.hpp"

#include <libs/modsim/modsim/simsystemmodel.hpp>

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

using namespace specsim;
using namespace specsim::Internal;

SimSystemEditorOutlineTreeView::SimSystemEditorOutlineTreeView(QWidget *parent)
  : Utils::NavigationTreeView(parent)
{}

void SimSystemEditorOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  if (!event)
    return;

  QMenu contextMenu;

  contextMenu.addAction(tr("Expand All"), this, &SimSystemEditorOutlineTreeView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &SimSystemEditorOutlineTreeView::collapseAll);

  contextMenu.exec(event->globalPos());

  event->accept();

  return;
}
void SimSystemEditorOutlineTreeView::currentChanged(const QModelIndex& current,
                                                 const QModelIndex& previous)
{
  emit this->currentModelItemChanged(current, previous);
}

class SimSystemEditorOutlineWidget::Private
{
  friend class SimSystemEditorOutlineWidget;

  SimSystemEditor*                 _simSystemEditor;
  SimSystemEditorOutlineTreeView*  _treeView;
  bool                          _enableCursorSync;
  bool                          _blockCursorSync;

  Private(SimSystemEditor* SimSystemEditor)
    : _simSystemEditor(SimSystemEditor),
      _enableCursorSync(true),
      _blockCursorSync(false)
  {}
};

SimSystemEditorOutlineWidget::SimSystemEditorOutlineWidget(SimSystemEditor* SimSystemEditor)
  : TextEditor::IOutlineWidget(), _d(new Private(SimSystemEditor))
{
  Q_ASSERT(this->_d->_simSystemEditor);

  this->_d->_treeView = new SimSystemEditorOutlineTreeView(this);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(this->_d->_treeView);
  this->setLayout(layout);

  SimSystemOutlineModelPtr simsystemOutlineModel = this->_d->_simSystemEditor->simsystemOutlineModel();
  Q_ASSERT(simsystemOutlineModel);
  this->_d->_treeView->setModel(simsystemOutlineModel.data());
  this->_d->_treeView->expandAll();

  this->updateSelectionInTree(simsystemOutlineModel->invisibleRootItem()->index());

  SimSystemEditorWidget* simsystemEditorWidget = qobject_cast<SimSystemEditorWidget*>(this->_d->_simSystemEditor->widget());
  Q_ASSERT(simsystemEditorWidget);

  connect(simsystemEditorWidget, &SimSystemEditorWidget::outlineModelIndexChanged,
          this, &SimSystemEditorOutlineWidget::updateSelectionInTree);
  connect(this->_d->_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &SimSystemEditorOutlineWidget::updateSelectionInText);
  connect(this->_d->_treeView, &SimSystemEditorOutlineTreeView::doubleClicked,
          this, &SimSystemEditorOutlineWidget::updateTextCursor);
  connect(this->_d->_treeView, &SimSystemEditorOutlineTreeView::currentModelItemChanged,
          this, &SimSystemEditorOutlineWidget::currentModelItemChangedSlot);
  connect(simsystemEditorWidget, &SimSystemEditorWidget::outlineModelIndexChanged,
          this, &SimSystemEditorOutlineWidget::currentModelItemChangedSlotC);
}

QList<QAction*> SimSystemEditorOutlineWidget::filterMenuActions() const
{
  return QList<QAction*>();
}

void SimSystemEditorOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
  this->_d->_enableCursorSync = syncWithCursor;
  if (this->_d->_enableCursorSync)
  {
    SimSystemEditorWidget* simsystemEditorWidget = qobject_cast<SimSystemEditorWidget*>(this->_d->_simSystemEditor->widget());
    Q_ASSERT(simsystemEditorWidget);
    this->updateSelectionInTree(simsystemEditorWidget->outlineModelIndex());
  }

  return;
}

void SimSystemEditorOutlineWidget::modelUpdated()
{
  this->_d->_treeView->expandAll();
}

void SimSystemEditorOutlineWidget::updateSelectionInTree(const QModelIndex &index)
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

void SimSystemEditorOutlineWidget::updateSelectionInText(const QItemSelection &selection)
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

void SimSystemEditorOutlineWidget::updateTextCursor(const QModelIndex &modelIndex)
{
  if(!this->_d->_simSystemEditor)
    return;

  using namespace dslparser;

  const SimSystemOutlineModelPtr simSystemOutlineModel = this->_d->_simSystemEditor->simsystemOutlineModel();
  Q_ASSERT(simSystemOutlineModel);

  TokenTextLocation const* tokTextLoc = simSystemOutlineModel->modelTextLocation(modelIndex);
  if (tokTextLoc)
  {
    this->_d->_blockCursorSync = true;

    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    SimSystemEditorWidget* simSystemEditorWidget = qobject_cast<SimSystemEditorWidget*>(this->_d->_simSystemEditor->widget());
    Q_ASSERT(simSystemEditorWidget);

    QTextCursor editCursor = simSystemEditorWidget->textCursor();
    editCursor.setPosition(tokTextLoc->start());
    simSystemEditorWidget->setTextCursor(editCursor);

    simSystemEditorWidget->setFocus();
    this->_d->_blockCursorSync = false;
  }

  return;
}

bool SimSystemEditorOutlineWidget::syncCursor()
{
  return this->_d->_enableCursorSync && !this->_d->_blockCursorSync;
}

void SimSystemEditorOutlineWidget::currentModelItemChangedSlot(const QModelIndex& current,
                                                            const QModelIndex& previous)
{
  Q_UNUSED(current);
  Q_UNUSED(previous);
  if(!this->_d->_simSystemEditor)
    return;

  const SimSystemOutlineModelPtr simSystemOutlineModel = this->_d->_simSystemEditor->simsystemOutlineModel();
  Q_ASSERT(simSystemOutlineModel);
  SimSystemEditorWidget* simSystemEditorWidget = qobject_cast<SimSystemEditorWidget*>(this->_d->_simSystemEditor->widget());
  Q_ASSERT(simSystemEditorWidget);

  //highlight current

  return;
}

void SimSystemEditorOutlineWidget::currentModelItemChangedSlotC(const QModelIndex& current)
{
  Q_UNUSED(current);
  if(!this->_d->_simSystemEditor)
    return;

  const SimSystemOutlineModelPtr simSystemOutlineModel = this->_d->_simSystemEditor->simsystemOutlineModel();
  Q_ASSERT(simSystemOutlineModel);
  SimSystemEditorWidget* simSystemEditorWidget = qobject_cast<SimSystemEditorWidget*>(this->_d->_simSystemEditor->widget());
  Q_ASSERT(simSystemEditorWidget);

  //highlight current

  return;
}

bool SimSystemEditorOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
  if(!editor)
    return false;

  if(!editor->document())
    return false;

  if(qobject_cast<SimSystemEditor*>(editor))
    return true;

  return false;
}

TextEditor::IOutlineWidget*
SimSystemEditorOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
  if(!editor)
    return 0;

  if(!editor->document())
    return 0;

  SimSystemEditorOutlineWidget* outlineWidget=0;
  if(SimSystemEditor* simsystemEditor=qobject_cast<SimSystemEditor*>(editor))
  {
    if(simsystemEditor->simsystemOutlineModel())
      outlineWidget=new SimSystemEditorOutlineWidget(simsystemEditor);
  }

  return outlineWidget;
}
