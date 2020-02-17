/*
 *  This file is part of statemachine Editor Plugin for AppCreator based application.
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

#ifndef STATEMACHINEEDITOR_STATEMACHINEEDITOROUTLINE_HPP
#define STATEMACHINEEDITOR_STATEMACHINEEDITOROUTLINE_HPP

#include <utils/navigationtreeview.h>
#include <texteditor/ioutlinewidget.h>

#include <QSortFilterProxyModel>
#include <QTreeView>

namespace statemachineeditor{

class StateMachineEditor;

namespace Internal{

class StateMachineEditorOutlineTreeView
    : public Utils::NavigationTreeView
{
  Q_OBJECT

public:
  StateMachineEditorOutlineTreeView(QWidget *parent);

  void contextMenuEvent(QContextMenuEvent *event);

protected:
  virtual void currentChanged(const QModelIndex& current,
                              const QModelIndex& previous);

signals:
  void currentModelItemChanged(const QModelIndex& current,
                               const QModelIndex& previous);
};

class StateMachineEditorOutlineWidget
    : public TextEditor::IOutlineWidget
{
  Q_OBJECT
public:
  StateMachineEditorOutlineWidget(StateMachineEditor* StateMachineEditor);

  // IOutlineWidget
  virtual QList<QAction*> filterMenuActions() const;
  virtual void setCursorSynchronization(bool syncWithCursor);


public slots:
  void modelUpdated();
  void updateSelectionInTree(const QModelIndex &index);
  void updateSelectionInText(const QItemSelection &selection);
  void updateTextCursor(const QModelIndex &modelIndex);

private:
  bool syncCursor();

private slots:
  void currentModelItemChangedSlot(const QModelIndex& current,
                                   const QModelIndex& previous);
  void currentModelItemChangedSlotC(const QModelIndex& current);

private:
  Q_DISABLE_COPY(StateMachineEditorOutlineWidget)
  class Private;
  Private* _d;
};

class StateMachineEditorOutlineWidgetFactory
    : public TextEditor::IOutlineWidgetFactory
{
  Q_OBJECT
public:
  bool supportsEditor(Core::IEditor *editor) const;
  TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor);
};

} // namespace Internal
} // namespace statemachineeditor

#endif // STATEMACHINEEDITOR_STATEMACHINEEDITOROUTLINE_HPP
