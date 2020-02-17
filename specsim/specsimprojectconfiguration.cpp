#include "specsimprojectconfiguration.hpp"

#include "specsimproject.hpp"

#include <QDebug>

static const QLatin1String useGlobalKey("SpecSimProjectConfiguration.UseGlobal");
static const QLatin1String specSimModelSettingsKey("SpecSimProjectConfiguration.SpecSimModelSettings");

namespace specsim {

class SpecSimProjectConfiguration::Private
{
  friend class SpecSimProjectConfiguration;

  SpecSimProject*         _specSimProject;
  bool                    _useGlobal;
  SpecSimModelSettingsSet _specSimModelSettings;

  Private(SpecSimProject* project)
    : _specSimProject(project),
      _useGlobal(true)
  {}
};

SpecSimProjectConfiguration::SpecSimProjectConfiguration(SpecSimProject* specSimProject)
  : QObject(specSimProject),
    _d(new Private(specSimProject))
{}

SpecSimProjectConfiguration::~SpecSimProjectConfiguration()
{
  delete this->_d;
}

void SpecSimProjectConfiguration::setUseGlobalSettings(bool use)
{
  this->_d->_useGlobal = use;
}

bool SpecSimProjectConfiguration::useGlobalSettings() const
{
  return this->_d->_useGlobal;
}

SpecSimModelSettings* SpecSimProjectConfiguration::specSimModelSettings(const QString& specSimModelFilePath) const
{
  for(SpecSimModelSettings& specSimModelSettings :
      this->_d->_specSimModelSettings)
  {
    if(specSimModelSettings.specSimModelFilePath().compare(specSimModelFilePath, Qt::CaseInsensitive)==0)
      return &specSimModelSettings;
  }

  return nullptr;
}

const SpecSimProjectConfiguration::SpecSimModelSettingsSet&
SpecSimProjectConfiguration::allSpecSimModelSettings() const
{
  return this->_d->_specSimModelSettings;
}

void SpecSimProjectConfiguration::addSpecSimModelSettings(const SpecSimModelSettings& newSpecSimModelSettings)
{
  if(SpecSimModelSettings* existSpecSimModelSettings = this->specSimModelSettings(newSpecSimModelSettings.specSimModelFilePath()))
  {
    existSpecSimModelSettings->setEnabled(newSpecSimModelSettings.isEnabled());
  }
  else
    this->_d->_specSimModelSettings.append(newSpecSimModelSettings);

  return;
}

void SpecSimProjectConfiguration::removeSpecSimModelSettings(const SpecSimModelSettings& specSimModelSettings)
{
  this->_d->_specSimModelSettings.removeAll(specSimModelSettings);

  return;
}

QVariantMap SpecSimProjectConfiguration::toMap() const
{
  QVariantMap map;
  map.insert(useGlobalKey, this->_d->_useGlobal);

  QVariantList specSimModelSettingsList;
  for(const SpecSimModelSettings& specSimModelSettings : this->_d->_specSimModelSettings)
    specSimModelSettingsList.append(specSimModelSettings.toMap());

  map.insert(specSimModelSettingsKey, specSimModelSettingsList);

  return map;
}

void SpecSimProjectConfiguration::fromMap(const QVariantMap& map)
{
  this->_d->_useGlobal = map.value(useGlobalKey, this->_d->_useGlobal).toBool();
  QVariantList specSimModelSettingsList = map.value(specSimModelSettingsKey).toList();
  for(const QVariant& listValue : specSimModelSettingsList)
  {
    SpecSimModelSettings temSpecSimModelSettings;
    temSpecSimModelSettings.fromMap(listValue.toMap());

    this->addSpecSimModelSettings(temSpecSimModelSettings);
  }

  return;
}

}// end namespace specsim

