#include "mbtpplugin.hpp"

#include "mbtpconstants.hpp"
#include "mbtpproject.hpp"
#include "mbtptoolchain.hpp"
#include "mbtpbuildconfiguration.hpp"
#include "mbtpbuildstep.hpp"
#include "mbtprunconfiguration.hpp"
#include "mbtpmode.hpp"
#include "mbtpprojectimportwizard.hpp"
#include "mbtpprojectwizard.hpp"
#include "mbtpgraphicfactory.hpp"
#include "mbtpsettingspage.hpp"
#include "mbtpattributesfactory.hpp"

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

#include <coreplugin/generatedfile.h>

#include <coreplugin/editormanager/editormanager.h>

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/toolchainmanager.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/buildmanager.h>

#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QtPlugin>
#include <QApplication>

#include <QFileDialog>

#include <QtPlugin>
#include <QApplication>

#include <mobata/memory_leak_start.hpp>

inline void initMbtpResource() { Q_INIT_RESOURCE(mbtp); }

namespace mbtp{

class MbtpPlugin::Private
{
  friend class MbtpPlugin;

  bool                      _toolChainsRegistered;
  ProjectExplorer::Project* _contextProject;
  ProjectExplorer::Node*    _contextNode;

  Private()
    : _toolChainsRegistered(false),
      _contextProject(nullptr),
      _contextNode(nullptr)
  {}
};

MbtpPlugin::MbtpPlugin()
  : _d(new Private())
{
}

MbtpPlugin::~MbtpPlugin()
{
  delete this->_d;
}

bool MbtpPlugin::initialize(const QStringList &arguments, QString *errorString)
{
  Q_UNUSED(arguments);
  Q_UNUSED(errorString);

  initMbtpResource();

  Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
  if(!mdb->addMimeTypes(QLatin1String(":/mbtp/mbtp.mimetypes.xml"),
                        errorString))
    return false;

  /////////project manager/////////////////////////
  MbtpProjectManager* manager = new MbtpProjectManager();
  this->addAutoReleasedObject(manager);


  Core::FileIconProvider *iconProvider = Core::FileIconProvider::instance();
  iconProvider->registerIconOverlayForSuffix(QIcon(QLatin1String(":/mbtp/images/mbtpproject.png")),
                                             QLatin1String("mbtpproject"));


  Core::BaseFileWizardParameters projectParams1(Core::IWizard::ProjectWizard);
  projectParams1.setCategory(QLatin1String("P.mbtpproject"));
  projectParams1.setDisplayCategory(tr("Mbtp"));
  projectParams1.setDescription(tr("Create"));
  projectParams1.setDisplayName(tr("Create"));
  projectParams1.setId(QStringLiteral("Mbtp.project"));
  this->addAutoReleasedObject(new MbtpWizard(projectParams1));

  Core::BaseFileWizardParameters projectParams2(Core::IWizard::ProjectWizard);
  projectParams2.setCategory(QLatin1String("P.mbtpproject"));
  projectParams2.setDisplayCategory(tr("Mbtp"));
  projectParams2.setDescription(tr("Import"));
  projectParams2.setDisplayName(tr("Import"));
  projectParams2.setId(QStringLiteral("Mbtp.project"));
  this->addAutoReleasedObject(new MbtpProjectImportWizard("mbtp",".mbtp",projectParams2));

  this->addAutoReleasedObject(new MbtpSettingsPage);

  /////////////////build+run integration//////////////////////////////////////
  //create and register objects
  this->addAutoReleasedObject(new MbtpToolChainFactory);
  this->addAutoReleasedObject(new MbtpBuildStepFactory);
  this->addAutoReleasedObject(new MbtpBuildConfigurationFactory);
  this->addAutoReleasedObject(new MbtpRunControlFactory);

  this->addAutoReleasedObject(new MbtpGraphicFactory);
  this->addAutoReleasedObject(new MbtpAttributesFactory);

  return true;
}

void MbtpPlugin::extensionsInitialized()
{
  MbtpMode* mbtpMode = new MbtpMode();
  MbtpMainModeWidget* mbtpMainModeWidget = new MbtpMainModeWidget(mbtpMode);
  MbtpModeWidget* mbtpModeWidget = new MbtpModeWidget(mbtpMode, mbtpMainModeWidget);
  mbtpMode->setWidget(mbtpModeWidget);
  this->addAutoReleasedObject(mbtpMode);
  return;
}

bool MbtpPlugin::delayedInitialize()
{
  this->registerToolChain();

  return true;
}

ExtensionSystem::IPlugin::ShutdownFlag MbtpPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void MbtpPlugin::registerToolChain()
{
  if(this->_d->_toolChainsRegistered)
    return;

  ProjectExplorer::ToolChainManager* tcManager=ProjectExplorer::ToolChainManager::instance();
  Q_ASSERT(tcManager);

  ProjectExplorer::KitManager* kitManager=ProjectExplorer::KitManager::instance();
  Q_ASSERT(kitManager);

  //mbtpproject tool chain////////
  ProjectExplorer::ToolChain* mbtpProjectToolChain = nullptr;
  for(ProjectExplorer::ToolChain* tc : tcManager->toolChains())
  {
    if(tc->type()==QLatin1String(Constants::MBTP_PROJECT))
    {
      mbtpProjectToolChain=tc;
      break;
    }
  }

  if(!mbtpProjectToolChain)
  {
    mbtpProjectToolChain=new MbtpToolChain;
    bool result=tcManager->registerToolChain(mbtpProjectToolChain);
    Q_ASSERT(result);
  }
  Q_ASSERT(mbtpProjectToolChain);

  // mbtpproject kit///////////
  ProjectExplorer::Kit* mbtpProjectKit=nullptr;
  for(ProjectExplorer::Kit* kit : kitManager->kits())
  {
    if(kit->id()==Core::Id(Constants::MBTP_KIT))
    {
      mbtpProjectKit=kit;
      break;
    }
  }

  if(!mbtpProjectKit)
  {
    mbtpProjectKit=new ProjectExplorer::Kit(Core::Id(Constants::MBTP_KIT));
    mbtpProjectKit->setAutoDetected(true);
    mbtpProjectKit->setDisplayName(QLatin1String("Test Prioritization Kit"));
    ProjectExplorer::ToolChainKitInformation::setToolChain(mbtpProjectKit,
                                                           mbtpProjectToolChain);
    mbtpProjectKit->setup();

    bool result=kitManager->registerKit(mbtpProjectKit);
    Q_ASSERT(result);
  }
  Q_ASSERT(mbtpProjectKit);

  this->_d->_toolChainsRegistered=true;

  return;
}

void MbtpPlugin::showContextMenu(ProjectExplorer::Project* project,
                                   ProjectExplorer::Node* node)
{
  this->_d->_contextProject=project;
  this->_d->_contextNode=node;

  return;
}

}///end namespace mbtp

Q_EXPORT_PLUGIN(mbtp::MbtpPlugin)

