#include "statemachineeditorplugin.hpp"

#include "statemachineeditorconstants.hpp"

#include "statemachineeditorfactory.hpp"
#include "statemachinegraphicfactory.hpp"
#include "statemachinehoverhandler.hpp"
#include "statemachinecompletionassist.hpp"
#include "statemachineeditoroutline.hpp"
#include "statemachineeditorwidget.hpp"
#include "statemachineeditorgraphvizsettingspage.hpp"
#include "statemachineeditorsimplestatesettingspage.hpp"
#include "statemachineeditortransitionsettingspage.hpp"
#include "statemachineeditorbackgroundsettingspage.hpp"
#include "statemachineeditorotherstatesettingspage.hpp"
#include "statemachineeditorhighlightsettingspage.hpp"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>

#include <texteditor/texteditorplugin.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorconstants.h>

#include <QAction>
#include <QMenu>
#include <QString>
#include <QDebug>

#include <QtPlugin>

#include <utils/qtcassert.h>

inline void initMobataResource() { Q_INIT_RESOURCE(mobata); }
inline void initGraphLayoutResource() { Q_INIT_RESOURCE(graphlayout);}

using namespace TextEditor;

namespace statemachineeditor{

StateMachineEditorPlugin* StateMachineEditorPlugin::_instance = 0;

class StateMachineEditorPlugin::Private
{
  friend class StateMachineEditorPlugin;

  Private()
    : _actionHandler(0)
  {
    this->_defaultStyleSheets=QLatin1String("");
  }

public:
  ~Private()
  { }

private:
  QString                   _defaultStyleSheets;
  TextEditorActionHandler*  _actionHandler;
};

StateMachineEditorPlugin::StateMachineEditorPlugin()
  : _d(new Private())
{
  _instance = this;
}

StateMachineEditorPlugin::~StateMachineEditorPlugin()
{
  // Unregister objects from the plugin manager's object pool
  // Delete members
  delete this->_d->_actionHandler;
  delete this->_d;
}

bool StateMachineEditorPlugin::initialize(const QStringList &arguments,
                                          QString *errorString)
{
  // Register objects in the plugin manager's object pool
  // Load settings
  // Add actions to menus
  // Connect to other plugins' signals
  // In the initialize method, a plugin can be sure that the plugins it
  // depends on have initialized their members.
  Q_UNUSED(arguments);

  initMobataResource();
  initGraphLayoutResource();

  Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
  if(!mdb->addMimeTypes(QLatin1String(":/statemachineeditor/statemachineeditor.mimetypes.xml"),
                        errorString))
  {
    if(errorString)
    {
      *errorString+=QLatin1String("mimetype for statemachineeditor plugin "
                                  "could not be added!");
      qDebug()<<*errorString;
    }
    return false;
  }

  this->addAutoReleasedObject(new StateMachineEditorFactory(this));
  StateMachineGraphicFactory* factory = new StateMachineGraphicFactory(this);
  this->addAutoReleasedObject(factory);
  this->addAutoReleasedObject(new StateMachineHoverHandler(this));
  this->addAutoReleasedObject(new StateMachineCompletionAssistProvider);
  this->addAutoReleasedObject(new Internal::StateMachineEditorOutlineWidgetFactory());

  StateMachineEditorGraphvizSettingsPage* page1 = new StateMachineEditorGraphvizSettingsPage();
  StateMachineEditorSimplestateSettingsPage* page2 = new StateMachineEditorSimplestateSettingsPage();
  StateMachineEditorOtherstateSettingsPage* page3 = new StateMachineEditorOtherstateSettingsPage();
  StateMachineEditorTransitionSettingsPage* page4 = new StateMachineEditorTransitionSettingsPage();
  StateMachineEditorBackgroundSettingsPage* page5 = new StateMachineEditorBackgroundSettingsPage();
  StateMachineEditorHighlightSettingsPage* page6 = new StateMachineEditorHighlightSettingsPage();

  QObject::connect(page1, &StateMachineEditorGraphvizSettingsPage::appliedSettings, factory, &StateMachineGraphicFactory::refreshGraphicIn);
  QObject::connect(page2, &StateMachineEditorSimplestateSettingsPage::appliedSettings, factory, &StateMachineGraphicFactory::refreshGraphicIn);
  QObject::connect(page3, &StateMachineEditorOtherstateSettingsPage::appliedSettings, factory, &StateMachineGraphicFactory::refreshGraphicIn);
  QObject::connect(page4, &StateMachineEditorTransitionSettingsPage::appliedSettings, factory, &StateMachineGraphicFactory::refreshGraphicIn);
  QObject::connect(page5, &StateMachineEditorBackgroundSettingsPage::appliedSettings, factory, &StateMachineGraphicFactory::refreshGraphicIn);
  QObject::connect(page6, &StateMachineEditorHighlightSettingsPage::appliedSettings, factory, &StateMachineGraphicFactory::refreshGraphicIn);

  this->addAutoReleasedObject(page1);
  this->addAutoReleasedObject(page2);
  this->addAutoReleasedObject(page3);
  this->addAutoReleasedObject(page4);
  this->addAutoReleasedObject(page5);
  this->addAutoReleasedObject(page6);

  this->_d->_actionHandler = new TextEditorActionHandler(Constants::STATEMACHINEEDITOR_CONTEXT,
                                                         TextEditorActionHandler::Format
                                                         | TextEditorActionHandler::UnCommentSelection
                                                         | TextEditorActionHandler::UnCollapseAll);
  this->_d->_actionHandler->initializeActions();

  return true;
}

void StateMachineEditorPlugin::extensionsInitialized()
{
  // Retrieve objects from the plugin manager's object pool
  // In the extensionsInitialized method, a plugin can be sure that all
  // plugins that depend on it are completely initialized.

  return;
}

ExtensionSystem::IPlugin::ShutdownFlag StateMachineEditorPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void StateMachineEditorPlugin::initializeEditor(StateMachineEditorWidget* editorWidget)
{
  QTC_CHECK(_instance);
  this->_d->_actionHandler->setupActions(editorWidget);
  TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

void StateMachineEditorPlugin::initializeGraphicEditor(StateMachineGraphicWidget* /*graphicWidget*/)
{
//  QTC_CHECK(_instance);
//  this->_d->_actionHandler->setupActions(editorWidget);
//  TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

const QString& StateMachineEditorPlugin::defaultStyleSheets() const
{
  return this->_d->_defaultStyleSheets;
}


}///end namespace statemachineeditor

//using namespace distesteditor;
//using namespace distesteditor::Internal;

Q_EXPORT_PLUGIN(distesteditor::StateMachineEditorPlugin)

