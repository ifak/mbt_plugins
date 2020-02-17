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

#include "commondecleditor_global.hpp"

#include <texteditor/basetexteditor.h>

#include "commondecldocument.hpp"

namespace model{ namespace commondecl{
class DeclModel;
}}

namespace commondecleditor{

class CommonDeclEditorWidget;
class CommonDeclOutlineModel;

class COMMONDECLEDITOR_EXPORT CommonDeclEditor
    : public TextEditor::BaseTextEditor
{
  Q_OBJECT

public:
  explicit CommonDeclEditor(CommonDeclEditorWidget* commondeclEditorWidget);
  virtual ~CommonDeclEditor();

public:
  static
  Core::Id        commondeclEditorId();

  bool            duplicateSupported() const;
  Core::IEditor*  duplicate(QWidget *parent);
  bool            isTemporary() const;
  Core::Id        id() const;

  CommonDeclDocument*     document();
  CommonDeclEditorWidget* commondeclEditorWidget();

  model::base::BaseModel* commondeclDeclModel();
  CommonDeclOutlineModel*       commondeclOutlineModel();

private:
  Q_DISABLE_COPY(CommonDeclEditor)
  class Private;
  Private* _d;
};

} //end namespace commondecleditor
