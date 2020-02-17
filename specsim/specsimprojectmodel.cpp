#include "specsimprojectmodel.hpp"

#include "specsimproject.hpp"
#include "specsimprojectconfiguration.hpp"
#include "specsimmodelsettings.hpp"
//#include "specsimspenatmodel.hpp"
//#include "specsimstatemachinemodel.hpp"
#include "specsimmodel.hpp" // temporarily include
#include "specsimconstants.hpp"

//#include <mobata/model/spenat/spenatmodel.hpp>
//#include <mobata/model/statemachine/statemachinemodel.hpp>

#include <QMenu>

namespace specsim {

typedef QHash<SpecSimModel*, QStandardItem*>  SpecSimModelSet;

class SpecSimProjectModel::Private
{
  friend class SpecSimProjectModel;

  SpecSimProject* _specSimProject;
  SpecSimModelSet _specSimModels;

  Private(SpecSimProject* specSimProject)
    : _specSimProject(specSimProject)
  {}
};

SpecSimProjectModel::SpecSimProjectModel(SpecSimProject* specSimProject)
  : QStandardItemModel(specSimProject), _d(new Private(specSimProject))
{
  Q_ASSERT(this->_d->_specSimProject);

  this->setColumnCount(1);
  this->setHeaderData(0, Qt::Horizontal,
                      tr("Simulation Models"),
                      Qt::DisplayRole);

  connect(this, &SpecSimProjectModel::itemChanged,
          this, &SpecSimProjectModel::handleItemChange);
}

SpecSimProjectModel::~SpecSimProjectModel()
{
  delete this->_d;
}

void SpecSimProjectModel::reset()
{
  qDeleteAll(this->_d->_specSimModels.keys());
  this->_d->_specSimModels.clear();
  this->invisibleRootItem()->removeRows(0, this->invisibleRootItem()->rowCount());

  return;
}

void SpecSimProjectModel::addSpecSimModel(SpecSimModel *specSimModel)
{
  if(!specSimModel)
    return;

  if(this->_d->_specSimModels.find(specSimModel)!=this->_d->_specSimModels.end())
    return;

  QStandardItem* newSpecModelItem = new QStandardItem(specSimModel->name());
  newSpecModelItem->setIcon(specSimModel->icon());
  newSpecModelItem->setFlags(newSpecModelItem->flags() & ~Qt::ItemIsEditable);
  newSpecModelItem->setCheckable(true);
  this->invisibleRootItem()->appendRow(newSpecModelItem);

  this->_d->_specSimModels.insert(specSimModel, newSpecModelItem);

  this->sort(0, Qt::AscendingOrder);

  return;
}

void SpecSimProjectModel::updateSpecSimModel(SpecSimModel *specSimModel)
{
  QStandardItem* specSimModelItem = this->_d->_specSimModels.value(specSimModel);
  if(!specSimModelItem)
    return;

  if(specSimModel->isEnabled())
    specSimModelItem->setCheckState(Qt::Checked);
  else
    specSimModelItem->setCheckState((Qt::Unchecked));

  return;
}

SpecSimModel* SpecSimProjectModel::specSimModel(const QString& specSimModelFilePath) const
{
  for(SpecSimModel* specSimModel : this->_d->_specSimModels.keys())
  {
    if(specSimModel->specSimModelFilePath() == specSimModelFilePath)
      return specSimModel;
  }

  return 0;
}

QModelIndex SpecSimProjectModel::specSimModelIndexOf(const QString& specSimModelFilePath) const
{
  SpecSimModel* specSimModel = this->specSimModel(specSimModelFilePath);
  if(!specSimModel)
    return QModelIndex();

  QStandardItem const* specSimModelItem = this->_d->_specSimModels.value(specSimModel);
  if(!specSimModelItem)
    return QModelIndex();

  return specSimModelItem->index();
}

SpecSimModel* SpecSimProjectModel::specSimModelForIndex(const QModelIndex& modelIndex) const
{
  if(!modelIndex.isValid())
    return nullptr;

  QStandardItem* specSimModelItem = this->itemFromIndex(modelIndex);
  if(!specSimModelItem)
    return nullptr;

  return this->_d->_specSimModels.key(specSimModelItem);
}

QList<SpecSimModel*> SpecSimProjectModel::specSimModels() const
{
  return this->_d->_specSimModels.keys();
}

Qt::DropActions SpecSimProjectModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

void SpecSimProjectModel::handleItemChange(QStandardItem* item)
{
  if(!item)
    return;

  SpecSimModel* specSimModel = this->_d->_specSimModels.key(item);
  if(specSimModel)
  {
    specSimModel->setEnabled((item->checkState()==Qt::Checked));
    SpecSimProjectConfiguration const* specSimProjectConfiguration = this->_d->_specSimProject->specSimProjectConfiguration();
    Q_ASSERT(specSimProjectConfiguration);
    SpecSimModelSettings* specSimModelSettings = specSimProjectConfiguration
                                                 ->specSimModelSettings(specSimModel->specSimModelFilePath());
    Q_ASSERT(specSimModelSettings);
    specSimModelSettings->setEnabled(specSimModel->isEnabled());
  }

  return;
}

} // namespace specsim
