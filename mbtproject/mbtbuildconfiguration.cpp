#include "mbtbuildconfiguration.hpp"

#include "mbtproject.hpp"
#include "mbtprojectconstants.hpp"
#include "mbtbuildstep.hpp"
#include "mbtruntime.hpp"

#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>

#include <utils/qtcassert.h>

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

class MbtBuildConfiguration::Private
{
  friend class MbtBuildConfiguration;

  MbtRuntime* _mbtRuntime;

  Private()
    : _mbtRuntime(0)
  {}
};

MbtBuildConfiguration::MbtBuildConfiguration(ProjectExplorer::Target* parent)
  : ProjectExplorer::BaseBuildConfiguration(parent,
                                            Core::Id(Constants::MBT_BUILD),
                                            QStringLiteral("MBT_Build")),
    _d(new Private)
{}

MbtBuildConfiguration::MbtBuildConfiguration(ProjectExplorer::Target* parent,
                                             const Core::Id& buildId)
  : ProjectExplorer::BaseBuildConfiguration(parent,
                                            buildId,
                                            QStringLiteral("MBT_Build")),
    _d(new Private)
{}

MbtBuildConfiguration::~MbtBuildConfiguration()
{
  delete this->_d->_mbtRuntime;
  delete this->_d;
}

void MbtBuildConfiguration::setMbtRuntime(MbtRuntime* mbtRuntime)
{
  delete this->_d->_mbtRuntime;
  this->_d->_mbtRuntime = mbtRuntime;

  return;
}

MbtRuntime* MbtBuildConfiguration::mbtRuntime()
{
  return this->_d->_mbtRuntime;
}

/*!
  \class MbtBuildConfigurationFactory
*/

MbtBuildConfigurationFactory::MbtBuildConfigurationFactory(QObject *parent)
  : ProjectExplorer::BaseBuildConfigurationFactory(Core::Id(Constants::MBT_PROJECT),
                                                   Core::Id(Constants::MBT_BUILD),
                                                   parent)
{}

MbtBuildConfigurationFactory::~MbtBuildConfigurationFactory()
{}

MbtBuildConfiguration* MbtBuildConfigurationFactory::create(ProjectExplorer::Target* target,
                                                            const Core::Id id,
                                                            const QString& name)
{
  if(!this->canCreate(target, id))
    return 0;

  MbtBuildConfiguration* newMbtProjectBC = new MbtBuildConfiguration(target,id);
  newMbtProjectBC->setDisplayName(name);

  ProjectExplorer::BuildStepList* buildSteps = 0;
  buildSteps=newMbtProjectBC->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
  Q_ASSERT(buildSteps);

  // ### Build Steps Build ###
  MbtBuildStep* modelBuildStep = new MbtBuildStep(buildSteps);
  buildSteps->insertStep(1, modelBuildStep);

  return newMbtProjectBC;
}

MbtBuildConfiguration* MbtBuildConfigurationFactory::restore(ProjectExplorer::Target* target,
                                                             const QVariantMap& map)
{
  if (!this->canRestore(target, map))
    return 0;

  MbtBuildConfiguration* bc = new MbtBuildConfiguration(target);
  if (bc->fromMap(map))
    return bc;

  delete bc;
  return 0;
}

} // namespace mbtproject
