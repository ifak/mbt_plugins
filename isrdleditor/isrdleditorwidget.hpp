/*
 * This file is part of mobata.
 *
 * mobata is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mobata is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mobata.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "isrdleditor_global.hpp"

#include <dsleditor/dsleditorwidget.hpp>
#include <coreplugin/icore.h>

#include "isrdleditor.hpp"

#include <QUuid>

namespace isrdleditor{

class ISRDLEDITOR_EXPORT IsrdlEditorWidget
    : public dsleditor::DslEditorWidget
{
  Q_OBJECT

public:
  explicit IsrdlEditorWidget(QWidget *parent = 0);
  virtual ~IsrdlEditorWidget();

public:
  virtual void duplicateFrom(BaseTextEditorWidget* editor) override;

public:
  const IsrdlDocument::Ptr&  isrdlDocument() const;
  void                        unCommentSelection();
  QModelIndex                 outlineModelIndex();

public:
  void showIsrdlGraphic();
  void highlightGraphicItemByUuid(const QUuid& itemUuid,
                                  bool highlight,
                                  bool unhighlightOtherItems = false);
  void highlightGraphicItemsByUuids(const QList<QUuid>& itemUuids,
                                    bool highlight,
                                    bool unhighlightOtherItems = true);
  void highlightGraphicItemByName(const QString& itemName,
                                  bool highlight,
                                  bool unhighlightOtherItems = false);
  void highlightGraphicItemsByNames(const QList<QString>& itemNames,
                                    bool highlight,
                                    bool unhighlightOtherItems = true);


public slots:
  virtual void setFontSettings(const TextEditor::FontSettings &) override;

signals:
  void outlineModelIndexChanged(const QModelIndex &index);

protected:
  IsrdlEditor*                   createEditor();
  QVector<TextEditor::TextStyle>  highlighterFormatCategories();
  TextEditor::IAssistInterface*   createAssistInterface(TextEditor::AssistKind assistKind,
                                                        TextEditor::AssistReason assistReason) const override;

private:
  QModelIndex indexForPosition(unsigned cursorPosition) const;

private slots:
  void updateOutline();
  void updateOutlineNow();
  void updateOutlineIndexNow();

private:
  void setSelectedElements();
  QString wordUnderCursor() const;

private:
  Q_DISABLE_COPY(IsrdlEditorWidget)
  class Private;
  Private*  _d;
};

} //end namespace isrdleditor
