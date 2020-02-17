#include "mbtspecmodelsettings.hpp"

static const QLatin1String mbtSpecModelFilePathKey("MbtSpecModelFilePath");
static const QLatin1String mbtSpecModelEnabledKey("MbtSpecModelEnabled");
static const QLatin1String mbtSpecModelTcgCoveragesKey("MbtSpecModelTcgCoverages");

namespace mbtproject {

MbtSpecModelSettings::MbtSpecModelSettings(const QString& mbtSpecModelFilePath)
  : _mbtSpecModelFilePath(mbtSpecModelFilePath),
    _enabled(false)
{}

MbtSpecModelSettings::MbtSpecModelSettings(const MbtSpecModelSettings& source)
  : _mbtSpecModelFilePath(source._mbtSpecModelFilePath),
    _enabled(source._enabled),
    _tcgCoverages(source._tcgCoverages)
{}

const QString& MbtSpecModelSettings::mbtSpecModelFilePath() const
{
  return this->_mbtSpecModelFilePath;
}

void MbtSpecModelSettings::setMbtSpecModelFilePath(const QString &mbtSpecModelFilePath)
{
  this->_mbtSpecModelFilePath = mbtSpecModelFilePath;

  return;
}

bool MbtSpecModelSettings::isEnabled() const
{
  return this->_enabled;
}

void MbtSpecModelSettings::setEnabled(bool enabled)
{
  this->_enabled = enabled;

  return;
}

void MbtSpecModelSettings::setTcgCoverages(const QStringList& tcgCoverages)
{
  this->_tcgCoverages = tcgCoverages;
}

const QStringList& MbtSpecModelSettings::tcgCoverages() const
{
  return this->_tcgCoverages;
}

bool MbtSpecModelSettings::addTcgCoverage(const QString& tcgCoverage)
{
  if(this->_tcgCoverages.contains(tcgCoverage, Qt::CaseInsensitive))
    return false;

  this->_tcgCoverages.append(tcgCoverage);

  return true;
}

bool MbtSpecModelSettings::removeTcgCoverage(const QString& tcgCoverage)
{
  if(!this->_tcgCoverages.contains(tcgCoverage, Qt::CaseInsensitive))
    return false;

  QStringList allTcgCoverages = this->_tcgCoverages.filter(tcgCoverage,
                                                           Qt::CaseInsensitive);
  for(const QString& removableTcgCover : allTcgCoverages)
    this->_tcgCoverages.removeAll(removableTcgCover);

  return true;
}

QVariantMap MbtSpecModelSettings::toMap() const
{
  QVariantMap map;

  map.insert(mbtSpecModelFilePathKey, this->_mbtSpecModelFilePath);
  map.insert(mbtSpecModelEnabledKey, this->_enabled);
  map.insert(mbtSpecModelTcgCoveragesKey, this->_tcgCoverages);

  return map;
}

void MbtSpecModelSettings::fromMap(const QVariantMap& map)
{
  this->_mbtSpecModelFilePath=map.value(mbtSpecModelFilePathKey).toString();
  this->_enabled = map.value(mbtSpecModelEnabledKey, false).toBool();
  this->_tcgCoverages = map.value(mbtSpecModelTcgCoveragesKey).toStringList();

  return;
}

} // namespace mbtproject
