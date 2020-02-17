#include "spenateditorplugin.hpp"

#include "spenateditorconstants.hpp"

#include "spenateditorfactory.hpp"
#include "spenatgraphicfactory.hpp"
#include "spenathoverhandler.hpp"
#include "spenatcompletionassist.hpp"
#include "spenateditoroutline.hpp"
#include "spenateditorwidget.hpp"
#include "spenateditorgraphvizsettingspage.hpp"
#include "spenateditorplacesettingspage.hpp"
#include "spenateditortransitionsettingspage.hpp"
#include "spenateditorarcsettingspage.hpp"
#include "spenateditorbackgroundsettingspage.hpp"
#include "spenateditorhighlightsettingspage.hpp"

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
inline void initGraphLayoutResource() { Q_INIT_RESOURCE(graphlayout); }

using namespace TextEditor;

namespace spenateditor{
namespace Internal{

SpenatEditorPlugin* SpenatEditorPlugin::_instance = 0;

class SpenatEditorPlugin::Private
{
  friend class SpenatEditorPlugin;

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

SpenatEditorPlugin::SpenatEditorPlugin()
  : _d(new Private())
{
  _instance = this;
}

SpenatEditorPlugin::~SpenatEditorPlugin()
{
  // Unregister objects from the plugin manager's object pool
  // Delete members
  delete this->_d->_actionHandler;
  delete this->_d;
}

bool SpenatEditorPlugin::initialize(const QStringList &arguments,
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
  if(!mdb->addMimeTypes(QLatin1String(":/spenateditor/spenateditor.mimetypes.xml"),
                        errorString))
  {
    if(errorString)
    {
      *errorString+=QLatin1String("mimetype for spenateditor plugin "
                                  "could not be added!");
      qDebug()<<*errorString;
    }
    return false;
  }

  this->addAutoReleasedObject(new SpenatEditorFactory(this));
  SpenatGraphicFactory* graphicFactory = new SpenatGraphicFactory(this);
  this->addAutoReleasedObject(graphicFactory);
  this->addAutoReleasedObject(new SpenatHoverHandler(this));
  this->addAutoReleasedObject(new SpenatCompletionAssistProvider);
  this->addAutoReleasedObject(new Internal::SpenatEditorOutlineWidgetFactory());
  SpenatEditorGraphvizSettingsPage* page1 = new SpenatEditorGraphvizSettingsPage();
  SpenatEditorPlaceSettingsPage* page2 = new SpenatEditorPlaceSettingsPage();
  SpenatEditorTransitionSettingsPage* page3 = new SpenatEditorTransitionSettingsPage();
  SpenatEditorArcSettingsPage* page4 = new SpenatEditorArcSettingsPage();
  SpenatEditorBackgroundSettingsPage* page5 = new SpenatEditorBackgroundSettingsPage();
  SpenatEditorHighlightSettingsPage* page6 = new SpenatEditorHighlightSettingsPage();

  QObject::connect(page1, &SpenatEditorGraphvizSettingsPage::appliedSettings, graphicFactory, &SpenatGraphicFactory::refreshGraphicIn);
  QObject::connect(page2, &SpenatEditorPlaceSettingsPage::appliedSettings, graphicFactory, &SpenatGraphicFactory::refreshGraphicIn);
  QObject::connect(page3, &SpenatEditorTransitionSettingsPage::appliedSettings, graphicFactory, &SpenatGraphicFactory::refreshGraphicIn);
  QObject::connect(page4, &SpenatEditorArcSettingsPage::appliedSettings, graphicFactory, &SpenatGraphicFactory::refreshGraphicIn);
  QObject::connect(page5, &SpenatEditorBackgroundSettingsPage::appliedSettings, graphicFactory, &SpenatGraphicFactory::refreshGraphicIn);
  QObject::connect(page6, &SpenatEditorHighlightSettingsPage::appliedSettings, graphicFactory, &SpenatGraphicFactory::refreshGraphicIn);


  this->addAutoReleasedObject(page1);
  this->addAutoReleasedObject(page2);
  this->addAutoReleasedObject(page3);
  this->addAutoReleasedObject(page4);
  this->addAutoReleasedObject(page5);
  this->addAutoReleasedObject(page6);

  this->_d->_actionHandler = new TextEditorActionHandler(Constants::SPENATEDITOR_CONTEXT,
                                                         TextEditorActionHandler::Format
                                                         | TextEditorActionHandler::UnCommentSelection
                                                         | TextEditorActionHandler::UnCollapseAll);
  this->_d->_actionHandler->initializeActions();

  return true;
}

void SpenatEditorPlugin::extensionsInitialized()
{
  // Retrieve objects from the plugin manager's object pool
  // In the extensionsInitialized method, a plugin can be sure that all
  // plugins that depend on it are completely initialized.

  return;
}

ExtensionSystem::IPlugin::ShutdownFlag SpenatEditorPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void SpenatEditorPlugin::initializeEditor(SpenatEditorWidget* editorWidget)
{
  QTC_CHECK(_instance);
  this->_d->_actionHandler->setupActions(editorWidget);
  TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

void SpenatEditorPlugin::initializeGraphicEditor(SpenatGraphicWidget* editorWidget)
{
  Q_UNUSED(editorWidget);

  //    QTC_CHECK(_instance);
  //    this->_d->_actionHandler->setupActions(editorWidget);
  //    TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

const QString& SpenatEditorPlugin::defaultStyleSheets() const
{
  return this->_d->_defaultStyleSheets;
}

void SpenatEditorPlugin::appliedSettings(){
    emit this->refreshGraphic();
}

}///end namespace Internal
}///end namespace spenateditor

//using namespace distesteditor;
//using namespace distesteditor::Internal;

Q_EXPORT_PLUGIN(distesteditor::Internal::SpenatEditorPlugin)

