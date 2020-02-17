#include "statemachineoutlinemodel.hpp"

#include <mobata/model/statemachine/abstractstateitem.hpp>
#include <mobata/model/statemachine/compositestateitem.hpp>
#include <mobata/model/statemachine/transitionitem.hpp>

#include <mobata/memory_leak_start.hpp>
#include <QDebug>

using namespace dslparser;

namespace statemachineeditor {

class StateMachineOutlineModel::Private
{
  friend class StateMachineOutlineModel;

  ModelTextLocations    _modelTextLocations;
  TokenTextLocations  _keywordTextLocations;
};

StateMachineOutlineModel::StateMachineOutlineModel(QObject* parent)
  : model::statemachine::StateMachineModel(parent),
    _d(new Private)
{}

StateMachineOutlineModel::~StateMachineOutlineModel()
{
  delete this->_d;
}

void StateMachineOutlineModel::reset()
{
  model::statemachine::StateMachineModel::reset();

  this->_d->_modelTextLocations.clear();
  this->_d->_keywordTextLocations.clear();

  return;
}

TokenTextLocation const* StateMachineOutlineModel::modelTextLocation(const QModelIndex &modelIntex) const
{
  ModelTextLocations::ConstIterator mt_it=this->_d->_modelTextLocations.find(modelIntex);
  if(mt_it == this->_d->_modelTextLocations.end())
    return 0;

  return &mt_it.value();
}

const ModelTextLocations& StateMachineOutlineModel::modelTextSelections() const
{
  return this->_d->_modelTextLocations;
}

const TokenTextLocations& StateMachineOutlineModel::keywordTextSelections() const
{
  return this->_d->_keywordTextLocations;
}

void StateMachineOutlineModel::setModelTextLocations(const ModelTextLocations& modelTextLocations)
{
  this->_d->_modelTextLocations = modelTextLocations;

  return;
}

void StateMachineOutlineModel::setKeywordTextLocations(const TokenTextLocations& keywordTextLocations)
{
  this->_d->_keywordTextLocations = keywordTextLocations;

  return;
}

model::statemachine::AbstractStateItem const* StateMachineOutlineModel::stateFromIndex(const QModelIndex& modelIndex) const{
  return dynamic_cast<model::statemachine::AbstractStateItem const*>(this->itemFromIndex(modelIndex));
}

model::statemachine::TransitionItem const* StateMachineOutlineModel::transitionFromIndex(const QModelIndex& modelIndex) const{
  return dynamic_cast<model::statemachine::TransitionItem const*>(this->itemFromIndex(modelIndex));
}

const QModelIndex StateMachineOutlineModel::checkChildrenForIndex(model::statemachine::AbstractStateItem const* parentState, const QUuid& uuid) const
{
  if(parentState->stateType() == model::statemachine::Composite){
    const model::statemachine::CompositeStateItem* compositeState = dynamic_cast<const model::statemachine::CompositeStateItem*>(parentState);
    for(model::statemachine::AbstractStateItem const* currentPlace : compositeState->states())
    {
      if(currentPlace->uuid()==uuid){
        return currentPlace->index();
      }
      if(currentPlace->stateType() == model::statemachine::Composite)
      {
        const QModelIndex index = this->checkChildrenForIndex(currentPlace,uuid);
        if (index != this->index(0,0)){
          return index;
        }
      }
    }
  }
  return this->index(0,0);
}
const QModelIndex StateMachineOutlineModel::uuidToIndex(const QUuid& uuid) const
{
    for(model::statemachine::AbstractStateItem const* currentPlace
        : this->states())
    {
        if(currentPlace->uuid()==uuid){
            return currentPlace->index();
        }
        if(currentPlace->stateType() == model::statemachine::Composite)
        {
          const QModelIndex index = this->checkChildrenForIndex(currentPlace,uuid);
          if (index != this->index(0,0)){
            return index;
          }
        }
    }
    for(model::statemachine::TransitionItem const* currentTransition
        : this->transitions())
    {
        if(currentTransition->uuid()==uuid)
            return currentTransition->index();
    }
    return this->index(0,0);
}
} // namespace statemachineeditor
