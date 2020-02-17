#include "mbtspecmodel.hpp"

#include "mbtproject.hpp"
#include "tcgcoverageproperty.hpp"

#include <mobata/utils/functors.hpp>

#include <QFileInfo>
#include <QIcon>

namespace mbtproject {

class MbtSpecModel::Private
{
  friend class MbtSpecModel;

  QString                           _mbtSpecModelFilePath;
  QIcon                             _mbtSpecModelIcon;
  bool                              _enabled;
  QList<ProjectExplorer::BaseError> _errors;
  QString                           _tcgInputFilePath;
  QList<TcgCoverageProperty*>       _tcgCoverageProperties;

  Private(const QString& mbtSpecModelFilePath,
          const QIcon& mbtSpecModelIcon)
    : _mbtSpecModelFilePath(mbtSpecModelFilePath),
      _mbtSpecModelIcon(mbtSpecModelIcon),
      _enabled(false)
  {}
};

MbtSpecModel::MbtSpecModel(const QString& mbtSpecModelFilePath,
                           const QIcon& mbtSpecModelIcon,
                           QObject *parent)
  : QObject(parent),
    _d(new Private(mbtSpecModelFilePath, mbtSpecModelIcon))
{}

MbtSpecModel::~MbtSpecModel()
{
  this->clearTcgCoverageProperties();
  delete this->_d;
}

const QList<ProjectExplorer::BaseError>&  MbtSpecModel::errors() const
{
  return this->_d->_errors;
}

QString MbtSpecModel::name() const
{
  return QFileInfo(this->_d->_mbtSpecModelFilePath).baseName();
}

const QString& MbtSpecModel::mbtSpecModelFilePath() const
{
  return this->_d->_mbtSpecModelFilePath;
}

const QIcon& MbtSpecModel::icon() const
{
  return this->_d->_mbtSpecModelIcon;
}

void MbtSpecModel::setEnabled(bool enabled)
{
  this->_d->_enabled=enabled;

  return;
}

bool MbtSpecModel::isEnabled() const
{
  return this->_d->_enabled;
}

const QString& MbtSpecModel::tcgInputFilePath() const
{
  return this->_d->_tcgInputFilePath;
}

void MbtSpecModel::setTcgInputFilePath(const QString& tcgInputFilePath)
{
  this->_d->_tcgInputFilePath = tcgInputFilePath;

  return;
}

void MbtSpecModel::clearTcgCoverageProperties()
{
  qDeleteAll(this->_d->_tcgCoverageProperties);
  this->_d->_tcgCoverageProperties.clear();

  return;
}

bool MbtSpecModel::addTcgCoverageProperty(TcgCoverageProperty* tcgCoverageProperty,
                                          QString* errorMessage)
{
  Q_ASSERT(tcgCoverageProperty);
  Q_ASSERT(tcgCoverageProperty->mbtSpecModel()==this);

  if(this->hasTcgCoverageProperty(tcgCoverageProperty->tcgCoverage()))
  {
    utils::AddPtrString(errorMessage)<<tr("there is already a tcg coverage '%1' "
                                          "for spec model '%2'!")
                                       .arg(tcgCoverageProperty->tcgCoverage())
                                       .arg(this->name());
    return false;
  }

  this->_d->_tcgCoverageProperties.append(tcgCoverageProperty);

  return true;
}

TcgCoverageProperty* MbtSpecModel::addTcgCoverageProperty(const QString& tcgCoverage)
{
  if(tcgCoverage.trimmed().isEmpty())
    return nullptr;

  TcgCoverageProperty* existTcgCoverProp = this->tcgCoverageProperty(tcgCoverage);
  if(existTcgCoverProp)
    return existTcgCoverProp;

  TcgCoverageProperty* newTcgCoverProp = new TcgCoverageProperty(tcgCoverage, this);
  this->_d->_tcgCoverageProperties.append(newTcgCoverProp);

  return newTcgCoverProp;
}

bool MbtSpecModel::removeTcgCoverageProperty(const QString& tcgCoverage)
{
  TcgCoverageProperty* existTcgCoverProp = this->tcgCoverageProperty(tcgCoverage);
  if(!existTcgCoverProp)
    return false;

  this->_d->_tcgCoverageProperties.removeAll(existTcgCoverProp);
  delete existTcgCoverProp;

  return true;
}

TcgCoverageProperty* MbtSpecModel::tcgCoverageProperty(const QString& tcgCoverage) const
{
  for(TcgCoverageProperty* tcgCoverageProperty
      : this->_d->_tcgCoverageProperties)
  {
    if(tcgCoverageProperty->tcgCoverage()
       .compare(tcgCoverage,Qt::CaseInsensitive) == 0)
      return tcgCoverageProperty;
  }

  return 0;
}

const QList<TcgCoverageProperty*>& MbtSpecModel::tcgCoverageProperties() const
{
  return this->_d->_tcgCoverageProperties;
}

bool MbtSpecModel::hasTcgCoverageProperty(const QString& tcgCoverage)
{
  if(this->tcgCoverageProperty(tcgCoverage))
    return true;

  return false;
}

void MbtSpecModel::clearErrors()
{
  this->_d->_errors.clear();

  return;
}

void MbtSpecModel::addDslError(const dslparser::DslError& dslError)
{
  MbtProject::addDslError(dslError, this->_d->_mbtSpecModelFilePath, &this->_d->_errors);

  return;
}

} // namespace mbtproject
