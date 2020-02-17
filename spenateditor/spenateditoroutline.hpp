/*
 *  This file is part of Spenat Editor Plugin for AppCreator based application.
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

#ifndef SPENATEDITOR_SPENATEDITOROUTLINE_HPP
#define SPENATEDITOR_SPENATEDITOROUTLINE_HPP

#include <utils/navigationtreeview.h>
#include <texteditor/ioutlinewidget.h>

#include <QSortFilterProxyModel>
#include <QTreeView>

namespace spenateditor{

class SpenatEditor;
class SpenatGraphicEditor;

namespace Internal{

class SpenatEditorOutlineTreeView
    : public Utils::NavigationTreeView
{
  Q_OBJECT

public:
  SpenatEditorOutlineTreeView(QWidget *parent);

  void contextMenuEvent(QContextMenuEvent *event);

protected:
  virtual void currentChanged(const QModelIndex& current,
                              const QModelIndex& previous);

signals:
  void currentModelItemChanged(const QModelIndex& current,
                               const QModelIndex& previous);
};

class SpenatEditorOutlineWidget
    : public TextEditor::IOutlineWidget
{
  Q_OBJECT
public:
  SpenatEditorOutlineWidget(SpenatEditor* spenatEditor);
  SpenatEditorOutlineWidget(SpenatGraphicEditor* spenatGraphicEditor);

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
  Q_DISABLE_COPY(SpenatEditorOutlineWidget)
  class Private;
  Private* _d;
};

class SpenatEditorOutlineWidgetFactory
    : public TextEditor::IOutlineWidgetFactory
{
  Q_OBJECT
public:
  bool supportsEditor(Core::IEditor *editor) const;
  TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor);
};

} // namespace Internal
} // namespace spenateditor

#endif // SPENATEDITOR_SPENATEDITOROUTLINE_HPP
