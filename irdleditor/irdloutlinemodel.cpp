#include "irdloutlinemodel.hpp"

//#include <mobata/model/requirement/placeitem.hpp>
//#include <mobata/model/requirement/transitionitem.hpp>

#include <mobata/memory_leak_start.hpp>

using namespace dslparser;

namespace irdleditor {

class IrdlOutlineModel::Private
{
  friend class IrdlOutlineModel;

  ModelTextLocations    _modelTextLocations;
  TokenTextLocations  _keywordTextLocations;
};

IrdlOutlineModel::IrdlOutlineModel(QObject* parent)
  : model::irdl::RequirementsModel(parent),
    _d(new Private)
{}

IrdlOutlineModel::~IrdlOutlineModel()
{
  delete this->_d;
}

void IrdlOutlineModel::reset()
{
  model::irdl::RequirementsModel::reset();

  this->_d->_modelTextLocations.clear();
  this->_d->_keywordTextLocations.clear();

  return;
}

TokenTextLocation const* IrdlOutlineModel::modelTextLocation(const QModelIndex &modelIntex) const
{
  ModelTextLocations::ConstIterator mt_it=this->_d->_modelTextLocations.find(modelIntex);
  if(mt_it == this->_d->_modelTextLocations.end())
    return 0;

  return &mt_it.value();
}

const ModelTextLocations& IrdlOutlineModel::modelTextSelections() const
{
  return this->_d->_modelTextLocations;
}

const TokenTextLocations& IrdlOutlineModel::keywordTextSelections() const
{
  return this->_d->_keywordTextLocations;
}

void IrdlOutlineModel::setModelTextLocations(const ModelTextLocations& modelTextLocations)
{
  this->_d->_modelTextLocations = modelTextLocations;

  return;
}

void IrdlOutlineModel::setKeywordTextLocations(const TokenTextLocations& keywordTextLocations)
{
  this->_d->_keywordTextLocations = keywordTextLocations;

  return;
}
const QModelIndex IrdlOutlineModel::uuidToIndexRec(const QUuid& uuid, QList<model::msc::MscSequenceItem const*> itemList) const
{
  for(const model::msc::MscSequenceItem* seqItem : itemList)
  {
    if(seqItem->uuid() == uuid){
      return seqItem->index();
    }
    if(seqItem->stepType()==model::msc::FragmentStep)
    {
      const model::msc::MscFragmentItem* fragmentItem = dynamic_cast<const model::msc::MscFragmentItem*>(seqItem);
      if(!fragmentItem){
        continue;
      }
      for(const model::msc::MscFragmentRegionItem* region : fragmentItem->regions())
      {
        QModelIndex back = this->uuidToIndexRec(uuid, region->sequenceItems());
        if(back != this->index(0,0)){
          return back;
        }
      }
    }
  }
  return this->index(0,0);
}
const QModelIndex IrdlOutlineModel::uuidToIndex(const QUuid& uuid) const
{
  return this->uuidToIndexRec(uuid, this->requirements().first()->sequenceItems());
}
} // namespace irdleditor
