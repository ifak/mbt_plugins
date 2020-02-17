#include "simsystemoutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>
#include <QDebug>

using namespace dslparser;

namespace specsim {

class SimSystemOutlineModel::Private
{
  friend class SimSystemOutlineModel;

  ModelTextLocations    _modelTextLocations;
  TokenTextLocations  _keywordTextLocations;
};

SimSystemOutlineModel::SimSystemOutlineModel(QObject* parent)
  : modsim::SimSystemModel(),
    _d(new Private)
{
  Q_UNUSED(parent);
}

SimSystemOutlineModel::~SimSystemOutlineModel()
{
  delete this->_d;
}

void SimSystemOutlineModel::reset()
{
  modsim::SimSystemModel::reset();

  this->_d->_modelTextLocations.clear();
  this->_d->_keywordTextLocations.clear();

  return;
}

TokenTextLocation const* SimSystemOutlineModel::modelTextLocation(const QModelIndex &modelIntex) const
{
  ModelTextLocations::ConstIterator mt_it=this->_d->_modelTextLocations.find(modelIntex);
  if(mt_it == this->_d->_modelTextLocations.end())
    return 0;

  return &mt_it.value();
}

const ModelTextLocations& SimSystemOutlineModel::modelTextSelections() const
{
  return this->_d->_modelTextLocations;
}

const TokenTextLocations& SimSystemOutlineModel::keywordTextSelections() const
{
  return this->_d->_keywordTextLocations;
}

void SimSystemOutlineModel::setModelTextLocations(const ModelTextLocations& modelTextLocations)
{
  this->_d->_modelTextLocations = modelTextLocations;

  return;
}

void SimSystemOutlineModel::setKeywordTextLocations(const TokenTextLocations& keywordTextLocations)
{
  this->_d->_keywordTextLocations = keywordTextLocations;

  return;
}

} // namespace simsystemeditor
