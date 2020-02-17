#include "spenatoutlinemodel.hpp"

#include <mobata/model/spenat/placeitem.hpp>
#include <mobata/model/spenat/transitionitem.hpp>

#include <mobata/memory_leak_start.hpp>

using namespace dslparser;

namespace spenateditor {

class SpenatOutlineModel::Private
{
  friend class SpenatOutlineModel;

  ModelTextLocations    _modelTextLocations;
  TokenTextLocations  _keywordTextLocations;
};

SpenatOutlineModel::SpenatOutlineModel(QObject* parent)
  : model::spenat::SpenatDeclModel(parent),
    _d(new Private)
{}

SpenatOutlineModel::~SpenatOutlineModel()
{
  delete this->_d;
}

void SpenatOutlineModel::reset()
{
  model::spenat::SpenatDeclModel::reset();

  this->_d->_modelTextLocations.clear();
  this->_d->_keywordTextLocations.clear();

  return;
}

TokenTextLocation const* SpenatOutlineModel::modelTextLocation(const QModelIndex &modelIntex) const
{
  ModelTextLocations::ConstIterator mt_it=this->_d->_modelTextLocations.find(modelIntex);
  if(mt_it == this->_d->_modelTextLocations.end())
    return 0;

  return &mt_it.value();
}

const ModelTextLocations& SpenatOutlineModel::modelTextSelections() const
{
  return this->_d->_modelTextLocations;
}

const TokenTextLocations& SpenatOutlineModel::keywordTextSelections() const
{
  return this->_d->_keywordTextLocations;
}

model::spenat::PlaceItem const* SpenatOutlineModel::placeFromIndex(const QModelIndex& modelIndex) const
{
  for(model::spenat::PlaceItem const* currentPlace
      : this->places())
  {
    if(currentPlace->index()==modelIndex)
      return currentPlace;
  }

  return nullptr;
}

model::spenat::TransitionItem const* SpenatOutlineModel::transitionFromIndex(const QModelIndex& modelIndex) const
{
  for(model::spenat::TransitionItem const* currentTransition
      : this->transitions())
  {
    if(currentTransition->index()==modelIndex)
      return currentTransition;
  }

  return nullptr;
}

void SpenatOutlineModel::setModelTextLocations(const ModelTextLocations& modelTextLocations)
{
  this->_d->_modelTextLocations = modelTextLocations;

  return;
}

void SpenatOutlineModel::setKeywordTextLocations(const TokenTextLocations& keywordTextLocations)
{
  this->_d->_keywordTextLocations = keywordTextLocations;

  return;
}
const QModelIndex SpenatOutlineModel::uuidToIndex(const QUuid& uuid) const
{
    for(model::spenat::PlaceItem const* currentPlace
        : this->places())
    {
        if(currentPlace->uuid()==uuid)
            return currentPlace->index();
    }
    for(model::spenat::TransitionItem const* currentTransition
        : this->transitions())
    {
        if(currentTransition->uuid()==uuid)
            return currentTransition->index();
    }
    return this->index(0,0);
}
} // namespace spenateditor
