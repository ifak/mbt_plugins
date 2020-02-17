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

#include "spenateditor_global.hpp"

#include <texteditor/basetexteditor.h>

#include "spenatdocument.hpp"

namespace model{ namespace spenat{
class SpenatDeclModel;
}}

namespace spenateditor{

class SpenatEditorWidget;
class SpenatOutlineModel;

class SPENATEDITOR_EXPORT SpenatEditor
    : public TextEditor::BaseTextEditor
{
  Q_OBJECT

public:
  explicit SpenatEditor(SpenatEditorWidget* spenatEditorWidget);
  virtual ~SpenatEditor();

public:
  static
  Core::Id        spenatEditorId();

  bool            duplicateSupported() const;
  Core::IEditor*  duplicate(QWidget *parent);
  bool            isTemporary() const;
  Core::Id        id() const;

  SpenatDocument*     document();
  SpenatEditorWidget* spenatEditorWidget();

  model::spenat::SpenatDeclModel* spenatDeclModel();
  SpenatOutlineModelPtr     spenatOutlineModel();

private:
  Q_DISABLE_COPY(SpenatEditor)
  class Private;
  Private* _d;
};

} //end namespace spenateditor
