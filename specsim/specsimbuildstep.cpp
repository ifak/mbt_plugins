#include "specsimbuildstep.hpp"

#include "specsimconstants.hpp"
//#include "mbtprojectmodel.hpp"
//#include "mbtspecmodel.hpp"
//#include "mbtruntime.hpp"

#include <mobata/utils/functors.hpp>

//#include <coreplugin/messagemanager.h>
//#include <coreplugin/editormanager/editormanager.h>

#include <projectexplorer/buildsteplist.h>
//#include <projectexplorer/target.h>
#include <projectexplorer/task.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <QDir>

namespace specsim {

SpecSimBuildStep::SpecSimBuildStep(ProjectExplorer::BuildStepList* parentStepList)
  : ProjectExplorer::BaseBuildStep(parentStepList, Constants::SPECSIM_BUILD)
{}

SpecSimBuildStep::SpecSimBuildStep(ProjectExplorer::BuildStepList* parentStepList,
                                   ProjectExplorer::BuildStep* step)
  : ProjectExplorer::BaseBuildStep(parentStepList, step)
{}

SpecSimBuildStep::~SpecSimBuildStep()
{}

SpecSimProject* SpecSimBuildStep::baseProject()
{
  SpecSimProject* specSimProject = qobject_cast<SpecSimProject*>(this->project());
  Q_ASSERT(specSimProject);

  return specSimProject;
}

SpecSimBuildConfiguration* SpecSimBuildStep::baseBuildConfiguration()
{
  SpecSimBuildConfiguration* specSimBC = qobject_cast<SpecSimBuildConfiguration*>(this->buildConfiguration());
  Q_ASSERT(specSimBC);

  return specSimBC;
}

bool SpecSimBuildStep::init()
{
  return BaseBuildStep::init();
}

void SpecSimBuildStep::run(QFutureInterface<bool>& fi)
{
  qDebug()<<"SpecSimBuildStep::run() executed!";

//  Core::EditorManager* editorManager = Core::EditorManager::instance();
//  Q_ASSERT(editorManager);
//  for(Core::IEditor* openEditor : editorManager->openedEditors())
//  {
//    if(dynamic_cast<testcaseeditor::TestCaseGraphicEditor*>(openEditor))
//      editorManager->closeEditors(QList<Core::IEditor*>()<<openEditor);
//  }

  SpecSimProject* specSimProject = this->baseProject();
  Q_ASSERT(specSimProject);

  QString errorMessage;
  bool result=specSimProject->reload(&errorMessage);
  if(!result)
  {
    emit addOutput(errorMessage, BuildStep::ErrorMessageOutput);

    if(specSimProject->isError())
    {
      for(const ProjectExplorer::BaseError& error : specSimProject->errors())
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

  bool allSimModelResult = true;
//  MbtProjectModel const* mbtProjectModel = specSimProject->mbtProjectModel();
//  Q_ASSERT(mbtProjectModel);
//  for(MbtSpecModel* mbtSpecModel : mbtProjectModel->mbtSpecModels())
//  {
//    if(!mbtSpecModel->isEnabled())
//      continue;

//    QString errorString;
//    bool specModelResult = mbtSpecModel->rebuildSpecModel(&errorString);
//    if(!specModelResult)
//    {
//      allSpecModelResult = false;
//      emit addOutput(errorString, BuildStep::ErrorMessageOutput);

//      for(const ProjectExplorer::BaseError& error : mbtSpecModel->errors())
//      {
//        emit addTask(ProjectExplorer::Task(ProjectExplorer::Task::Error,
//                                           error.description(),
//                                           Utils::FileName::fromString(mbtSpecModel->mbtSpecModelFilePath()),
//                                           error.line(),
//                                           Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_COMPILE)));
//      }
//    }
//  }

//  //build mbt-runtime///
//  if(allSpecModelResult)
//  {
//    allSpecModelResult=this->buildMbtRuntime(&errorMessage);
//    if(!allSpecModelResult)//error reporting
//      emit addOutput(errorMessage, BuildStep::ErrorMessageOutput);
//  }

  fi.reportResult(allSimModelResult);
  emit finished();

  return;
}

bool SpecSimBuildStep::buildSpecSimRuntime(QString* errorMessage)
{
  qDebug()<<"SpecSimBuildStep::buildSpecSimRuntime() executed!";
  qDebug()<<"SpecSimBuildStep::buildDir: "<<this->baseBuildConfiguration()->buildDirectory();

  Q_ASSERT(this->baseProject());
  Q_ASSERT(this->baseBuildConfiguration());

  if(!QFileInfo(this->baseBuildConfiguration()->buildDirectory()).exists())
  {
    utils::AddPtrString(errorMessage)<<tr("build directory '%1' of project '%2' does not exist!")
                                       .arg(this->baseBuildConfiguration()->buildDirectory())
                                       .arg(this->baseProject()->displayName());
    return false;
  }

  SpecSimProject* specSimProject = this->baseProject();
  Q_ASSERT(specSimProject);

  SpecSimProjectModel const* specSimProjectModel = specSimProject->specSimProjectModel();
  Q_ASSERT(specSimProjectModel);

//  SpecSimRuntime* specSimRuntime = new SpecSimRuntime(specSimProject);
//  for(MbtSpecModel* mbtSpecModel : specSimProjectModel->mbtSpecModels())
//  {
//    if(!mbtSpecModel->isEnabled())
//      continue;

//    mbtRuntime->addMbtSpecModel(mbtSpecModel);
//  }
//  bool result = specSimRuntime->prepare(errorMessage);
//  if(!result)
//  {
//    delete specSimRuntime;
//    this->baseBuildConfiguration()->setSpecSimRuntime(nullptr);
//    return false;
//  }
//  else
//    this->baseBuildConfiguration()->setSpecSimRuntime(specSimRuntime);

  return true;
}

ProjectExplorer::BuildStepConfigWidget* SpecSimBuildStep::createConfigWidget()
{
  return nullptr;
}

bool SpecSimBuildStep::runInGuiThread() const
{
  return true;
}

//
// SpecSimBuildStepFactory
//
SpecSimBuildStepFactory::SpecSimBuildStepFactory(QObject* parent)
  : ProjectExplorer::BaseBuildStepFactory(Constants::SPECSIM_PROJECT,
                                          Constants::SPECSIM_BUILD,
                                          parent)
{}

SpecSimBuildStepFactory::~SpecSimBuildStepFactory()
{}

ProjectExplorer::BuildStep* SpecSimBuildStepFactory::create(ProjectExplorer::BuildStepList* parentStepList,
                                                        const Core::Id id)
{
  if (!this->canCreate(parentStepList, id))
    return nullptr;

  SpecSimBuildStep* step = new SpecSimBuildStep(parentStepList);

  return step;
}

ProjectExplorer::BuildStep* SpecSimBuildStepFactory::restore(ProjectExplorer::BuildStepList* parentStepList,
                                                         const QVariantMap& map)
{
  if (!this->canRestore(parentStepList, map))
    return nullptr;

  SpecSimBuildStep* specSimBuildStep(new SpecSimBuildStep(parentStepList));
  if (specSimBuildStep->fromMap(map))
    return specSimBuildStep;

  delete specSimBuildStep;

  return nullptr;
}

} // namespace mbtproject

