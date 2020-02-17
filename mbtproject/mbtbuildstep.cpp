#include "mbtbuildstep.hpp"

#include "mbtbuildconfiguration.hpp"
#include "mbtproject.hpp"
#include "mbtprojectconstants.hpp"
#include "mbtprojectmodel.hpp"
#include "mbtspecmodel.hpp"
#include "mbtruntime.hpp"

#include <mobata/utils/functors.hpp>

#include <coreplugin/messagemanager.h>
#include <coreplugin/editormanager/editormanager.h>

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/target.h>
#include <projectexplorer/task.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <testcaseeditor/testcasegraphiceditor.hpp>

#include <QDir>

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

MbtBuildStep::MbtBuildStep(ProjectExplorer::BuildStepList* parentStepList)
  : ProjectExplorer::BaseBuildStep(parentStepList, Constants::MBT_BUILD)
{}

MbtBuildStep::MbtBuildStep(ProjectExplorer::BuildStepList* parentStepList,
                           ProjectExplorer::BuildStep* step)
  : ProjectExplorer::BaseBuildStep(parentStepList, step)
{}

MbtBuildStep::~MbtBuildStep()
{}

MbtProject* MbtBuildStep::baseProject()
{
  MbtProject* mbtProject = qobject_cast<MbtProject*>(this->project());
  Q_ASSERT(mbtProject);

  return mbtProject;
}

MbtBuildConfiguration* MbtBuildStep::baseBuildConfiguration()
{
  MbtBuildConfiguration* mbtBC = qobject_cast<MbtBuildConfiguration*>(this->buildConfiguration());
  Q_ASSERT(mbtBC);

  return mbtBC;
}

bool MbtBuildStep::init()
{
  return BaseBuildStep::init();
}

void MbtBuildStep::run(QFutureInterface<bool>& fi)
{
//  qDebug()<<"MbtBuildStep::run() executed!";

  Core::EditorManager* editorManager = Core::EditorManager::instance();
  Q_ASSERT(editorManager);
  for(Core::IEditor* openEditor : editorManager->openedEditors())
  {
    if(dynamic_cast<testcaseeditor::TestCaseGraphicEditor*>(openEditor))
      editorManager->closeEditors(QList<Core::IEditor*>()<<openEditor);
  }

  MbtProject* mbtProject = this->baseProject();
  Q_ASSERT(mbtProject);

  QString errorMessage;
  bool result=mbtProject->reload(&errorMessage);
  if(!result)
  {
    emit addOutput(errorMessage, BuildStep::ErrorMessageOutput);

    if(mbtProject->isError())
    {
      for(const ProjectExplorer::BaseError& error : mbtProject->errors())
      {
        emit addTask(ProjectExplorer::Task(ProjectExplorer::Task::Error,
                                           error.description(),
                                           Utils::FileName::fromString(error.url().path()),
                                           error.line(),
                                           Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_COMPILE)));
      }
    }

    fi.reportResult(false);
    emit finished();
    return;
  }

  bool allSpecModelResult = true;
  MbtProjectModel const* mbtProjectModel = mbtProject->mbtProjectModel();
  Q_ASSERT(mbtProjectModel);
  for(MbtSpecModel* mbtSpecModel : mbtProjectModel->mbtSpecModels())
  {
    if(!mbtSpecModel->isEnabled())
      continue;

    QString errorString;
    bool specModelResult = mbtSpecModel->rebuildSpecModel(&errorString);
    if(!specModelResult)
    {
      allSpecModelResult = false;
      emit addOutput(errorString, BuildStep::ErrorMessageOutput);

      for(const ProjectExplorer::BaseError& error : mbtSpecModel->errors())
      {
        emit addTask(ProjectExplorer::Task(ProjectExplorer::Task::Error,
                                           error.description(),
                                           Utils::FileName::fromString(mbtSpecModel->mbtSpecModelFilePath()),
                                           error.line(),
                                           Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_COMPILE)));
      }
    }
  }

  //build mbt-runtime///
  if(allSpecModelResult)
  {
    allSpecModelResult=this->buildMbtRuntime(&errorMessage);
    if(!allSpecModelResult)//error reporting
      emit addOutput(errorMessage, BuildStep::ErrorMessageOutput);
  }

  fi.reportResult(allSpecModelResult);
  emit finished();

  return;
}

bool MbtBuildStep::buildMbtRuntime(QString* errorMessage)
{
//  qDebug()<<"MbtBuildStep::buildMbtRuntime() executed!";
//  qDebug()<<"MbtBuildStep::buildDir: "<<this->baseBuildConfiguration()->buildDirectory();

  Q_ASSERT(this->baseProject());
  Q_ASSERT(this->baseBuildConfiguration());

  if(!QFileInfo(this->baseBuildConfiguration()->buildDirectory()).exists())
  {
    utils::AddPtrString(errorMessage)<<tr("build directory '%1' of project '%2' does not exist!")
                                       .arg(this->baseBuildConfiguration()->buildDirectory())
                                       .arg(this->baseProject()->displayName());
    return false;
  }

  MbtProjectModel const* mbtProjectModel = this->baseProject()->mbtProjectModel();
  Q_ASSERT(mbtProjectModel);

  MbtRuntime* mbtRuntime = new MbtRuntime(this->baseProject());
  for(MbtSpecModel* mbtSpecModel : mbtProjectModel->mbtSpecModels())
  {
    if(!mbtSpecModel->isEnabled())
      continue;

    mbtRuntime->addMbtSpecModel(mbtSpecModel);
  }
  bool result = mbtRuntime->prepare(errorMessage);
  if(!result)
  {
    delete mbtRuntime;
    this->baseBuildConfiguration()->setMbtRuntime(0);
    return false;
  }
  else
    this->baseBuildConfiguration()->setMbtRuntime(mbtRuntime);

  return true;
}

ProjectExplorer::BuildStepConfigWidget* MbtBuildStep::createConfigWidget()
{
  return new ProjectExplorer::SimpleBuildStepConfigWidget(this);
}

bool MbtBuildStep::runInGuiThread() const
{
  return true;
}

//
// MbtBuildStepFactory
//
MbtBuildStepFactory::MbtBuildStepFactory(QObject* parent)
  : ProjectExplorer::BaseBuildStepFactory(Constants::MBT_PROJECT,
                                          Constants::MBT_BUILD,
                                          parent)
{}

MbtBuildStepFactory::~MbtBuildStepFactory()
{}

ProjectExplorer::BuildStep* MbtBuildStepFactory::create(ProjectExplorer::BuildStepList* parentStepList,
                                                       const Core::Id id)
{
  if (!this->canCreate(parentStepList, id))
    return 0;

  MbtBuildStep* step = new MbtBuildStep(parentStepList);

  return step;
}

ProjectExplorer::BuildStep* MbtBuildStepFactory::restore(ProjectExplorer::BuildStepList* parentStepList,
                                                        const QVariantMap& map)
{
  if (!this->canRestore(parentStepList, map))
    return 0;

  MbtBuildStep* mbc(new MbtBuildStep(parentStepList));
  if (mbc->fromMap(map))
    return mbc;

  delete mbc;

  return 0;
}

} // namespace mbtproject

