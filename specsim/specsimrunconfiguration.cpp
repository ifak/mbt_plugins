#include "specsimrunconfiguration.hpp"

#include "specsimconstants.hpp"

#include <QDebug>

namespace specsim {

///SpecSimRunConfiguration//////////

class SpecSimRunConfiguration::Private
{
  friend class SpecSimRunConfiguration;

  bool _enabled;

  Private()
    : _enabled(true)
  {}
};

SpecSimRunConfiguration::SpecSimRunConfiguration(ProjectExplorer::Target* parent)
  : ProjectExplorer::RunConfiguration(parent, Constants::SPECSIM_RUN),
    _d(new Private)
{
  Q_ASSERT(parent);

  this->setDisplayName(QLatin1String("Simulate and Validate Specification Models"));
}

SpecSimRunConfiguration::~SpecSimRunConfiguration()
{
  delete this->_d;
}

QWidget* SpecSimRunConfiguration::createConfigurationWidget()
{
  qDebug()<<"SpecSimRunConfiguration::createConfigWidget() executed()";

  return nullptr;
}

QString SpecSimRunConfiguration::title() const
{
  return this->displayName();
}

void SpecSimRunConfiguration::setEnabled(const bool enabled)
{
  if(this->_d->_enabled == enabled)
    return;

  this->_d->_enabled=enabled;
  emit enabledChanged();

  return;
}

bool SpecSimRunConfiguration::isEnabled() const
{
  return this->_d->_enabled;
}

} // namespace specsim

