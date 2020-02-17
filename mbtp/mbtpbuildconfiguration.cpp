#include "mbtpbuildconfiguration.hpp"

#include "mbtpconstants.hpp"
#include "mbtpbuildstep.hpp"

#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>

#include <utils/qtcassert.h>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class MbtpBuildConfiguration::Private
{
  friend class MbtpBuildConfiguration;

  MbtpRuntime* _mbtpRuntime;

  Private()
    : _mbtpRuntime(nullptr)
  {}
};

MbtpBuildConfiguration::MbtpBuildConfiguration(ProjectExplorer::Target* parent)
  : ProjectExplorer::BaseBuildConfiguration(parent,
                                                 Core::Id(Constants::MBTP_BUILD),
                                                 QStringLiteral("Mbtp_Build")),
    _d(new Private)
{}

MbtpBuildConfiguration::MbtpBuildConfiguration(ProjectExplorer::Target* parent,
                                                   const Core::Id id)
  : ProjectExplorer::BaseBuildConfiguration(parent,
                                              id,
                                              QStringLiteral("Mbtp_Build")),
    _d(new Private)
{}

MbtpBuildConfiguration::~MbtpBuildConfiguration()
{
  delete this->_d;
}

void MbtpBuildConfiguration::setMbtpRuntime(MbtpRuntime* mbtpRuntime)
{
  this->_d->_mbtpRuntime = mbtpRuntime;

  return;
}

MbtpRuntime* MbtpBuildConfiguration::mbtpRuntime()
{
  return this->_d->_mbtpRuntime;
}

/*!
  \class MbtpBuildConfigurationFactory
*/

MbtpBuildConfigurationFactory::MbtpBuildConfigurationFactory(QObject *parent)
  : ProjectExplorer::BaseBuildConfigurationFactory(Core::Id(Constants::MBTP_PROJECT),
                                                     Core::Id(Constants::MBTP_BUILD),
                                                     parent)
{}

MbtpBuildConfigurationFactory::~MbtpBuildConfigurationFactory()
{}

MbtpBuildConfiguration* MbtpBuildConfigurationFactory::create(ProjectExplorer::Target* target,
                                                                  const Core::Id id,
                                                                  const QString& name)
{
  if(!this->canCreate(target, id))
    return nullptr;

  MbtpBuildConfiguration* newMbtpBC = new MbtpBuildConfiguration(target,id);
  newMbtpBC->setDisplayName(name);

  ProjectExplorer::BuildStepList* buildSteps = nullptr;
  buildSteps=newMbtpBC->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
  Q_ASSERT(buildSteps);

  // ### Build Steps Build ###
  MbtpBuildStep* mbtpBuildStep=new MbtpBuildStep(buildSteps);
  buildSteps->insertStep(0, mbtpBuildStep);

  return newMbtpBC;
}

MbtpBuildConfiguration* MbtpBuildConfigurationFactory::restore(ProjectExplorer::Target* target,
                                                                   const QVariantMap& map)
{
  if (!this->canRestore(target, map))
      return nullptr;

  MbtpBuildConfiguration *bc = new MbtpBuildConfiguration(target);
  if (bc->fromMap(map))
      return bc;

  delete bc;
  return nullptr;
}

} // namespace mbtp

