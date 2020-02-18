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

#include "specsim_global.hpp"

#include <texteditor/basetexteditor.h>

#include "simsystemdocument.hpp"

namespace modsim{
class SimSystemModel;
}

namespace specsim{

class SimSystemEditorWidget;
class SimSystemOutlineModel;

class SPECSIMSHARED_EXPORT SimSystemEditor
    : public TextEditor::BaseTextEditor
{
  Q_OBJECT

public:
  explicit SimSystemEditor(SimSystemEditorWidget* SimSystemEditorWidget);
  virtual ~SimSystemEditor();

public:
  static
  Core::Id        simSystemEditorId();

  bool            duplicateSupported() const;
  Core::IEditor*  duplicate(QWidget *parent);
  bool            isTemporary() const;
  Core::Id        id() const;

  SimSystemDocument* document();

  modsim::SimSystemModel* simsystemDeclModel();
  SimSystemOutlineModelPtr             simsystemOutlineModel();

private:
  Q_DISABLE_COPY(SimSystemEditor)
  class Private;
  Private* _d;
};

} //end namespace simsystemeditor
