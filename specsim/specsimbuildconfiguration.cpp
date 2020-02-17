#include "specsimbuildconfiguration.hpp"

#include "specsimconstants.hpp"
#include "specsimbuildstep.hpp"
#include "specsimruntime.hpp"

#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>

#include <utils/qtcassert.h>

#include <mobata/memory_leak_start.hpp>

namespace specsim {

class SpecSimBuildConfiguration::Private
{
  friend class SpecSimBuildConfiguration;

  SpecSimRuntime* _specSimRuntime;

  Private()
    : _specSimRuntime(nullptr)
  {}
};

SpecSimBuildConfiguration::SpecSimBuildConfiguration(ProjectExplorer::Target* parent)
  : ProjectExplorer::BaseBuildConfiguration(parent,
                                            Core::Id(Constants::SPECSIM_BUILD),
                                            QStringLiteral("Simulation Model Build")),
    _d(new Private)
{}

SpecSimBuildConfiguration::SpecSimBuildConfiguration(ProjectExplorer::Target* parent,
                                                     const Core::Id& buildId)
  : ProjectExplorer::BaseBuildConfiguration(parent,
                                            buildId,
                                            QStringLiteral("Simulation Model Build")),
    _d(new Private)
{}

SpecSimBuildConfiguration::~SpecSimBuildConfiguration()
{
  delete this->_d->_specSimRuntime;
  delete this->_d;
}

void SpecSimBuildConfiguration::setSpecSimRuntime(SpecSimRuntime* specSimRuntime)
{
  delete this->_d->_specSimRuntime;
  this->_d->_specSimRuntime = specSimRuntime;

  return;
}

SpecSimRuntime* SpecSimBuildConfiguration::specSimRuntime()
{
  return this->_d->_specSimRuntime;
}

/*!
  \class SpecSimBuildConfigurationFactory
*/

SpecSimBuildConfigurationFactory::SpecSimBuildConfigurationFactory(QObject* parent)
  : ProjectExplorer::BaseBuildConfigurationFactory(Core::Id(Constants::SPECSIM_PROJECT),
                                                   Core::Id(Constants::SPECSIM_BUILD),
                                                   parent)
{}

SpecSimBuildConfigurationFactory::~SpecSimBuildConfigurationFactory()
{}

SpecSimBuildConfiguration* SpecSimBuildConfigurationFactory::create(ProjectExplorer::Target* target,
                                                                    const Core::Id id,
                                                                    const QString& name)
{
  if(!this->canCreate(target, id))
    return 0;

  SpecSimBuildConfiguration* newSpecSimProjectBC = new SpecSimBuildConfiguration(target,id);
  newSpecSimProjectBC->setDisplayName(name);

  ProjectExplorer::BuildStepList* buildSteps = 0;
  buildSteps=newSpecSimProjectBC->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
  Q_ASSERT(buildSteps);

  // ### Build Steps Build ###
  SpecSimBuildStep* specSimBuildStep = new SpecSimBuildStep(buildSteps);
  buildSteps->insertStep(1, specSimBuildStep);

  return newSpecSimProjectBC;
}

SpecSimBuildConfiguration* SpecSimBuildConfigurationFactory::restore(ProjectExplorer::Target* target,
                                                                     const QVariantMap& map)
{
  if (!this->canRestore(target, map))
    return 0;

  SpecSimBuildConfiguration* bc = new SpecSimBuildConfiguration(target);
  if (bc->fromMap(map))
    return bc;

  delete bc;
  return 0;
}

} // namespace mbtproject
