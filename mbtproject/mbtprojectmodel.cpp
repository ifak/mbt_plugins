#include "mbtprojectmodel.hpp"

#include "mbtproject.hpp"
#include "mbtprojectconfiguration.hpp"
#include "mbtspecmodelsettings.hpp"
#include "tcgcoverageproperty.hpp"
#include "mbtspenatmodel.hpp"
#include "mbtstatemachinemodel.hpp"
#include "mbtprojectconstants.hpp"

#include <mobata/model/spenat/spenatmodel.hpp>
#include <mobata/model/statemachine/statemachinemodel.hpp>
#include <mobata/model/ts/testsuiteitem.hpp>

#include <QMenu>

namespace mbtproject {

typedef QHash<MbtSpecModel*, QStandardItem*>  MbtSpecModelSet;

class MbtProjectModel::Private
{
  friend class MbtProjectModel;

  MbtProject*     _mbtProject;
  MbtSpecModelSet _mbtSpecModels;

  QIcon           _tcgCoverageSpecifiedIcon;
  QIcon           _tcgCoverageNotSpecifiedIcon;

  Private(MbtProject* mbtProject)
    : _mbtProject(mbtProject)
  {}
};

MbtProjectModel::MbtProjectModel(MbtProject* mbtProject)
  : QStandardItemModel(mbtProject), _d(new Private(mbtProject))
{
  Q_ASSERT(this->_d->_mbtProject);

  this->setColumnCount(1);
  this->setHeaderData(0, Qt::Horizontal,
                      tr("MBT Specification Models"),
                      Qt::DisplayRole);

  this->_d->_tcgCoverageNotSpecifiedIcon=QIcon(QLatin1String(":/mbtproject/images/tcgcoverage_question.png"));
  this->_d->_tcgCoverageSpecifiedIcon=QIcon(QLatin1String(":/mbtproject/images/tcgcoverage_check.png"));

  connect(this, &MbtProjectModel::itemChanged,
          this, &MbtProjectModel::handleItemChange);
}

MbtProjectModel::~MbtProjectModel()
{
  delete this->_d;
}

void MbtProjectModel::reset()
{
  qDeleteAll(this->_d->_mbtSpecModels.keys());
  this->_d->_mbtSpecModels.clear();
  this->invisibleRootItem()->removeRows(0, this->invisibleRootItem()->rowCount());

  return;
}

void MbtProjectModel::addMbtSpecModel(MbtSpecModel* mbtSpecModel)
{
  if(!mbtSpecModel)
    return;

  if(this->_d->_mbtSpecModels.find(mbtSpecModel)!=this->_d->_mbtSpecModels.end())
    return;

  QStandardItem* newSpecModelItem = new QStandardItem(mbtSpecModel->name());
  newSpecModelItem->setIcon(mbtSpecModel->icon());
  newSpecModelItem->setFlags(newSpecModelItem->flags() & ~Qt::ItemIsEditable);
  newSpecModelItem->setCheckable(true);
  this->invisibleRootItem()->appendRow(newSpecModelItem);

  //tcg coverage items
  QStandardItem* tcgCoverageItem = new QStandardItem(QLatin1String("tcg coverage"));
  tcgCoverageItem->setFlags(tcgCoverageItem->flags() & ~Qt::ItemIsEditable);
  tcgCoverageItem->setIcon(this->_d->_tcgCoverageNotSpecifiedIcon);
  tcgCoverageItem->setData(TcgCoverageParentItem, MbtProjectItemRole);
  newSpecModelItem->appendRow(tcgCoverageItem);

  QStandardItem* allPlacesCoverageItem = new QStandardItem(QLatin1String(Constants::MBT_TCGCOVERAGE_ALLPLACES));
  allPlacesCoverageItem->setFlags(allPlacesCoverageItem->flags() & ~Qt::ItemIsEditable);
  allPlacesCoverageItem->setCheckable(true);
  allPlacesCoverageItem->setData(TcgCoverageItem, MbtProjectItemRole);
  tcgCoverageItem->appendRow(allPlacesCoverageItem);

  QStandardItem* allTransitionsCoverageItem = new QStandardItem(QLatin1String(Constants::MBT_TCGCOVERAGE_ALLTRANSITIONS));
  allTransitionsCoverageItem->setFlags(allTransitionsCoverageItem->flags() & ~Qt::ItemIsEditable);
  allTransitionsCoverageItem->setCheckable(true);
  allTransitionsCoverageItem->setData(TcgCoverageItem, MbtProjectItemRole);
  tcgCoverageItem->appendRow(allTransitionsCoverageItem);

  QStandardItem* allDecisionsCoverageItem = new QStandardItem(QLatin1String(Constants::MBT_TCGCOVERAGE_ALLDECISIONS));
  allDecisionsCoverageItem->setFlags(allDecisionsCoverageItem->flags() & ~Qt::ItemIsEditable);
  allDecisionsCoverageItem->setCheckable(true);
  allDecisionsCoverageItem->setData(TcgCoverageItem, MbtProjectItemRole);
  tcgCoverageItem->appendRow(allDecisionsCoverageItem);

  QStandardItem* allPathsCoverageItem = new QStandardItem(QLatin1String(Constants::MBT_TCGCOVERAGE_ALLPATHS));
  allPathsCoverageItem->setFlags(allPathsCoverageItem->flags() & ~Qt::ItemIsEditable);
  allPathsCoverageItem->setCheckable(true);
  allPathsCoverageItem->setData(TcgCoverageItem, MbtProjectItemRole);
  tcgCoverageItem->appendRow(allPathsCoverageItem);

  QStandardItem* allRoundTripPathsCoverageItem = new QStandardItem(QLatin1String(Constants::MBT_TCGCOVERAGE_ALLROUNDTRIPPATHS));
  allRoundTripPathsCoverageItem->setFlags(allRoundTripPathsCoverageItem->flags() & ~Qt::ItemIsEditable);
  allRoundTripPathsCoverageItem->setCheckable(false);
  allRoundTripPathsCoverageItem->setData(TcgCoverageItem, MbtProjectItemRole);
  allRoundTripPathsCoverageItem->setEnabled(false);
  tcgCoverageItem->appendRow(allRoundTripPathsCoverageItem);

  this->_d->_mbtSpecModels.insert(mbtSpecModel, newSpecModelItem);

  this->sort(0, Qt::AscendingOrder);

  return;
}

void MbtProjectModel::updateMbtSpecModel(MbtSpecModel* mbtSpecModel)
{
  QStandardItem* mbtSpecModelItem = this->_d->_mbtSpecModels.value(mbtSpecModel);
  if(!mbtSpecModelItem)
    return;

  if(mbtSpecModel->isEnabled())
    mbtSpecModelItem->setCheckState(Qt::Checked);
  else
    mbtSpecModelItem->setCheckState((Qt::Unchecked));

  //update check states of spec model child items recursively
  int rowCount = mbtSpecModelItem->rowCount();
  for(int i = 0; i<rowCount; ++i)
  {
    QStandardItem* childItem = mbtSpecModelItem->child(i);

    if(childItem->data(MbtProjectItemRole)==TcgCoverageParentItem)//tcg coverage parent item
    {
      int tcgCoverItemRowCount = childItem->rowCount();
      for(int j=0; j<tcgCoverItemRowCount; ++j)
      {
        QStandardItem* tcgCoverItem = childItem->child(j);

        TcgCoverageProperty* tcgCoverageProperty = mbtSpecModel->tcgCoverageProperty(tcgCoverItem->text());
        if(tcgCoverageProperty)
        {
          tcgCoverItem->setCheckState(Qt::Checked);
          if(QStandardItem* tcgTestSuiteItem = tcgCoverageProperty->tcgTestSuiteItem())
          {
            bool existing=false;
            int childCount = tcgCoverItem->rowCount();
            for(int i=0; i<childCount; ++i)
            {
              if(tcgCoverItem->child(i) == tcgTestSuiteItem)
              {
                existing = true;
                break;
              }
            }
            if(!existing)
              tcgCoverItem->appendRow(tcgTestSuiteItem);
          }
          else
            tcgCoverItem->removeRows(0, tcgCoverItem->rowCount());
        }
        else
        {
          tcgCoverItem->setCheckState(Qt::Unchecked);
          tcgCoverItem->removeRows(0, tcgCoverItem->rowCount());
        }
      }
    }
  }

  return;
}

MbtSpecModel* MbtProjectModel::mbtSpecModel(const QString& mbtSpecModelFilePath) const
{
  for(MbtSpecModel* mbtSpecModel : this->_d->_mbtSpecModels.keys())
  {
    if(mbtSpecModel->mbtSpecModelFilePath() == mbtSpecModelFilePath)
      return mbtSpecModel;
  }

  return 0;
}

QModelIndex MbtProjectModel::mbtSpecModelIndexOf(const QString& mbtSpecModelFilePath) const
{
  MbtSpecModel* mbtSpecModel = this->mbtSpecModel(mbtSpecModelFilePath);
  if(!mbtSpecModel)
    return QModelIndex();

  QStandardItem const* mbtSpecModelItem = this->_d->_mbtSpecModels.value(mbtSpecModel);
  if(!mbtSpecModelItem)
    return QModelIndex();

  return mbtSpecModelItem->index();
}

MbtSpecModel* MbtProjectModel::mbtSpecModelForIndex(const QModelIndex& modelIndex) const
{
  if(!modelIndex.isValid())
    return nullptr;

  QStandardItem* mbtSpecModelItem = this->itemFromIndex(modelIndex);
  if(!mbtSpecModelItem)
    return nullptr;

  return this->_d->_mbtSpecModels.key(mbtSpecModelItem);
}

QList<MbtSpecModel*> MbtProjectModel::mbtSpecModels() const
{
  return this->_d->_mbtSpecModels.keys();
}

Qt::DropActions MbtProjectModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

void MbtProjectModel::handleItemChange(QStandardItem* item)
{
  if(!item)
    return;

  MbtSpecModel* mbtSpecModel = this->_d->_mbtSpecModels.key(item);
  if(mbtSpecModel)
  {
    mbtSpecModel->setEnabled((item->checkState()==Qt::Checked));
    MbtProjectConfiguration const* mbtProjectConfiguration = this->_d->_mbtProject->mbtProjectConfiguration();
    Q_ASSERT(mbtProjectConfiguration);
    MbtSpecModelSettings* mbtSpecModelSettings = mbtProjectConfiguration
                                                 ->mbtSpecModelSettings(mbtSpecModel->mbtSpecModelFilePath());
    Q_ASSERT(mbtSpecModelSettings);
    mbtSpecModelSettings->setEnabled(mbtSpecModel->isEnabled());
  }
  else if(item->isCheckable()
          && item->data(MbtProjectItemRole) == TcgCoverageItem)
  {
    QStandardItem* parentTcgCoverageItem = item->parent();
    Q_ASSERT(parentTcgCoverageItem);

    QStandardItem* mbtSpecModelItem = parentTcgCoverageItem->parent();
    Q_ASSERT(mbtSpecModelItem);
    MbtSpecModel* mbtSpecModel = this->_d->_mbtSpecModels.key(mbtSpecModelItem);
    Q_ASSERT(mbtSpecModel);
    MbtProjectConfiguration const* mbtProjectConfiguration = this->_d->_mbtProject->mbtProjectConfiguration();
    Q_ASSERT(mbtProjectConfiguration);
    MbtSpecModelSettings* mbtSpecModelSettings = mbtProjectConfiguration
                                                 ->mbtSpecModelSettings(mbtSpecModel->mbtSpecModelFilePath());
    Q_ASSERT(mbtSpecModelSettings);

    bool oneTcgCoverageItemChecked = false;
    QString tcgCoverageText = item -> text();

    if(item->checkState()==Qt::Checked)
    {
      oneTcgCoverageItemChecked = true;
      mbtSpecModel->addTcgCoverageProperty(tcgCoverageText);
      mbtSpecModelSettings->addTcgCoverage(tcgCoverageText);
    }
    else
    {
      mbtSpecModel->removeTcgCoverageProperty(tcgCoverageText);
      item->removeRows(0, item->rowCount());
      mbtSpecModelSettings->removeTcgCoverage(tcgCoverageText);

      //children of parent tcg coverage item -> including item!///
      // for icon of parentTcgCoverageItem
      int rowCount = parentTcgCoverageItem->rowCount();
      for(int i = 0; i<rowCount; ++i)
      {
        QStandardItem* childItem = parentTcgCoverageItem->child(i);

        if(!childItem->isCheckable())
          continue;

        if(childItem == item)
          continue;

        if(childItem->checkState()==Qt::Checked)
        {
          oneTcgCoverageItemChecked=true;
          break;
        }
      }
    }

    if(oneTcgCoverageItemChecked)
      parentTcgCoverageItem->setIcon(this->_d->_tcgCoverageSpecifiedIcon);
    else
      parentTcgCoverageItem->setIcon(this->_d->_tcgCoverageNotSpecifiedIcon);
  }

  return;
}

} // namespace mbtproject
