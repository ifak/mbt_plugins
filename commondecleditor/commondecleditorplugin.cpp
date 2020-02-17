#include "commondecleditorplugin.hpp"

#include "commondecleditorconstants.hpp"

#include "commondecleditorfactory.hpp"
#include "commondeclhoverhandler.hpp"
#include "commondeclcompletionassist.hpp"
#include "commondecleditoroutline.hpp"
#include "commondecleditorwidget.hpp"

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

using namespace TextEditor;

namespace commondecleditor{
namespace Internal{

CommonDeclEditorPlugin* CommonDeclEditorPlugin::_instance = 0;

class CommonDeclEditorPlugin::Private
{
  friend class CommonDeclEditorPlugin;

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

CommonDeclEditorPlugin::CommonDeclEditorPlugin()
  : _d(new Private())
{
  _instance = this;
}

CommonDeclEditorPlugin::~CommonDeclEditorPlugin()
{
  // Unregister objects from the plugin manager's object pool
  // Delete members
  delete this->_d->_actionHandler;
  delete this->_d;
}

bool CommonDeclEditorPlugin::initialize(const QStringList &arguments,
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

  Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
  if(!mdb->addMimeTypes(QLatin1String(":/commondecleditor/commondecleditor.mimetypes.xml"),
                        errorString))
  {
    if(errorString)
    {
      *errorString+=QLatin1String("mimetype for commondecleditor plugin "
                                  "could not be added!");
      qDebug()<<*errorString;
    }
    return false;
  }

  this->addAutoReleasedObject(new CommonDeclEditorFactory(this));
  this->addAutoReleasedObject(new CommonDeclHoverHandler(this));
  this->addAutoReleasedObject(new CommonDeclCompletionAssistProvider);
  this->addAutoReleasedObject(new Internal::CommonDeclEditorOutlineWidgetFactory());

  this->_d->_actionHandler = new TextEditorActionHandler(Constants::COMMONDECLEDITOR_CONTEXT,
                                                         TextEditorActionHandler::Format
                                                         | TextEditorActionHandler::UnCommentSelection
                                                         | TextEditorActionHandler::UnCollapseAll);
  this->_d->_actionHandler->initializeActions();

  return true;
}

void CommonDeclEditorPlugin::extensionsInitialized()
{
  // Retrieve objects from the plugin manager's object pool
  // In the extensionsInitialized method, a plugin can be sure that all
  // plugins that depend on it are completely initialized.

  return;
}

ExtensionSystem::IPlugin::ShutdownFlag CommonDeclEditorPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void CommonDeclEditorPlugin::initializeEditor(CommonDeclEditorWidget* editorWidget)
{
  QTC_CHECK(_instance);
  this->_d->_actionHandler->setupActions(editorWidget);
  TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

void CommonDeclEditorPlugin::initializeGraphicEditor(CommonDeclGraphicWidget* editorWidget)
{
    Q_UNUSED(editorWidget);
  //    QTC_CHECK(_instance);
  //    this->_d->_actionHandler->setupActions(editorWidget);
  //    TextEditorSettings::instance()->initializeEditor(editorWidget);

  return;
}

const QString& CommonDeclEditorPlugin::defaultStyleSheets() const
{
  return this->_d->_defaultStyleSheets;
}

}///end namespace Internal
}///end namespace commondecleditor

//using namespace distesteditor;
//using namespace distesteditor::Internal;

Q_EXPORT_PLUGIN(distesteditor::Internal::CommonDeclEditorPlugin)

