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

#include <texteditor/basetexteditor.h>

#include "irdldocument.hpp"

namespace model{ namespace irdl{
class DeclModel;
}}

namespace irdleditor{

class IrdlEditorWidget;
class IrdlOutlineModel;

class IRDLEDITOR_EXPORT IrdlEditor
    : public TextEditor::BaseTextEditor
{
  Q_OBJECT

public:
  explicit IrdlEditor(IrdlEditorWidget* irdlEditorWidget);
  virtual ~IrdlEditor();

public:
  static
  Core::Id        irdlEditorId();

  bool            duplicateSupported() const;
  Core::IEditor*  duplicate(QWidget *parent);
  bool            isTemporary() const;
  Core::Id        id() const;

  IrdlDocument*     document();
  IrdlEditorWidget* irdlEditorWidget();

  model::irdl::RequirementsModel* irdlDeclModel();
  IrdlOutlineModel*       irdlOutlineModel();

private:
  Q_DISABLE_COPY(IrdlEditor)
  class Private;
  Private* _d;
};

} //end namespace irdleditor
