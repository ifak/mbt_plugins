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
#ifndef STATEMACHINEEDITOR_COMBUILDSTATEMACHINEOUTLINEMODEL_HPP
#define STATEMACHINEEDITOR_COMBUILDSTATEMACHINEOUTLINEMODEL_HPP

#include <dslparser/statemachine/combuildstatemachinemodel.hpp>

namespace statemachineeditor {

class StateMachineOutlineModel;

class ComBuildStateMachineOutlineModel
    : public dslparser::statemachine::ComBuildStateMachineModel
{
public:
  ComBuildStateMachineOutlineModel(const QString& statemachineDocText,
                             StateMachineOutlineModel* stateMachineOutlineModel,const QString &praefix = QString(""),
                             QObject* parent = 0);
  virtual ~ComBuildStateMachineOutlineModel();

public:
  bool execute(QString* errorString = 0);
};

} // namespace statemachineeditor

#endif // STATEMACHINEEDITOR_COMBUILDSTATEMACHINEOUTLINEMODEL_HPP
