#include "mbtpbuildstep.hpp"

#include "mbtpconstants.hpp"
#include "mbtpproject.hpp"
#include "mbtpbuildconfiguration.hpp"
#include "runtime/mbtpruntime.hpp"
#include "runtime/combuildmbtptestruntime.hpp"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/task.h>

#include <coreplugin/messagemanager.h>
#include <coreplugin/editormanager/editormanager.h>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

MbtpBuildStep::MbtpBuildStep(ProjectExplorer::BuildStepList* parentStepList)
  : ProjectExplorer::BaseBuildStep(parentStepList, Constants::MBTP_BUILD)
{
  qDebug()<<"HERE1";
}

MbtpBuildStep::MbtpBuildStep(ProjectExplorer::BuildStepList* parentStepList,
                             ProjectExplorer::BuildStep* step)
  : ProjectExplorer::BaseBuildStep(parentStepList, step)
{
  qDebug()<<"HERE2";
}

MbtpBuildStep::~MbtpBuildStep()
{}

MbtpProject* MbtpBuildStep::baseProject()
{
  MbtpProject* mbtpProject = qobject_cast<MbtpProject*>(this->project());
  Q_ASSERT(mbtpProject);

  return mbtpProject;
}

MbtpBuildConfiguration* MbtpBuildStep::mbtpBuildConfiguration()
{
  MbtpBuildConfiguration* mbtpBc = qobject_cast<MbtpBuildConfiguration*>(this->buildConfiguration());
  Q_ASSERT(mbtpBc);

  return mbtpBc;
}

bool MbtpBuildStep::init()
{
  qDebug()<<"MbtpStep::init()";

  return true;
}

void MbtpBuildStep::run(QFutureInterface<bool> &fi)
{
  qDebug()<<"MbtpStep::run()";

//  mbtp::MbtpRuntime* runtime = new mbtp::MbtpRuntime(this->baseProject(),this->parent());
//  runtime->start();
  QString errorString;
  bool result = this->buildMbtpTestRuntime(&errorString);

  fi.reportResult(result);
  emit finished();
    return;
}

bool MbtpBuildStep::buildMbtpTestRuntime(QString* errorMessage)
{
  Q_UNUSED(errorMessage);
  qDebug()<<"MbtpStep::build()";
 MbtpRuntime* mbtpRuntime = new MbtpRuntime(this->baseProject());
 this->mbtpBuildConfiguration()->setMbtpRuntime(mbtpRuntime);
  return true;
}

ProjectExplorer::BuildStepConfigWidget *MbtpBuildStep::createConfigWidget()
{
  return new ProjectExplorer::SimpleBuildStepConfigWidget(this);
}

bool MbtpBuildStep::runInGuiThread() const
{
  return true;
}


//
// MbtpStepFactory
//

MbtpBuildStepFactory::MbtpBuildStepFactory(QObject* parent)
  : ProjectExplorer::BaseBuildStepFactory(Constants::MBTP_PROJECT,
                                          Constants::MBTP_BUILD,
                                          parent)
{}

MbtpBuildStepFactory::~MbtpBuildStepFactory()
{}

ProjectExplorer::BuildStep *MbtpBuildStepFactory::create(ProjectExplorer::BuildStepList* parentStepList,
                                                         const Core::Id id)
{
  if (!canCreate(parentStepList, id))
    return nullptr;

  MbtpBuildStep *step = new MbtpBuildStep(parentStepList);

  return step;
}

ProjectExplorer::BuildStep *MbtpBuildStepFactory::restore(ProjectExplorer::BuildStepList* parentStepList,
                                                          const QVariantMap &map)
{
  if (!canRestore(parentStepList, map))
    return nullptr;

  MbtpBuildStep* as(new MbtpBuildStep(parentStepList));
  if (as->fromMap(map))
    return as;

  delete as;

  return nullptr;
}

} // namespace mbtp

