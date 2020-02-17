#include "tcgcoverageproperty.hpp"

#include "mbtspecmodel.hpp"

#include <mobata/model/ts/testsuiteitem.hpp>

namespace mbtproject {

class TcgCoverageProperty::Private
{
  friend class TcgCoverageProperty;

  QString                   _tcgCoverage;
  MbtSpecModel*             _mbtSpecModel;
  QString                   _tcgOutputFilePath;
  model::ts::TestSuiteItem* _tcgTestSuiteItem;

  Private(const QString& tcgCoverage,
          MbtSpecModel* mbtSpecModel)
    : _tcgCoverage(tcgCoverage),
      _mbtSpecModel(mbtSpecModel),
      _tcgTestSuiteItem(0)
  {}
};

TcgCoverageProperty::TcgCoverageProperty(const QString& tcgCoverage,
                                         MbtSpecModel* mbtSpecModel)
  : QObject(mbtSpecModel), _d(new Private(tcgCoverage, mbtSpecModel))
{
  Q_ASSERT(this->_d->_mbtSpecModel);
}

TcgCoverageProperty::~TcgCoverageProperty()
{
  if(this->_d->_tcgTestSuiteItem
     && !this->_d->_tcgTestSuiteItem->parent())
    delete this->_d->_tcgTestSuiteItem;

  delete this->_d;
}

MbtSpecModel* TcgCoverageProperty::mbtSpecModel() const
{
  return this->_d->_mbtSpecModel;
}

const QString& TcgCoverageProperty::tcgCoverage() const
{
  return this->_d->_tcgCoverage;
}

void TcgCoverageProperty::setTcgCoverage(const QString& tcgCoverage)
{
  this->_d->_tcgCoverage = tcgCoverage;

  return;
}

const QString& TcgCoverageProperty::tcgOutputFilePath() const
{
  return this->_d->_tcgOutputFilePath;
}

void TcgCoverageProperty::setTcgOutputFilePath(const QString& tcgOutputFilePath)
{
  this->_d->_tcgOutputFilePath = tcgOutputFilePath;

  return;
}

void TcgCoverageProperty::setTcgTestSuiteItem(model::ts::TestSuiteItem* testSuiteItem)
{
  if(this->_d->_tcgTestSuiteItem
     && !this->_d->_tcgTestSuiteItem->parent())
    delete this->_d->_tcgTestSuiteItem;

  this->_d->_tcgTestSuiteItem = testSuiteItem;

  return;
}

model::ts::TestSuiteItem* TcgCoverageProperty::tcgTestSuiteItem() const
{
  return this->_d->_tcgTestSuiteItem;
}

} // namespace mbtproject
