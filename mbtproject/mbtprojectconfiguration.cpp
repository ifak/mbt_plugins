#include "mbtprojectconfiguration.hpp"

#include "mbtproject.hpp"

#include <QDebug>

static const QLatin1String useGlobalKey("MbtProjectConfiguration.UseGlobal");
static const QLatin1String mbtSpecModelSettingsKey("MbtProjectConfiguration.MbtSpecModelSettings");
static const QLatin1String mbtSpenatSettingsKey("MbtProjectConfiguration.MbtSpenatSettings");

namespace mbtproject {

class MbtProjectConfiguration::Private
{
  friend class MbtProjectConfiguration;

  MbtProject*                 _mbtProject;
  bool                        _useGlobal;
  QList<MbtSpecModelSettings> _mbtSpecModelSettings;

  Private(MbtProject* project)
    : _mbtProject(project),
      _useGlobal(true)
  {}
};

MbtProjectConfiguration::MbtProjectConfiguration(MbtProject* mbtProject)
  : QObject(mbtProject),
    _d(new Private(mbtProject))
{}

MbtProjectConfiguration::~MbtProjectConfiguration()
{
  delete this->_d;
}

void MbtProjectConfiguration::setUseGlobalSettings(bool use)
{
  this->_d->_useGlobal = use;
}

bool MbtProjectConfiguration::useGlobalSettings() const
{
  return this->_d->_useGlobal;
}

MbtSpecModelSettings* MbtProjectConfiguration::mbtSpecModelSettings(const QString& mbtSpecModelFilePath) const
{
  for(MbtSpecModelSettings& mbtSpecModelSettings :
      this->_d->_mbtSpecModelSettings)
  {
    if(mbtSpecModelSettings.mbtSpecModelFilePath() == mbtSpecModelFilePath)
      return &mbtSpecModelSettings;
  }

  return nullptr;
}

const QList<MbtSpecModelSettings>& MbtProjectConfiguration::allMbtSpecModelSettings() const
{
  return this->_d->_mbtSpecModelSettings;
}

void MbtProjectConfiguration::addMbtSpecModelSettings(const MbtSpecModelSettings& newMbtSpecModelSettings)
{
  if(MbtSpecModelSettings* existMbtSpecModelSettings = this->mbtSpecModelSettings(newMbtSpecModelSettings.mbtSpecModelFilePath()))
  {
    existMbtSpecModelSettings->setEnabled(newMbtSpecModelSettings.isEnabled());
    existMbtSpecModelSettings->setTcgCoverages(newMbtSpecModelSettings.tcgCoverages());
  }
  else
    this->_d->_mbtSpecModelSettings.append(newMbtSpecModelSettings);

  return;
}

void MbtProjectConfiguration::removeMbtSpecModelSettings(const MbtSpecModelSettings& mbtSpecModelSettings)
{
  this->_d->_mbtSpecModelSettings.removeAll(mbtSpecModelSettings);

  return;
}

QVariantMap MbtProjectConfiguration::toMap() const
{
  QVariantMap map;
  map.insert(useGlobalKey, this->_d->_useGlobal);

  QVariantList mbtSpecModelSettingsList;
  for(const MbtSpecModelSettings& mbtSpecModelSettings : this->_d->_mbtSpecModelSettings)
    mbtSpecModelSettingsList.append(mbtSpecModelSettings.toMap());

  map.insert(mbtSpecModelSettingsKey, mbtSpecModelSettingsList);

  return map;
}

void MbtProjectConfiguration::fromMap(const QVariantMap& map)
{
  this->_d->_useGlobal = map.value(useGlobalKey, this->_d->_useGlobal).toBool();
  QVariantList mbtSpecModelSettingsList = map.value(mbtSpecModelSettingsKey).toList();
  for(const QVariant& listValue : mbtSpecModelSettingsList)
  {
    MbtSpecModelSettings tempMbtSpecModelSettings;
    tempMbtSpecModelSettings.fromMap(listValue.toMap());

    this->addMbtSpecModelSettings(tempMbtSpecModelSettings);
  }

  return;
}

}// end namespace mbtproject

