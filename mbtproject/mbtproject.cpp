#include "mbtproject.hpp"

#include "mbtprojectconstants.hpp"
#include "mbtprojectmodel.hpp"
#include "mbtprojectconfiguration.hpp"
#include "mbtspecmodel.hpp"
#include "tcgcoverageproperty.hpp"
#include "mbtrunconfiguration.hpp"
#include "combuildmbtprojectmodel.hpp"
#include "comexporttestsuite.hpp"

#include <mobata/model/ts/testcaseitem.hpp>
#include <mobatawidgets/ts/comrendertestcase.hpp>

#include <utils/qtcassert.h>

#include <coreplugin/icontext.h>
#include <coreplugin/messagemanager.h>
#include <coreplugin/documentmanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>

#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/session.h>
#include <projectexplorer/taskhub.h>
#include <projectexplorer/buildtargetinfo.h>
#include <projectexplorer/buildconfiguration.h>

#include <testcaseeditor/testcaseeditorconstants.hpp>
#include <testcaseeditor/testcasegraphiceditor.hpp>

#include <QDebug>

namespace mbtproject {

///////////////////////////MbtProject/////////////////////////////////////////
class MbtProject::Private
{
  friend class MbtProject;

  MbtProjectModel*                      _mbtProjectModel;
  ProjectExplorer::FolderNode*          _declFolder;
  ProjectExplorer::FolderNode*          _specFolder;
  MbtProjectConfiguration*              _mbtProjectConfiguration;
  QString                               _tcgAppFilePath;
  ExportTestSuiteMenu                   _exportTestSuiteMenu;
  QHash<ComExportTestSuite*, QAction*>  _exportTestSuiteCommands;

  Private()
    : _mbtProjectModel(0),
      _declFolder(0),
      _specFolder(0),
      _mbtProjectConfiguration(0),
      _exportTestSuiteMenu(QLatin1String("Export Test Suite as..."))
  {
    this->_exportTestSuiteMenu.setIcon(QIcon(QLatin1String(":/mobata/images/ts/testsuite.png")));
  }
};

MbtProject::MbtProject(ProjectExplorer::BaseProjectManager* projectManager,
                       const QString& fileName)
  : ProjectExplorer::BaseProject(projectManager,
                                 fileName,
                                 QLatin1String(Constants::MBT_PROJECT_MIME_TYPE),
                                 QLatin1String(Constants::MBT_PROJECT)),
    _d(new Private)
{
  this->_d->_mbtProjectModel = new MbtProjectModel(this);
  this->_d->_mbtProjectConfiguration = new MbtProjectConfiguration(this);

  ///project folders//////////////////////
  using namespace ProjectExplorer;
  this->_d->_declFolder=this->addFolderNode(QLatin1String("declarations"),
                                            QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                            true);
  Q_ASSERT(this->_d->_declFolder);
  this->_d->_declFolder->setProperty(Constants::MBT_PROJECTNODE_TYPE,
                                     QLatin1String(Constants::MBT_PROJECTNODE_DECLARATIONS));
  this->addArrayFilesKey(QLatin1String(Constants::MBT_PROJECT_DECLFILES_KEY), this->_d->_declFolder);

  this->_d->_specFolder=this->addFolderNode(QLatin1String("specification models"),
                                            QIcon(QLatin1String(":/mobata/images/ts/sut.png")),
                                            true);
  Q_ASSERT(this->_d->_specFolder);
  this->_d->_specFolder->setProperty(Constants::MBT_PROJECTNODE_TYPE,
                                     QLatin1String(Constants::MBT_PROJECTNODE_SPECMODELS));
  this->addArrayFilesKey(QLatin1String(Constants::MBT_PROJECT_SPECMODELFILES_KEY),
                         this->_d->_specFolder);

  QString applicationDirPath = QCoreApplication::applicationDirPath();
  if(!applicationDirPath.endsWith(QStringLiteral("/")))
    applicationDirPath += QStringLiteral("/");
  this->_d->_tcgAppFilePath = applicationDirPath + QStringLiteral("tcgapp");

  connect(this, &MbtProject::projectFileRemoved,
          this, &MbtProject::mbtSpecModelFileRemoved);
}

MbtProject::~MbtProject()
{
  delete this->_d;
}

MbtProjectConfiguration const* MbtProject::mbtProjectConfiguration() const
{
  return this->_d->_mbtProjectConfiguration;
}

MbtProjectConfiguration* MbtProject::mbtProjectConfiguration()
{
  return this->_d->_mbtProjectConfiguration;
}

bool MbtProject::supportsKit(ProjectExplorer::Kit *kit,
                             QString *errorMessage) const
{
//  qDebug()<<"MbtProject::supportsKit() executed!";

  if(!kit)
    return false;

  if(kit->id()==Core::Id(Constants::MBT_KIT))
    return true;

  QTC_ASSERT(errorMessage, return false);

  *errorMessage+=tr("kit '%1' is not supported!").arg(kit->displayName());

  return false;
}

bool MbtProject::needsConfiguration() const
{
  return false;
}

bool MbtProject::supportsNoTargetPanel() const
{
  return false;
}

QStringList MbtProject::files(FilesMode fileMode) const
{
  return BaseProject::files(fileMode);
}

bool MbtProject::reload(QString* errorString)
{
//  qDebug()<<"MbtProject::reload() ist executed!";

  this->resetErrors();

  if(!ProjectExplorer::BaseProject::reload(errorString))
  {
    ProjectExplorer::BaseError error;
    if(errorString)
      error.setDescription(*errorString);
    else
      error.setDescription(tr("there was an error while reloading the base project!"));
    error.setUrl(QUrl(this->rootProjectNode()->projectDocument()->fileName()));

    this->addError(error);

    return false;
  }

  ComBuildMbtProjectModel buildCommand(this);
  bool result = buildCommand.execute(errorString);

  //read out or build project configuration
  for(MbtSpecModel* mbtSpecModel
      : this->_d->_mbtProjectModel->mbtSpecModels())
  {
    MbtSpecModelSettings* mbtSpecModelSettings = this->_d->_mbtProjectConfiguration
                                                 ->mbtSpecModelSettings(mbtSpecModel->mbtSpecModelFilePath());
    if(mbtSpecModelSettings)
    {
      mbtSpecModel->setEnabled(mbtSpecModelSettings->isEnabled());
      mbtSpecModel->clearTcgCoverageProperties();
      for(const QString& tcgCoverage : mbtSpecModelSettings->tcgCoverages())
      {
        TcgCoverageProperty* tcgCoverageProperty = new TcgCoverageProperty(tcgCoverage,
                                                                           mbtSpecModel);
        QString errorString;
        bool result = mbtSpecModel->addTcgCoverageProperty(tcgCoverageProperty, &errorString);
        if(!result)
        {
          qCritical()<<"error while adding tcg coverage property: "<<errorString;
          return false;
        }
      }

      this->_d->_mbtProjectModel->updateMbtSpecModel(mbtSpecModel);
    }
    else
    {
      MbtSpecModelSettings newMbtSpecModelSettings(mbtSpecModel->mbtSpecModelFilePath());
      this->_d->_mbtProjectConfiguration->addMbtSpecModelSettings(newMbtSpecModelSettings);
    }
  }

  //remove unneeded project configuration//
  for(const MbtSpecModelSettings& specModelSettings
      : this->_d->_mbtProjectConfiguration->allMbtSpecModelSettings())
  {
    MbtSpecModel* mbtSpecModel = this->_d->_mbtProjectModel->mbtSpecModel(specModelSettings.mbtSpecModelFilePath());
    if(!mbtSpecModel)
      this->_d->_mbtProjectConfiguration->removeMbtSpecModelSettings(specModelSettings);
  }

  return result;
}

MbtProjectModel const* MbtProject::mbtProjectModel() const
{
  return this->_d->_mbtProjectModel;
}

MbtProjectModel* MbtProject::mbtProjectModel()
{
  return this->_d->_mbtProjectModel;
}

const QStringList& MbtProject::declFiles() const
{
  return this->rootProjectNode()->projectDocument()->projectArrayFiles(QLatin1String(Constants::MBT_PROJECT_DECLFILES_KEY));
}

const QStringList& MbtProject::specModelFiles() const
{
  return this->rootProjectNode()->projectDocument()->projectArrayFiles(QLatin1String(Constants::MBT_PROJECT_SPECMODELFILES_KEY));
}

quint16 MbtProject::specModelCount() const
{
  return this->rootProjectNode()->projectDocument()->projectArrayFilesCount(QLatin1String(Constants::MBT_PROJECT_SPECMODELFILES_KEY));
}

bool MbtProject::addSpecModelFile(const QString &specModelFileName,
                                  QString *errorMessage)
{
  if(!QFileInfo(specModelFileName).exists())
  {
    if(errorMessage)
      *errorMessage+=tr("spec model file '%1' does not exist, "
                        "spec model file will not be added!").arg(specModelFileName);
    return false;
  }

  QString relSpecModelFilePath=QDir::cleanPath(this->rootProjectNode()->projectDocument()->fileDir().relativeFilePath(specModelFileName));
  //  qDebug()<<"relative spec model file path:"<<relSpecModelFilePath;

  bool result = this->rootProjectNode()->projectDocument()->addProjectArrayFile(QLatin1String(Constants::MBT_PROJECT_SPECMODELFILES_KEY),
                                                                                relSpecModelFilePath,
                                                                                errorMessage);
  if(!result)
    return false;

  if(!this->reload(errorMessage))
    return false;

  return true;
}

ExportTestSuiteMenu* MbtProject::exportTestSuiteMenu() const
{
  return &this->_d->_exportTestSuiteMenu;
}

void MbtProject::addExportTestSuiteCommand(ComExportTestSuite* exportTestSuiteCommand)
{
  Q_ASSERT(exportTestSuiteCommand);

  if(this->_d->_exportTestSuiteCommands.find(exportTestSuiteCommand)
     !=this->_d->_exportTestSuiteCommands.end())
    return;

  QAction* exportTestSuiteCommandAction=new QAction(exportTestSuiteCommand->icon(),
                                                    exportTestSuiteCommand->title(),
                                                    this);
  connect(exportTestSuiteCommandAction, &QAction::triggered,
          this, &MbtProject::dispatchExportTestSuiteCommand);
  this->_d->_exportTestSuiteMenu.addAction(exportTestSuiteCommandAction);
  this->_d->_exportTestSuiteCommands[exportTestSuiteCommand]=exportTestSuiteCommandAction;

  return;
}

QList<ComExportTestSuite*> MbtProject::exportTestSuiteCommands() const
{
  return this->_d->_exportTestSuiteCommands.keys();
}

void MbtProject::dispatchExportTestSuiteCommand()
{
  QAction* senderAction = qobject_cast<QAction*>(this->sender());
  if(!senderAction)
    return;

  QHash<ComExportTestSuite*, QAction*>::iterator exp_it;
  for (exp_it = this->_d->_exportTestSuiteCommands.begin();
       exp_it != this->_d->_exportTestSuiteCommands.end();
       ++exp_it)
  {
    if(exp_it.value()==senderAction)
    {
      QString errorMessage;
      bool result = exp_it.key()->exportTestSuite(this->_d->_exportTestSuiteMenu.exportTestSuiteItem(),
                                                  this->_d->_exportTestSuiteMenu.mbtSpecModel(),
                                                  &errorMessage);
      if(!result)
      {
        Core::MessageManager::instance()->printToOutputPane(errorMessage,
                                                            Core::MessageManager::WithFocus);
        Core::MessageManager::instance()->showOutputPane();
      }
    }
  }

  return;
}

void MbtProject::mbtSpecModelFileRemoved(const QString& mbtSpecModelFileName)
{
  qDebug()<<"MbtProject::mbtSpecModelFileRemoved() with filename '"
         <<mbtSpecModelFileName<<"' executed!";

  QFileInfo fileInfo(mbtSpecModelFileName);
  QString layoutFilePath = fileInfo.filePath();
  layoutFilePath.replace(fileInfo.suffix(), "layout", Qt::CaseSensitive);

  QFileInfo layoutFileInfo(layoutFilePath);
  if(!layoutFileInfo.exists())
    return;

  if(!QFile(layoutFilePath).remove())
    qWarning()<<"layout file '"<<layoutFilePath<<"' could not be removed!";

  return;
}

void MbtProject::addDslError(const dslparser::DslError& newDslError,
                             const QString& errorFileName,
                             QList<ProjectExplorer::BaseError>* baseErrorList)
{
//  if(!baseErrorList)
//    return;

//  ProjectExplorer::BaseError error;

//  if(newDslError.errorType()==dslparser::DslError::SyntaxError)
//    error.setErrorType(ProjectExplorer::BaseError::SyntaxError);
//  else if(newDslError.errorType()==dslparser::DslError::AmbiguityError)
//    error.setErrorType(ProjectExplorer::BaseError::AmbiguityError);
//  else if(newDslError.errorType()==dslparser::DslError::ContextError)
//    error.setErrorType(ProjectExplorer::BaseError::ContextError);
//  else if(newDslError.errorType()==dslparser::DslError::SensitivityError)
//    error.setErrorType(ProjectExplorer::BaseError::SensitivityError);
//  else if(newDslError.errorType()==dslparser::DslError::FileError)
//    error.setErrorType(ProjectExplorer::BaseError::FileError);
//  else if(newDslError.errorType()==dslparser::DslError::UnknownError)
//    error.setErrorType(ProjectExplorer::BaseError::UnknownError);

//  error.setDescription(newDslError.message());
//  error.setUrl(QUrl(errorFileName));
//  error.setLine(newDslError.line());
//  error.setColumn(newDslError.charPositionInLine());

//  baseErrorList->append(error);

  return ProjectExplorer::addDslError(newDslError, errorFileName, baseErrorList);

  return;
}

const QString& MbtProject::tcgAppFilePath() const
{
  return this->_d->_tcgAppFilePath;
}

void MbtProject::setTcgAppFilePath(const QString& tcgAppFilePath)
{
  this->_d->_tcgAppFilePath = tcgAppFilePath;

  return;
}

void MbtProject::showTestCaseGraphic(model::ts::TestCaseItem const* testCaseItem,
                                     TcgCoverageProperty const* tcgCoverageProperty)
{
  using namespace testcaseeditor;

  Q_ASSERT(testCaseItem);
  Q_ASSERT(tcgCoverageProperty);

  Q_ASSERT(this->activeTarget());
  Q_ASSERT(this->activeTarget()->activeBuildConfiguration());

  QString tempTestCaseGraphicFilePath = this->activeTarget()
                                        ->activeBuildConfiguration()->buildDirectory();
  if (!tempTestCaseGraphicFilePath.endsWith(QLatin1Char('/')))
    tempTestCaseGraphicFilePath += QLatin1Char('/');

  tempTestCaseGraphicFilePath += tcgCoverageProperty->mbtSpecModel()->name()
                                 + QLatin1Char('/')
                                 + tcgCoverageProperty->tcgCoverage()
                                 +QLatin1Char('/');

  QDir tempTestCaseGraphicFileDir(tempTestCaseGraphicFilePath);
  if(!tempTestCaseGraphicFileDir.exists())
  {
    bool result = tempTestCaseGraphicFileDir.mkpath(tempTestCaseGraphicFilePath);
    if(!result)
    {
      QString errorMessage = tr("directory '%1' for test case graphic file could not be created!")
                             .arg(tempTestCaseGraphicFilePath);
      Core::MessageManager::instance()->printToOutputPane(errorMessage,
                                                          Core::MessageManager::WithFocus);
      Core::MessageManager::instance()->showOutputPane();

      return;
    }
  }

  tempTestCaseGraphicFilePath += testCaseItem->name()
                                 + QLatin1String(".testcasegraphic");

  QFile tempTestCaseGraphicFile(tempTestCaseGraphicFilePath);
  if(!tempTestCaseGraphicFile.open(QIODevice::ReadWrite))
  {
    QString errorMessage = tr("test case graphic file '%1' could not be opened!")
                           .arg(tempTestCaseGraphicFilePath);
    Core::MessageManager::instance()->printToOutputPane(errorMessage,
                                                        Core::MessageManager::WithFocus);
    Core::MessageManager::instance()->showOutputPane();

    return;
  }
  tempTestCaseGraphicFile.close();

  Core::IEditor* editor=Core::EditorManager::instance()
                        ->openEditor(tempTestCaseGraphicFile.fileName(),
                                     Core::Id(testcaseeditor::Constants::TESTCASEGRAPHIC_ID));
  TestCaseGraphicEditor* testCaseGraphicEditor=qobject_cast<TestCaseGraphicEditor*>(editor);
  if(!testCaseGraphicEditor)
    return;

  testCaseGraphicEditor->setTestCaseItem(testCaseItem);

  ///render testcase////
  TestCaseGraphicWidget* testCaseGraphicWidget=testCaseGraphicEditor->testCaseGraphicWidget();
  Q_ASSERT(testCaseGraphicWidget);
  testCaseGraphicWidget->updateTestCaseGraphic();

  return;
}

bool MbtProject::fromMap(const QVariantMap& map)
{
//  qDebug()<<"MbtProject::fromMap() executed!";

  if (!BaseProject::fromMap(map))
    return false;

  if (map.contains(QLatin1String(Constants::MBT_PROJECT_SETTINGS_KEY)))
  {
    QVariantMap value(map.value(QLatin1String(Constants::MBT_PROJECT_SETTINGS_KEY)).toMap());
    this->_d->_mbtProjectConfiguration->fromMap(value);
  }

  //update project model and project configuration//
  for(const MbtSpecModelSettings& specModelSettings
      : this->_d->_mbtProjectConfiguration->allMbtSpecModelSettings())
  {
    MbtSpecModel* mbtSpecModel = this->_d->_mbtProjectModel
                                 ->mbtSpecModel(specModelSettings.mbtSpecModelFilePath());
    if(mbtSpecModel)
    {
      mbtSpecModel->setEnabled(specModelSettings.isEnabled());
      mbtSpecModel->clearTcgCoverageProperties();
      for(const QString& tcgCoverage : specModelSettings.tcgCoverages())
      {
        TcgCoverageProperty* tcgCoverageProperty = new TcgCoverageProperty(tcgCoverage,
                                                                           mbtSpecModel);
        QString errorString;
        bool result = mbtSpecModel->addTcgCoverageProperty(tcgCoverageProperty, &errorString);
        if(!result)
        {
          qCritical()<<"error while adding tcg coverage property: "<<errorString;
          return false;
        }
      }
      this->_d->_mbtProjectModel->updateMbtSpecModel(mbtSpecModel);
    }
    else
    {
      this->_d->_mbtProjectConfiguration->removeMbtSpecModelSettings(specModelSettings);
    }
  }

  ProjectExplorer::KitManager* kitManager=ProjectExplorer::KitManager::instance();
  Q_ASSERT(kitManager);

  ProjectExplorer::Kit* mbtKit=0;
  foreach(ProjectExplorer::Kit* kit, kitManager->kits())
  {
    if(kit->id()==Core::Id(Constants::MBT_KIT))
    {
      mbtKit=kit;
      break;
    }
  }

  ProjectExplorer::Target* activeTarget=this->activeTarget();
  if (!activeTarget)
  {
    if(!mbtKit)
      return false;

    ProjectExplorer::Target* mbtTarget=this->createTarget(mbtKit);
    Q_ASSERT(mbtTarget);
    mbtTarget->setDisplayName(QLatin1String("MBT target"));
    this->addTarget(mbtTarget);
    this->setActiveTarget(mbtTarget);
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
      MbtRunConfiguration *rc = new MbtRunConfiguration(t);
      t->addRunConfiguration(rc);
      t->setActiveRunConfiguration(rc);
    }
  }

  return true;
}

QVariantMap MbtProject::toMap() const
{
//  qDebug()<<"MbtProject::toMap() executed!";

  QVariantMap map = BaseProject::toMap();
  map.insert(QLatin1String(Constants::MBT_PROJECT_SETTINGS_KEY),
             this->_d->_mbtProjectConfiguration->toMap());

  return map;
}

/////////////////////MbtProjectManager//////////////////////
MbtProjectManager::MbtProjectManager()
  : ProjectExplorer::BaseProjectManager(QLatin1String(Constants::MBT_PROJECT_MIME_TYPE))
{}

MbtProjectManager::~MbtProjectManager()
{}

MbtProject*
MbtProjectManager::createProject(const QString &fileName)
{
  return new MbtProject(this, fileName);
}

} // namespace mbtproject
