/*
 *  This file is part of simsystem Editor Plugin for AppCreator based application.
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

#ifndef STATEMACHINEEDITOR_STATEMACHINEEDITORWIDGET_HPP
#define STATEMACHINEEDITOR_STATEMACHINEEDITORWIDGET_HPP

#include <dsleditor/dsleditorwidget.hpp>
#include <coreplugin/icore.h>

#include "simsystemeditor.hpp"

namespace specsim{

class SPECSIMSHARED_EXPORT SimSystemEditorWidget
    : public dsleditor::DslEditorWidget
{
  Q_OBJECT

public:
  explicit SimSystemEditorWidget(QWidget *parent = 0);
  virtual ~SimSystemEditorWidget();

public:
  virtual void duplicateFrom(BaseTextEditorWidget* editor) override;

public:
  const SimSystemDocument::Ptr&  simSystemDocument() const;
  void                              unCommentSelection() override;
  QModelIndex                       outlineModelIndex();

public:
  void showSimSystemGraphic();
  void highlightGraphicItemByUuid(const QUuid& itemUuid,
                                  bool highlight,
                                  bool unhighlightOtherItems = true);
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
  SimSystemEditor*             createEditor() override;
  QVector<TextEditor::TextStyle>  highlighterFormatCategories() override;
  TextEditor::IAssistInterface*   createAssistInterface(TextEditor::AssistKind assistKind,
                                                        TextEditor::AssistReason assistReason) const override;

private:
  QModelIndex indexForPosition(unsigned cursorPosition) const;

private slots:
  void updateOutline();
  void updateOutlineNow();
  void updateOutlineIndexNow();

private:
  void    setSelectedElements();
  QString wordUnderCursor() const;

private:
  Q_DISABLE_COPY(SimSystemEditorWidget)
  class Private;
  Private*  _d;
};

} //end namespace simsystemeditor

#endif //STATEMACHINEEDITOR_STATEMACHINEEDITORWIDGET_HPP
