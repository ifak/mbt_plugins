#include "commondecloutlinemodel.hpp"

//#include <mobata/model/requirement/placeitem.hpp>
//#include <mobata/model/requirement/transitionitem.hpp>

#include <mobata/memory_leak_start.hpp>

using namespace dslparser;

namespace commondecleditor {

class CommonDeclOutlineModel::Private
{
  friend class CommonDeclOutlineModel;

  ModelTextLocations    _modelTextLocations;
  TokenTextLocations  _keywordTextLocations;
};

CommonDeclOutlineModel::CommonDeclOutlineModel(QObject* parent)
  : model::base::BaseModel(parent),
    _d(new Private)
{}

CommonDeclOutlineModel::~CommonDeclOutlineModel()
{
  delete this->_d;
}

void CommonDeclOutlineModel::reset()
{
  model::base::BaseModel::reset();

  this->_d->_modelTextLocations.clear();
  this->_d->_keywordTextLocations.clear();

  return;
}

TokenTextLocation const* CommonDeclOutlineModel::modelTextLocation(const QModelIndex &modelIntex) const
{
  ModelTextLocations::ConstIterator mt_it=this->_d->_modelTextLocations.find(modelIntex);
  if(mt_it == this->_d->_modelTextLocations.end())
    return 0;

  return &mt_it.value();
}

const ModelTextLocations& CommonDeclOutlineModel::modelTextSelections() const
{
  return this->_d->_modelTextLocations;
}

const TokenTextLocations& CommonDeclOutlineModel::keywordTextSelections() const
{
  return this->_d->_keywordTextLocations;
}

void CommonDeclOutlineModel::setModelTextLocations(const ModelTextLocations& modelTextLocations)
{
  this->_d->_modelTextLocations = modelTextLocations;

  return;
}

void CommonDeclOutlineModel::setKeywordTextLocations(const TokenTextLocations& keywordTextLocations)
{
  this->_d->_keywordTextLocations = keywordTextLocations;

  return;
}

} // namespace commondecleditor
