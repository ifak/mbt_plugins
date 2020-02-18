/*
 * This file is part of mbt_plugins.
 *
 * mbt_plugins is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mbt_plugins is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mbt_plugins.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <utils/navigationtreeview.h>
#include <texteditor/ioutlinewidget.h>

#include <QSortFilterProxyModel>
#include <QTreeView>

namespace commondecleditor{

class CommonDeclEditor;

namespace Internal{

class CommonDeclEditorOutlineTreeView
    : public Utils::NavigationTreeView
{
  Q_OBJECT

public:
  CommonDeclEditorOutlineTreeView(QWidget *parent);

  void contextMenuEvent(QContextMenuEvent *event);

protected:
  virtual void currentChanged(const QModelIndex& current,
                              const QModelIndex& previous);

signals:
  void currentModelItemChanged(const QModelIndex& current,
                               const QModelIndex& previous);
};

class CommonDeclEditorOutlineWidget
    : public TextEditor::IOutlineWidget
{
  Q_OBJECT
public:
  CommonDeclEditorOutlineWidget(CommonDeclEditor* commondeclEditor);

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

private:
  Q_DISABLE_COPY(CommonDeclEditorOutlineWidget)
  class Private;
  Private* _d;
};

class CommonDeclEditorOutlineWidgetFactory
    : public TextEditor::IOutlineWidgetFactory
{
  Q_OBJECT
public:
  bool supportsEditor(Core::IEditor *editor) const;
  TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor);
};

} // namespace Internal
} // namespace commondecleditor
