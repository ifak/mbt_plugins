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

#include "statemachineeditor_global.hpp"

#include <texteditor/basetextdocument.h>
#include "statemachineoutlinemodel.hpp"

namespace model{ namespace statemachine{
class StateMachineModel;
}}

namespace statemachineeditor{

class StateMachineOutlineModel;

class StateMachineDocument
    : public TextEditor::BaseTextDocument
{
  Q_OBJECT

public:
  typedef QSharedPointer<StateMachineDocument> Ptr;

public:
  explicit StateMachineDocument(QObject *parent = 0);
  virtual ~StateMachineDocument();

public:
  model::statemachine::StateMachineModel* statemachineDeclModel();
  const StateMachineOutlineModelPtr&      statemachineOutlineModel();

private:
  Q_DISABLE_COPY(StateMachineDocument)
  class Private;
  Private* _d;
};

} //end namespace statemachineeditor
