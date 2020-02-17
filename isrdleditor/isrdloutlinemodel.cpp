#include "isrdloutlinemodel.hpp"

//#include <mobata/model/requirement/placeitem.hpp>
//#include <mobata/model/requirement/transitionitem.hpp>

#include <mobata/memory_leak_start.hpp>

using namespace dslparser;

namespace isrdleditor {

class IsrdlOutlineModel::Private
{
  friend class IsrdlOutlineModel;

  ModelTextLocations    _modelTextLocations;
  TokenTextLocations  _keywordTextLocations;
};

IsrdlOutlineModel::IsrdlOutlineModel(QObject* parent)
  : model::isrdl::IsrdlModel(parent),
    _d(new Private)
{}

IsrdlOutlineModel::~IsrdlOutlineModel()
{
  delete this->_d;
}

void IsrdlOutlineModel::reset()
{
  model::isrdl::IsrdlModel::reset();

  this->_d->_modelTextLocations.clear();
  this->_d->_keywordTextLocations.clear();

  return;
}

TokenTextLocation const* IsrdlOutlineModel::modelTextLocation(const QModelIndex &modelIntex) const
{
  ModelTextLocations::ConstIterator mt_it=this->_d->_modelTextLocations.find(modelIntex);
  if(mt_it == this->_d->_modelTextLocations.end())
    return 0;

  return &mt_it.value();
}

const ModelTextLocations& IsrdlOutlineModel::modelTextSelections() const
{
  return this->_d->_modelTextLocations;
}

const TokenTextLocations& IsrdlOutlineModel::keywordTextSelections() const
{
  return this->_d->_keywordTextLocations;
}

void IsrdlOutlineModel::setModelTextLocations(const ModelTextLocations& modelTextLocations)
{
  this->_d->_modelTextLocations = modelTextLocations;

  return;
}

void IsrdlOutlineModel::setKeywordTextLocations(const TokenTextLocations& keywordTextLocations)
{
  this->_d->_keywordTextLocations = keywordTextLocations;

  return;
}

} // namespace isrdleditor
