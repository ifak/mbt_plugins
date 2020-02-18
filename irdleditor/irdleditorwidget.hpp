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

#include "irdleditor_global.hpp"

#include <dsleditor/dsleditorwidget.hpp>
#include <coreplugin/icore.h>

#include "irdleditor.hpp"

#include <QUuid>

namespace irdleditor{

class IRDLEDITOR_EXPORT IrdlEditorWidget
    : public dsleditor::DslEditorWidget
{
  Q_OBJECT

public:
  explicit IrdlEditorWidget(QWidget *parent = 0);
  virtual ~IrdlEditorWidget();

public:
  virtual void duplicateFrom(BaseTextEditorWidget* editor) override;

public:
  const IrdlDocument::Ptr&  irdlDocument() const;
  void                        unCommentSelection();
  QModelIndex                 outlineModelIndex();

public:
  void showIrdlGraphic();
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
  void selectedGrapicItemByUuidIn(const QUuid& uuid);

public slots:
  virtual void setFontSettings(const TextEditor::FontSettings &) override;

signals:
  void outlineModelIndexChanged(const QModelIndex &index);

protected:
  IrdlEditor*                   createEditor();
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
  Q_DISABLE_COPY(IrdlEditorWidget)
  class Private;
  Private*  _d;
};

} //end namespace irdleditor
