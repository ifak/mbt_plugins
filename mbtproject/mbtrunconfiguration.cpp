#include "mbtrunconfiguration.hpp"

#include "mbtprojectconstants.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

///MbtRunConfiguration//////////

class MbtRunConfiguration::Private
{
  friend class MbtRunConfiguration;

  bool _enabled;

  Private()
    : _enabled(true)
  {}
};

MbtRunConfiguration::MbtRunConfiguration(ProjectExplorer::Target* parent)
  : ProjectExplorer::RunConfiguration(parent, Constants::MBT_RUN),
    _d(new Private)
{
  Q_ASSERT(parent);

  this->setDisplayName(QLatin1String("Generate Test Cases"));
}

MbtRunConfiguration::~MbtRunConfiguration()
{
  delete this->_d;
}

QWidget* MbtRunConfiguration::createConfigurationWidget()
{
//  qDebug()<<"MbtRunConfiguration::createConfigWidget() executed()";

  return 0;
}

QString MbtRunConfiguration::title() const
{
  return this->displayName();
}

void MbtRunConfiguration::setEnabled(const bool enabled)
{
  if(this->_d->_enabled == enabled)
    return;

  this->_d->_enabled=enabled;
  emit enabledChanged();

  return;
}

bool MbtRunConfiguration::isEnabled() const
{
  return this->_d->_enabled;
}

} // namespace mbtproject

