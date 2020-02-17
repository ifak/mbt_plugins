#include "specsimplugin.hpp"

#include "specsimmode.hpp"
#include "specsimconstants.hpp"
#include "specsimproject.hpp"
#include "specsimmodelwizard.hpp"
#include "specsimprojectwizard.hpp"
#include "specsimprojecttoolchain.hpp"
#include "specsimbuildconfiguration.hpp"
#include "specsimbuildstep.hpp"
#include "specsimruncontrol.hpp"

#include <extensionsystem/pluginmanager.h>

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>
#include <coreplugin/fileiconprovider.h>

#include "simsystemeditorfactory.hpp"
#include "simsystemcompletionassist.hpp"
#include "simsystemeditoroutline.hpp"
#include "simsystemeditorwidget.hpp"

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/toolchainmanager.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/projectnodes.h>

#include <texteditor/texteditorplugin.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorconstants.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QtPlugin>

inline void initMobataResource()    { Q_INIT_RESOURCE(mobata); }
inline void initSpecSimResource()   { Q_INIT_RESOURCE(specsim); }

using namespace specsim;
using namespace TextEditor;

static SpecSimPlugin *m_instance = 0;

SpecSimPlugin* SpecSimPlugin::instance() { return m_instance; }

class SpecSimPlugin::Private
{
  friend class SpecSimPlugin;

  bool                      _toolChainsRegistered;
  QMenu*                    _specSimModelsMenu;
  QAction*                  _addSimModelAction;
  ProjectExplorer::Project* _contextProject;
  ProjectExplorer::Node*    _contextNode;

  Private()
    : _toolChainsRegistered(false),
      _specSimModelsMenu(nullptr),
      _addSimModelAction(nullptr),
      _contextProject(nullptr),
      _contextNode(nullptr),
      _actionHandler(0)
  {}
private:
  TextEditorActionHandler*  _actionHandler;
};

SpecSimPlugin::SpecSimPlugin()
  : _d(new Private)
{
  m_instance = this;
}

SpecSimPlugin::~SpecSimPlugin()
{
  // Unregister objects from the plugin manager's object pool
  // Delete members

  delete this->_d;
}

bool SpecSimPlugin::initialize(const QStringList &arguments, QString *errorString)
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
  initSpecSimResource();

  Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
  if(!mdb->addMimeTypes(QLatin1String(":/specsim/specsim.mimetypes.xml"),
                        errorString))
  {
      if(errorString)
      {
          *errorString+=QLatin1String("mimetype for specsimeditor plugin "
                                      "could not be added!");
          qDebug()<<*errorString;
      }
      return false;
  }

  this->addAutoReleasedObject(new SimSystemEditorFactory(this));
  this->addAutoReleasedObject(new SimSystemCompletionAssistProvider);
  this->addAutoReleasedObject(new Internal::SimSystemEditorOutlineWidgetFactory());
  this->addAutoReleasedObject(new SpecSimProjectManager());

  this->_d->_actionHandler = new TextEditorActionHandler(Constants::SIMSYSTEMEDITOR_CONTEXT,
                                                         TextEditorActionHandler::Format
                                                         | TextEditorActionHandler::UnCommentSelection
                                                         | TextEditorActionHandler::UnCollapseAll);
  this->_d->_actionHandler->initializeActions();

  // add simulation model menu + action/////////
  connect(ProjectExplorer::ProjectExplorerPlugin::instance(),
          &ProjectExplorer::ProjectExplorerPlugin::aboutToShowContextMenu,
          this, &SpecSimPlugin::showContextMenu);

  Core::Context projectTreeContext(ProjectExplorer::Constants::C_PROJECT_TREE);
  Core::ActionContainer*  folderContextMenu =
      Core::ActionManager::createMenu(Constants::SPECSIM_PROJECTCONTEXT);
  this->_d->_specSimModelsMenu=folderContextMenu->menu();
  Q_ASSERT(this->_d->_specSimModelsMenu);

  // new action
  this->_d->_addSimModelAction = new QAction(tr("Add Simulation Model..."), this);
  Core::Command* cmd = Core::ActionManager::registerAction(this->_d->_addSimModelAction,
                                                           Constants::SPECSIM_ADDSIMMODEL,
                                                           projectTreeContext);
  folderContextMenu->addAction(cmd);

  connect(this->_d->_addSimModelAction, &QAction::triggered,
          this, &SpecSimPlugin::addSpecSimModel);

  QAction* addExistingSpecModelAction = new QAction(tr("Add Existing Simulation Model..."), this);
  cmd = Core::ActionManager::registerAction(addExistingSpecModelAction,
                                            Constants::SPECSIM_ADDEXISTSIMMODEL,
                                            projectTreeContext);
  folderContextMenu->addAction(cmd);
  connect(addExistingSpecModelAction, &QAction::triggered,
          this, &SpecSimPlugin::addExistingSpecSimModel);

  ///spec model file wizard////////////////
  Core::BaseFileWizardParameters simModelParams(Core::IWizard::FileWizard);
  simModelParams.setCategory(QLatin1String("P.specsim"));
  simModelParams.setDisplayCategory(tr("SpecSim"));
  simModelParams.setDescription(tr("Creates a new Simulation Model File."));
  simModelParams.setDisplayName(tr("Simulation Model"));
  simModelParams.setId(QStringLiteral("SpecSim.simmodel"));
  this->addAutoReleasedObject(new Internal::SpecSimModelWizard(simModelParams));

  ///project wizard////////////////
  Core::BaseFileWizardParameters projectParams(Core::IWizard::ProjectWizard);
  projectParams.setCategory(QLatin1String("P.specsimproject"));
  projectParams.setDisplayCategory(tr("Model Simulation Project"));
  projectParams.setDescription(tr("Creates a Model Simulation Project."));
  projectParams.setDisplayName(tr("Model Simulation Project"));
  projectParams.setId(QLatin1String(Constants::SPECSIM_PROJECT));
  this->addAutoReleasedObject(new SpecSimProjectWizard(projectParams));

  Core::FileIconProvider *iconProvider = Core::FileIconProvider::instance();
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                             QLatin1String("decl"));
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mobata/images/base/model.png")),
                                             QLatin1String("statemachine"));
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mobata/images/spenat/spenat.png")),
                                             QLatin1String("spenat"));
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/specsim/images/specsimproject.png")),
                                             QLatin1String("specsimproject"));

  ///////////////build+run integration//////////////////////////////////////
  this->addAutoReleasedObject(new SpecSimProjectToolChainFactory);
  this->addAutoReleasedObject(new SpecSimBuildConfigurationFactory);
  this->addAutoReleasedObject(new SpecSimBuildStepFactory);
  this->addAutoReleasedObject(new SpecSimRunControlFactory);

  return true;
}

void SpecSimPlugin::initializeEditor(SimSystemEditorWidget* editorWidget)
{
  QTC_CHECK(m_instance);
  this->_d->_actionHandler->setupActions(editorWidget);
  TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

void SpecSimPlugin::extensionsInitialized()
{
  // Retrieve objects from the plugin manager's object pool
  // In the extensionsInitialized method, a plugin can be sure that all
  // plugins that depend on it are completely initialized.

  SpecSimMode* specSimMode = new SpecSimMode();
  SpecSimMainModeWidget* specSimMainModeWidget = new SpecSimMainModeWidget(specSimMode);
  SpecSimModeWidget* specSimModeWidget = new SpecSimModeWidget(specSimMode, specSimMainModeWidget);
  specSimMode->setWidget(specSimModeWidget);
  this->addAutoReleasedObject(specSimMode);

  return;
}

bool SpecSimPlugin::delayedInitialize()
{
  this->registerToolChain();

  return true;
}

ExtensionSystem::IPlugin::ShutdownFlag SpecSimPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void SpecSimPlugin::showContextMenu(ProjectExplorer::Project* project,
                                    ProjectExplorer::Node* node)
{
  this->_d->_contextProject=project;
  this->_d->_contextNode=node;

  if(this->_d->_contextNode->property(Constants::SPECSIM_PROJECTNODE_TYPE)
     == QLatin1String(Constants::SPECSIM_PROJECTNODE_SPECSIMMODELS))
    this->_d->_specSimModelsMenu->popup(QCursor::pos());

  return;
}

void SpecSimPlugin::addSpecSimModel()
{
  using namespace Internal;

  typedef  QList<SpecSimModelWizard*> SpecSimModelWizardList;

  SpecSimModelWizardList specSimModelWizards = ExtensionSystem::PluginManager::getObjects<SpecSimModelWizard>();
  Q_ASSERT(specSimModelWizards.size()==1);

  SpecSimModelWizard* specSimModelWizard=specSimModelWizards.first();
  specSimModelWizard->setContextNode(this->_d->_contextNode);
  specSimModelWizard->setContextProject(this->_d->_contextProject);

  QString defaultPath=QDir::homePath();
  if(this->_d->_contextNode)
  {
    defaultPath=this->_d->_contextNode->path();
    defaultPath= QFileInfo(defaultPath).absolutePath();
  }

  Core::ICore::showNewItemDialog(tr("Add new simulation model..."),
                                 QList<Core::IWizard*>()<<specSimModelWizard,
                                 defaultPath);

  this->_d->_contextNode=0;
  this->_d->_contextProject=0;

  return;
}

void SpecSimPlugin::addExistingSpecSimModel()
{
  if(!this->_d->_contextNode)
    return;
  if(!this->_d->_contextProject)
    return;

  SpecSimProject* contextSpecSimProject = qobject_cast<SpecSimProject*>(this->_d->_contextProject);
  if(!contextSpecSimProject)
    return;

  QString defaultPath=QDir::homePath();
  if(this->_d->_contextNode)
  {
    defaultPath=this->_d->_contextNode->path();
    defaultPath= QFileInfo(defaultPath).absolutePath();
  }

  QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                  tr("Open Existing Simulation Model"),
                                                  defaultPath,
                                                  tr("Simulation Model Files (*.spenat *.statemachine)"));
  if(fileName.isEmpty())
    return;

  QString errorMessage;
  bool result = contextSpecSimProject->addSpecSimModelFile(fileName,&errorMessage);
  if(!result)
    QMessageBox::critical(nullptr, tr("error while adding existing simulation model file"),errorMessage);

  this->_d->_contextNode=0;
  this->_d->_contextProject=0;

  return;
}

void SpecSimPlugin::registerToolChain()
{
  if(this->_d->_toolChainsRegistered)
    return;

  ProjectExplorer::ToolChainManager* tcManager=ProjectExplorer::ToolChainManager::instance();
  Q_ASSERT(tcManager);

  ProjectExplorer::KitManager* kitManager=ProjectExplorer::KitManager::instance();
  Q_ASSERT(kitManager);

  //mbtproject tool chain////////
  ProjectExplorer::ToolChain* specSimProjectToolChain=0;
  for(ProjectExplorer::ToolChain* tc : tcManager->toolChains())
  {
    if(tc->type()==QLatin1String(Constants::SPECSIM_PROJECT))
    {
      specSimProjectToolChain=tc;
      break;
    }
  }

  if(!specSimProjectToolChain)
  {
    specSimProjectToolChain=new SpecSimProjectToolChain;
    bool result=tcManager->registerToolChain(specSimProjectToolChain);
    Q_ASSERT(result);
  }
  Q_ASSERT(specSimProjectToolChain);

  // mbtproject kit///////////
  ProjectExplorer::Kit* specSimProjectKit=0;
  for(ProjectExplorer::Kit* kit : kitManager->kits())
  {
    if(kit->id()==Core::Id(Constants::SPECSIM_KIT))
    {
      specSimProjectKit=kit;
      break;
    }
  }

  if(!specSimProjectKit)
  {
    specSimProjectKit=new ProjectExplorer::Kit(Core::Id(Constants::SPECSIM_KIT));
    specSimProjectKit->setAutoDetected(true);
    specSimProjectKit->setDisplayName(QLatin1String("Model Simulation Kit"));
    ProjectExplorer::ToolChainKitInformation::setToolChain(specSimProjectKit,
                                                           specSimProjectToolChain);
    specSimProjectKit->setup();



    bool result=kitManager->registerKit(specSimProjectKit);
    Q_ASSERT(result);
  }
  Q_ASSERT(specSimProjectKit);

  this->_d->_toolChainsRegistered=true;

  return;
}

Q_EXPORT_PLUGIN(SpecSimPlugin)

