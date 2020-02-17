#include "specsimmodelsettings.hpp"

static const QLatin1String specSimModelFilePathKey("SpecSimModelFilePath");
static const QLatin1String specSimModelEnabledKey("SpecSimModelEnabled");

namespace specsim {

SpecSimModelSettings::SpecSimModelSettings(const QString& specSimModelFilePath)
  : _specSimModelFilePath(specSimModelFilePath),
    _enabled(false)
{}

SpecSimModelSettings::SpecSimModelSettings(const SpecSimModelSettings& source)
  : _specSimModelFilePath(source._specSimModelFilePath),
    _enabled(source._enabled)
{}

const QString& SpecSimModelSettings::specSimModelFilePath() const
{
  return this->_specSimModelFilePath;
}

void SpecSimModelSettings::setSpecSimModelFilePath(const QString& specSimModelFilePath)
{
  this->_specSimModelFilePath = specSimModelFilePath;

  return;
}

bool SpecSimModelSettings::isEnabled() const
{
  return this->_enabled;
}

void SpecSimModelSettings::setEnabled(bool enabled)
{
  this->_enabled = enabled;

  return;
}

QVariantMap SpecSimModelSettings::toMap() const
{
  QVariantMap map;

  map.insert(specSimModelFilePathKey, this->_specSimModelFilePath);
  map.insert(specSimModelEnabledKey, this->_enabled);

  return map;
}

void SpecSimModelSettings::fromMap(const QVariantMap& map)
{
  this->_specSimModelFilePath=map.value(specSimModelFilePathKey).toString();
  this->_enabled = map.value(specSimModelEnabledKey, false).toBool();

  return;
}

} // namespace specsim
