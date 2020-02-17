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

#include <mobata/model/statemachine/statemachinemodel.hpp>

#include <dslparser/dslparser_types.hpp>

namespace statemachineeditor {

class StateMachineOutlineModel : public model::statemachine::StateMachineModel
{
  friend class ComBuildStateMachineOutlineModel;

public:
  typedef QSharedPointer<StateMachineOutlineModel> Ptr;

public:
  StateMachineOutlineModel(QObject* parent = 0);
  virtual ~StateMachineOutlineModel();

public:
  virtual void reset();

public:
  dslparser::TokenTextLocation const* modelTextLocation(const QModelIndex& modelIntex) const;

public:
  const dslparser::ModelTextLocations& modelTextSelections() const;
  const dslparser::TokenTextLocations& keywordTextSelections() const;
  model::statemachine::AbstractStateItem const* stateFromIndex(const QModelIndex& modelIndex) const;
  model::statemachine::TransitionItem const* transitionFromIndex(const QModelIndex& modelIndex) const;
  const QModelIndex uuidToIndex(const QUuid& uuid) const;

private:
  const QModelIndex checkChildrenForIndex(model::statemachine::AbstractStateItem const* parentState, const QUuid& uuid) const;
  void setModelTextLocations(const dslparser::ModelTextLocations& modelTextLocations);
  void setKeywordTextLocations(const dslparser::TokenTextLocations& keywordTextLocations);

private:
  Q_DISABLE_COPY(StateMachineOutlineModel)
  class Private;
  Private* _d;
};

typedef StateMachineOutlineModel::Ptr StateMachineOutlineModelPtr;

} // namespace statemachineeditor
