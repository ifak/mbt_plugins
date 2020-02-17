#include "mbtprojectplugin.hpp"

#include "mbtprojectconstants.hpp"
#include "mbtprojectwizard.hpp"
#include "mbtproject.hpp"
#include "mbtspecmodelwizard.hpp"
#include "mbtprojecttoolchain.hpp"
#include "mbtbuildconfiguration.hpp"
#include "mbtbuildstep.hpp"
#include "mbtruncontrol.hpp"
#include "mbtmode.hpp"

#include <app/app_version.h>

#include <utils/hostosinfo.h>

#include <extensionsystem/pluginmanager.h>

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/imode.h>
#include <coreplugin/dialogs/iwizard.h>

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/toolchainmanager.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/buildmanager.h>

#include <QMessageBox>
#include <QFileDialog>

#include <QtPlugin>
#include <QApplication>

inline void initMobataResource()      { Q_INIT_RESOURCE(mobata); }
inline void initMbtProjectResource()  { Q_INIT_RESOURCE(mbtproject); }

namespace mbtproject{

using namespace Internal;

static MbtProjectPlugin *m_instance = 0;

MbtProjectPlugin *MbtProjectPlugin::instance() { return m_instance; }

class MbtProjectPlugin::Private
{
  friend class MbtProjectPlugin;

  bool                      _toolChainsRegistered;
  QList<QAction*>           _hideActions;
  QMenu*                    _specModelsMenu;
  QAction*                  _addSpecModelAction;
  ProjectExplorer::Project* _contextProject;
  ProjectExplorer::Node*    _contextNode;

  Private()
    : _toolChainsRegistered(false),
      _specModelsMenu(nullptr),
      _addSpecModelAction(nullptr),
      _contextProject(nullptr),
      _contextNode(nullptr)
  {}
};

MbtProjectPlugin::MbtProjectPlugin()
  : _d(new Private)
{
  m_instance = this;
}

MbtProjectPlugin::~MbtProjectPlugin()
{
  // Unregister objects from the plugin manager's object pool
  // Delete members

  delete this->_d;
}

bool MbtProjectPlugin::initialize(const QStringList &arguments, QString *errorString)
{
  // Register objects in the plugin manager's object pool
  // Load settings
  // Add actions to menus
  // Connect to other plugins' signals
  // In the initialize method, a plugin can be sure that the plugins it
  // depends on have initialized their members.

  Q_UNUSED(arguments);
  Q_UNUSED(errorString);

  initMobataResource();
  initMbtProjectResource();

  Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
  if(!mdb->addMimeTypes(QLatin1String(":/mbtproject/mbtproject.mimetypes.xml"),
                        errorString))
    return false;

  this->addAutoReleasedObject(new MbtProjectManager());

  // add spec model menu + action/////////
  connect(ProjectExplorer::ProjectExplorerPlugin::instance(),
          &ProjectExplorer::ProjectExplorerPlugin::aboutToShowContextMenu,
          this, &MbtProjectPlugin::showContextMenu);

  Core::Context projectTreeContext(ProjectExplorer::Constants::C_PROJECT_TREE);
//  Core::Context projectTreeContext(Constants::MBT_PROJECTCONTEXT,
//                                   ProjectExplorer::Constants::C_PROJECT_TREE);

//  Core::ActionContainer*  folderContextMenu =
//      Core::ActionManager::createMenu(ProjectExplorer::Constants::M_FOLDERCONTEXT);
  Core::ActionContainer*  folderContextMenu =
      Core::ActionManager::createMenu(Constants::MBT_PROJECTCONTEXT);
  this->_d->_specModelsMenu=folderContextMenu->menu();
  Q_ASSERT(this->_d->_specModelsMenu);

  // new action
  this->_d->_addSpecModelAction = new QAction(tr("Add Specification Model..."), this);
  Core::Command* cmd = Core::ActionManager::registerAction(this->_d->_addSpecModelAction,
                                                           Constants::MBT_ADDSPECMODEL,
                                                           projectTreeContext);
  //  cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+M")));
  folderContextMenu->addAction(cmd);

  connect(this->_d->_addSpecModelAction, &QAction::triggered,
          this, &MbtProjectPlugin::addSpecModel);

  QAction* addExistingSpecModelAction = new QAction(tr("Add Existing Specification Model..."), this);
  cmd = Core::ActionManager::registerAction(addExistingSpecModelAction,
                                            Constants::MBT_ADDEXISTSPECMODEL,
                                            projectTreeContext);
  folderContextMenu->addAction(cmd);
  connect(addExistingSpecModelAction, &QAction::triggered,
          this, &MbtProjectPlugin::addExistingSpecModel);

  ///spec model file wizard////////////////
  Core::BaseFileWizardParameters specModelParams(Core::IWizard::FileWizard);
  specModelParams.setCategory(QLatin1String("P.mbtspec"));
  specModelParams.setDisplayCategory(tr("MbtSpec"));
  specModelParams.setDescription(tr("Creates a new Specification Model File."));
  specModelParams.setDisplayName(tr("Specification Model"));
  specModelParams.setId(QStringLiteral("Mbt.specmodel"));
  this->addAutoReleasedObject(new MbtSpecModelWizard(specModelParams));

  ///project wizard////////////////
  Core::BaseFileWizardParameters projectParams(Core::IWizard::ProjectWizard);
  projectParams.setCategory(QLatin1String("P.mbtproject"));
  projectParams.setDisplayCategory(tr("MBT Project"));
  projectParams.setDescription(tr("Creates a MBT Project."));
  projectParams.setDisplayName(tr("MBT Project"));
  projectParams.setId(QLatin1String(Constants::MBT_PROJECT));
  this->addAutoReleasedObject(new MbtProjectWizard(projectParams));

  Core::FileIconProvider *iconProvider = Core::FileIconProvider::instance();
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                             QLatin1String("decl"));
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mobata/images/base/model.png")),
                                             QLatin1String("statemachine"));
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mobata/images/spenat/spenat.png")),
                                             QLatin1String("spenat"));
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mbtproject/images/mbtproject.png")),
                                             QLatin1String("mbtproject"));

//  // add Mbt Project Settings Config-Widget in the Window 'Project'
//  this->addAutoReleasedObject(new MbtProjectSettingsPanelFactory());

  ///////////////build+run integration//////////////////////////////////////
  this->addAutoReleasedObject(new MbtProjectToolChainFactory);
  this->addAutoReleasedObject(new MbtBuildConfigurationFactory);
  this->addAutoReleasedObject(new MbtBuildStepFactory);
  this->addAutoReleasedObject(new MbtRunControlFactory);

  return true;
}

void MbtProjectPlugin::extensionsInitialized()
{
  // Retrieve objects from the plugin manager's object pool
  // In the extensionsInitialized method, a plugin can be sure that all
  // plugins that depend on it are completely initialized.

  Core::ActionManager* actionManager=Core::ActionManager::instance();
  Q_ASSERT(actionManager);

  /// hide "debug" menu
  Core::ActionContainer* actContainer =actionManager->actionContainer(ProjectExplorer
                                                                      ::Constants
                                                                      ::M_DEBUG);
  if(actContainer)
    this->hideAction(actContainer->menu()->menuAction());

  ///hide session context menu actions....
  Core::Command* sessionCmd=actionManager->command(ProjectExplorer::Constants::BUILDSESSION);
  if(sessionCmd)
    this->hideAction(sessionCmd->action());

  sessionCmd=actionManager->command(ProjectExplorer::Constants::DEPLOYSESSION);
  if(sessionCmd)
    this->hideAction(sessionCmd->action());

  sessionCmd=actionManager->command(ProjectExplorer::Constants::REBUILDSESSION);
  if(sessionCmd)
    this->hideAction(sessionCmd->action());

  sessionCmd=actionManager->command(ProjectExplorer::Constants::CLEANSESSION);
  if(sessionCmd)
    this->hideAction(sessionCmd->action());

  MbtMode* mbtMode = new MbtMode();
  MbtMainModeWidget* mbtMainModeWidget = new MbtMainModeWidget(mbtMode);
  MbtModeWidget* mbtModeWidget = new MbtModeWidget(mbtMode, mbtMainModeWidget);
  mbtMode->setWidget(mbtModeWidget);
  this->addAutoReleasedObject(mbtMode);

  return;
}

bool MbtProjectPlugin::delayedInitialize()
{
  this->registerToolChain();

  return true;
}

ExtensionSystem::IPlugin::ShutdownFlag MbtProjectPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void MbtProjectPlugin::showContextMenu(ProjectExplorer::Project* project,
                                       ProjectExplorer::Node* node)
{
  this->_d->_contextProject=project;
  this->_d->_contextNode=node;

  if(this->_d->_contextNode->property(Constants::MBT_PROJECTNODE_TYPE)
     == QLatin1String(Constants::MBT_PROJECTNODE_SPECMODELS))
    this->_d->_specModelsMenu->popup(QCursor::pos());

  return;
}

void MbtProjectPlugin::addSpecModel()
{
  typedef  QList<MbtSpecModelWizard*> SpecModelWizardList;

  SpecModelWizardList specModelWizards = ExtensionSystem::PluginManager::getObjects<MbtSpecModelWizard>();
  Q_ASSERT(specModelWizards.size()==1);

  MbtSpecModelWizard* specModelWizard=specModelWizards.first();
  specModelWizard->setContextNode(this->_d->_contextNode);
  specModelWizard->setContextProject(this->_d->_contextProject);

  QString defaultPath=QDir::homePath();
  if(this->_d->_contextNode)
  {
    defaultPath=this->_d->_contextNode->path();
    defaultPath= QFileInfo(defaultPath).absolutePath();
  }

  Core::ICore::showNewItemDialog(tr("Add new specification model..."),
                                 QList<Core::IWizard*>()<<specModelWizard,
                                 defaultPath);

  this->_d->_contextNode=0;
  this->_d->_contextProject=0;

  return;
}

void MbtProjectPlugin::addExistingSpecModel()
{
  if(!this->_d->_contextNode)
    return;
  if(!this->_d->_contextProject)
    return;

  MbtProject* contextMbtProject = qobject_cast<MbtProject*>(this->_d->_contextProject);
  if(!contextMbtProject)
    return;

  QString defaultPath=QDir::homePath();
  if(this->_d->_contextNode)
  {
    defaultPath=this->_d->_contextNode->path();
    defaultPath= QFileInfo(defaultPath).absolutePath();
  }

  QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                  tr("Open Existing Specification Model"),
                                                  defaultPath,
                                                  tr("Specification Model Files (*.spenat *.statemachine)"));
  if(fileName.isEmpty())
    return;

  QString errorMessage;
  bool result = contextMbtProject->addSpecModelFile(fileName,&errorMessage);
  if(!result)
    QMessageBox::critical(nullptr, tr("error while adding existing spec model file"),errorMessage);

  this->_d->_contextNode=0;
  this->_d->_contextProject=0;

  return;
}

void MbtProjectPlugin::hideAction(QAction *action)
{
  if(action)
  {
    action->setVisible(false);
    this->_d->_hideActions.append(action);
  }

  return;
}

QMenu* MbtProjectPlugin::specModelsMenu()
{
  return this->_d->_specModelsMenu;
}

void MbtProjectPlugin::registerToolChain()
{
  if(this->_d->_toolChainsRegistered)
    return;

  ProjectExplorer::ToolChainManager* tcManager=ProjectExplorer::ToolChainManager::instance();
  Q_ASSERT(tcManager);

  ProjectExplorer::KitManager* kitManager=ProjectExplorer::KitManager::instance();
  Q_ASSERT(kitManager);

  //mbtproject tool chain////////
  ProjectExplorer::ToolChain* mbtProjectToolChain=0;
  for(ProjectExplorer::ToolChain* tc : tcManager->toolChains())
  {
    if(tc->type()==QLatin1String(Constants::MBT_PROJECT))
    {
      mbtProjectToolChain=tc;
      break;
    }
  }

  if(!mbtProjectToolChain)
  {
    mbtProjectToolChain=new MbtProjectToolChain;
    bool result=tcManager->registerToolChain(mbtProjectToolChain);
    Q_ASSERT(result);
  }
  Q_ASSERT(mbtProjectToolChain);

  // mbtproject kit///////////
  ProjectExplorer::Kit* mbtProjectKit=0;
  for(ProjectExplorer::Kit* kit : kitManager->kits())
  {
    if(kit->id()==Core::Id(Constants::MBT_KIT))
    {
      mbtProjectKit=kit;
      break;
    }
  }

  if(!mbtProjectKit)
  {
    mbtProjectKit=new ProjectExplorer::Kit(Core::Id(Constants::MBT_KIT));
    mbtProjectKit->setAutoDetected(true);
    mbtProjectKit->setDisplayName(QLatin1String("Test Generation Kit"));
    ProjectExplorer::ToolChainKitInformation::setToolChain(mbtProjectKit,
                                                           mbtProjectToolChain);
    mbtProjectKit->setup();

    bool result=kitManager->registerKit(mbtProjectKit);
    Q_ASSERT(result);
    kitManager->setDefaultKit(mbtProjectKit);
  }
  Q_ASSERT(mbtProjectKit);

  this->_d->_toolChainsRegistered=true;

  return;
}

}///end namespace mbtproject

Q_EXPORT_PLUGIN(mbtproject::MbtProjectPlugin)

