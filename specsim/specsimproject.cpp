#include "specsimproject.hpp"

#include "specsimconstants.hpp"
#include "specsimprojectmodel.hpp"
#include "specsimprojectconfiguration.hpp"
#include "specsimrunconfiguration.hpp"
#include "combuildspecsimprojectmodel.hpp"
#include "specsimmodel.hpp"
#include "specsimmodelsettings.hpp"

#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/target.h>

#include <utils/qtcassert.h>

#include <QDebug>

namespace specsim {

///////////////////////////SpecSimProject/////////////////////////////////////////
class SpecSimProject::Private
{
  friend class SpecSimProject;

  SpecSimProjectModel*                  _specSimProjectModel;
  ProjectExplorer::FolderNode*          _declFolder;
  ProjectExplorer::FolderNode*          _specSimFolder;
  SpecSimProjectConfiguration*          _specSimProjectConfiguration;

  Private()
    : _specSimProjectModel(nullptr),
      _declFolder(nullptr),
      _specSimFolder(nullptr),
      _specSimProjectConfiguration(nullptr)
  {}
};

SpecSimProject::SpecSimProject(ProjectExplorer::BaseProjectManager* projectManager,
                               const QString& fileName)
  : ProjectExplorer::BaseProject(projectManager,
                                 fileName,
                                 QLatin1String(Constants::SPECSIM_PROJECT_MIME_TYPE),
                                 QLatin1String(Constants::SPECSIM_PROJECT)),
    _d(new Private)
{
  this->_d->_specSimProjectModel = new SpecSimProjectModel(this);
  this->_d->_specSimProjectConfiguration = new SpecSimProjectConfiguration(this);

  ///project folders//////////////////////
  using namespace ProjectExplorer;
  this->_d->_declFolder=this->addFolderNode(QLatin1String("declarations"),
                                            QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                            true);
  Q_ASSERT(this->_d->_declFolder);
  this->_d->_declFolder->setProperty(Constants::SPECSIM_PROJECTNODE_TYPE,
                                     QLatin1String(Constants::SPECSIM_PROJECTNODE_DECLARATIONS));
  this->addArrayFilesKey(QLatin1String(Constants::SPECSIM_PROJECT_DECLFILES_KEY), this->_d->_declFolder);

  this->_d->_specSimFolder=this->addFolderNode(QLatin1String("simulation models"),
                                               QIcon(QLatin1String(":/specsim/images/specsimmodel.png")),
                                               true);
  Q_ASSERT(this->_d->_specSimFolder);
  this->_d->_specSimFolder->setProperty(Constants::SPECSIM_PROJECTNODE_TYPE,
                                        QLatin1String(Constants::SPECSIM_PROJECTNODE_SPECSIMMODELS));
  this->addArrayFilesKey(QLatin1String(Constants::SPECSIM_PROJECT_SPECSIMMODELFILES_KEY),
                         this->_d->_specSimFolder);

  connect(this, &SpecSimProject::projectFileRemoved,
          this, &SpecSimProject::specSimModelFileRemoved);
}

SpecSimProject::~SpecSimProject()
{
  delete this->_d;
}

SpecSimProjectConfiguration const* SpecSimProject::specSimProjectConfiguration() const
{
  return this->_d->_specSimProjectConfiguration;
}

SpecSimProjectConfiguration* SpecSimProject::specSimProjectConfiguration()
{
  return this->_d->_specSimProjectConfiguration;
}

bool SpecSimProject::supportsKit(ProjectExplorer::Kit *kit,
                                 QString *errorMessage) const
{
//  qDebug()<<"SpecSimProject::supportsKit() executed!";

  if(!kit)
    return false;

  if(kit->id()==Core::Id(Constants::SPECSIM_KIT))
    return true;

  QTC_ASSERT(errorMessage, return false);

  *errorMessage+=tr("kit '%1' is not supported!").arg(kit->displayName());

  return false;
}

bool SpecSimProject::needsConfiguration() const
{
  return false;
}

bool SpecSimProject::supportsNoTargetPanel() const
{
  return false;
}

QStringList SpecSimProject::files(FilesMode fileMode) const
{
  return BaseProject::files(fileMode);
}

bool SpecSimProject::reload(QString* errorString)
{
//  qDebug()<<"SpecSimProject::reload() ist executed!";

  this->resetErrors();

  if(!ProjectExplorer::BaseProject::reload(errorString))
  {
    ProjectExplorer::BaseError error;
    if(errorString)
      error.setDescription(*errorString);
    else
      error.setDescription(tr("there was an error while reloading the spec sim project!"));
    error.setUrl(QUrl(this->rootProjectNode()->projectDocument()->fileName()));

    this->addError(error);

    return false;
  }

  ComBuildSpecSimProjectModel buildCommand(this);
  bool result = buildCommand.execute(errorString);
  if(!result)
  {
    this->_d->_specSimProjectModel->reset();
    return false;
  }

  //read out or build project configuration
  for(SpecSimModel* specSimModel
      : this->_d->_specSimProjectModel->specSimModels())
  {
    SpecSimModelSettings* specSimModelSettings = this->_d->_specSimProjectConfiguration
                                                 ->specSimModelSettings(specSimModel->specSimModelFilePath());
    if(specSimModelSettings)
    {
      specSimModel->setEnabled(specSimModelSettings->isEnabled());
      this->_d->_specSimProjectModel->updateSpecSimModel(specSimModel);
    }
    else
    {
      SpecSimModelSettings newSpecSimModelSettings(specSimModel->specSimModelFilePath());
      this->_d->_specSimProjectConfiguration->addSpecSimModelSettings(newSpecSimModelSettings);
    }
  }

  //remove unneeded project configuration//
  for(const SpecSimModelSettings& specSimModelSettings
      : this->_d->_specSimProjectConfiguration->allSpecSimModelSettings())
  {
    SpecSimModel* specSimModel = this->_d->_specSimProjectModel->specSimModel(specSimModelSettings.specSimModelFilePath());
    if(!specSimModel)
      this->_d->_specSimProjectConfiguration->removeSpecSimModelSettings(specSimModelSettings);
  }

  return result;
}

SpecSimProjectModel const* SpecSimProject::specSimProjectModel() const
{
  return this->_d->_specSimProjectModel;
}

SpecSimProjectModel* SpecSimProject::specSimProjectModel()
{
  return this->_d->_specSimProjectModel;
}

const QStringList& SpecSimProject::declFiles() const
{
  return this->rootProjectNode()->projectDocument()->projectArrayFiles(QLatin1String(Constants::SPECSIM_PROJECT_DECLFILES_KEY));
}

const QStringList& SpecSimProject::specSimModelFiles() const
{
  return this->rootProjectNode()->projectDocument()->projectArrayFiles(QLatin1String(Constants::SPECSIM_PROJECT_SPECSIMMODELFILES_KEY));
}

quint16 SpecSimProject::specSimModelCount() const
{
  return this->rootProjectNode()->projectDocument()->projectArrayFilesCount(QLatin1String(Constants::SPECSIM_PROJECT_SPECSIMMODELFILES_KEY));
}

bool SpecSimProject::addSpecSimModelFile(const QString &specSimModelFileName,
                                         QString *errorMessage)
{
  if(!QFileInfo(specSimModelFileName).exists())
  {
    if(errorMessage)
      *errorMessage+=tr("simulation model file '%1' does not exist, "
                        "simulation model file will not be added!").arg(specSimModelFileName);
    return false;
  }

  QString relSpecModelFilePath=QDir::cleanPath(this->rootProjectNode()->projectDocument()->fileDir().relativeFilePath(specSimModelFileName));
  //  qDebug()<<"relative spec model file path:"<<relSpecModelFilePath;

  bool result = this->rootProjectNode()->projectDocument()->addProjectArrayFile(QLatin1String(Constants::SPECSIM_PROJECT_SPECSIMMODELFILES_KEY),
                                                                                relSpecModelFilePath,
                                                                                errorMessage);
  if(!result)
    return false;

  if(!this->reload(errorMessage))
    return false;

  return true;
}

void SpecSimProject::addDslError(const dslparser::DslError& newDslError,
                                 const QString& errorFileName,
                                 QList<ProjectExplorer::BaseError>* baseErrorList)
{
  return ProjectExplorer::addDslError(newDslError, errorFileName, baseErrorList);
}

void SpecSimProject::specSimModelFileRemoved(const QString& specSimModelFileName)
{
  qDebug()<<"SpecSimProject::specSimModelFileRemoved() with filename '"
         <<specSimModelFileName<<"' executed!";

  QFileInfo fileInfo(specSimModelFileName);
  QString layoutFilePath = fileInfo.filePath();
  layoutFilePath.replace(fileInfo.suffix(), "layout", Qt::CaseSensitive);

  QFileInfo layoutFileInfo(layoutFilePath);
  if(!layoutFileInfo.exists())
    return;

  if(!QFile(layoutFilePath).remove())
    qWarning()<<"layout file '"<<layoutFilePath<<"' could not be removed!";

  return;
}

bool SpecSimProject::fromMap(const QVariantMap& map)
{
//  qDebug()<<"SpecSimProject::fromMap() executed!";

  if (!BaseProject::fromMap(map))
    return false;

  if (map.contains(QLatin1String(Constants::SPECSIM_PROJECT_SETTINGS_KEY)))
  {
    QVariantMap value(map.value(QLatin1String(Constants::SPECSIM_PROJECT_SETTINGS_KEY)).toMap());
    this->_d->_specSimProjectConfiguration->fromMap(value);
  }

  //update project model and project configuration//
  for(const SpecSimModelSettings& specSimModelSettings
      : this->_d->_specSimProjectConfiguration->allSpecSimModelSettings())
  {
    SpecSimModel* specSimModel = this->_d->_specSimProjectModel
                                 ->specSimModel(specSimModelSettings.specSimModelFilePath());
    if(specSimModel)
    {
      specSimModel->setEnabled(specSimModelSettings.isEnabled());
      this->_d->_specSimProjectModel->updateSpecSimModel(specSimModel);
    }
    else
    {
      this->_d->_specSimProjectConfiguration->removeSpecSimModelSettings(specSimModelSettings);
    }
  }

  ProjectExplorer::KitManager* kitManager=ProjectExplorer::KitManager::instance();
  Q_ASSERT(kitManager);

  ProjectExplorer::Kit* specSimKit=0;
  foreach(ProjectExplorer::Kit* kit, kitManager->kits())
  {
    if(kit->id()==Core::Id(Constants::SPECSIM_KIT))
    {
      specSimKit=kit;
      break;
    }
  }

  ProjectExplorer::Target* activeTarget=this->activeTarget();
  if (!activeTarget)
  {
    if(!specSimKit)
      return false;

    ProjectExplorer::Target* specSimTarget=this->createTarget(specSimKit);
    Q_ASSERT(specSimTarget);
    specSimTarget->setDisplayName(QLatin1String("Model Simulation Target"));
    this->addTarget(specSimTarget);
    this->setActiveTarget(specSimTarget);
  }

  // Sanity check: We need both a buildconfiguration and a runconfiguration!
  QList<ProjectExplorer::Target *> targetList = this->targets();
  foreach (ProjectExplorer::Target *t, targetList)
  {
    if (!t->activeBuildConfiguration())
    {
      this->removeTarget(t);
      continue;
    }

    if (!t->activeRunConfiguration())
    {
      SpecSimRunConfiguration *rc = new SpecSimRunConfiguration(t);
      t->addRunConfiguration(rc);
      t->setActiveRunConfiguration(rc);
    }
  }

  return true;
}

QVariantMap SpecSimProject::toMap() const
{
//  qDebug()<<"SpecSimProject::toMap() executed!";

  QVariantMap map = BaseProject::toMap();
  map.insert(QLatin1String(Constants::SPECSIM_PROJECT_SETTINGS_KEY),
             this->_d->_specSimProjectConfiguration->toMap());

  return map;
}

/////////////////////SpecSimProjectManager//////////////////////
SpecSimProjectManager::SpecSimProjectManager()
  : ProjectExplorer::BaseProjectManager(QLatin1String(Constants::SPECSIM_PROJECT_MIME_TYPE))
{}

SpecSimProjectManager::~SpecSimProjectManager()
{}

SpecSimProject*
SpecSimProjectManager::createProject(const QString &fileName)
{
  return new SpecSimProject(this, fileName);
}

} // namespace mbtproject
