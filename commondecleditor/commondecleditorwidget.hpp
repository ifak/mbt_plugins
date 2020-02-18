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

#include "commondecleditor_global.hpp"

#include <dsleditor/dsleditorwidget.hpp>
#include <coreplugin/icore.h>

#include "commondecleditor.hpp"

#include <QUuid>

namespace commondecleditor{

class COMMONDECLEDITOR_EXPORT CommonDeclEditorWidget
    : public dsleditor::DslEditorWidget
{
  Q_OBJECT

public:
  explicit CommonDeclEditorWidget(QWidget *parent = 0);
  virtual ~CommonDeclEditorWidget();

public:
  virtual void duplicateFrom(BaseTextEditorWidget* editor) override;

public:
  const CommonDeclDocument::Ptr&  commondeclDocument() const;
  void                        unCommentSelection() override;
  QModelIndex                 outlineModelIndex();

public slots:
  virtual void setFontSettings(const TextEditor::FontSettings &) override;

signals:
  void outlineModelIndexChanged(const QModelIndex &index);

protected:
  CommonDeclEditor*               createEditor() override;
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
  void setSelectedElements();
  QString wordUnderCursor() const;

private:
  Q_DISABLE_COPY(CommonDeclEditorWidget)
  class Private;
  Private*  _d;
};

} //end namespace commondecleditor
