#include "irdleditorplugin.hpp"

#include "irdleditorconstants.hpp"

#include "irdleditorfactory.hpp"
#include "irdlgraphicfactory.hpp"
#include "irdlhoverhandler.hpp"
#include "irdlcompletionassist.hpp"
#include "irdleditoroutline.hpp"
#include "irdleditorwidget.hpp"
#include "irdleditorstepsettingspage.hpp"
#include "irdleditorbackgroundsettingspage.hpp"

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

namespace irdleditor{
namespace Internal{

IrdlEditorPlugin* IrdlEditorPlugin::_instance = 0;

class IrdlEditorPlugin::Private
{
    friend class IrdlEditorPlugin;

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

IrdlEditorPlugin::IrdlEditorPlugin()
    : _d(new Private())
{
    _instance = this;
}

IrdlEditorPlugin::~IrdlEditorPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
    delete this->_d->_actionHandler;
    delete this->_d;
}

bool IrdlEditorPlugin::initialize(const QStringList &arguments,
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
    if(!mdb->addMimeTypes(QLatin1String(":/irdleditor/irdleditor.mimetypes.xml"),
                          errorString))
    {
        if(errorString)
        {
            *errorString+=QLatin1String("mimetype for irdleditor plugin "
                                        "could not be added!");
            qDebug()<<*errorString;
        }
        return false;
    }

    this->addAutoReleasedObject(new IrdlEditorFactory(this));
    IrdlGraphicFactory* graphicFactory = new IrdlGraphicFactory(this);
    this->addAutoReleasedObject(graphicFactory);
    this->addAutoReleasedObject(new IrdlHoverHandler(this));
    this->addAutoReleasedObject(new IrdlCompletionAssistProvider);
    this->addAutoReleasedObject(new Internal::IrdlEditorOutlineWidgetFactory());

    IrdlEditorBackgroundSettingsPage* page1 = new IrdlEditorBackgroundSettingsPage();
    IrdlEditorStepSettingsPage* page2 = new IrdlEditorStepSettingsPage();

    QObject::connect(page1, &IrdlEditorBackgroundSettingsPage::appliedSettings, graphicFactory, &IrdlGraphicFactory::refreshGraphicIn);
    QObject::connect(page2, &IrdlEditorStepSettingsPage::appliedSettings, graphicFactory, &IrdlGraphicFactory::refreshGraphicIn);

    this->addAutoReleasedObject(page1);
    this->addAutoReleasedObject(page2);

    this->_d->_actionHandler = new TextEditorActionHandler(Constants::IRDLEDITOR_CONTEXT,
                                                           TextEditorActionHandler::Format
                                                           | TextEditorActionHandler::UnCommentSelection
                                                           | TextEditorActionHandler::UnCollapseAll);
    this->_d->_actionHandler->initializeActions();

    return true;
}

void IrdlEditorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized method, a plugin can be sure that all
    // plugins that depend on it are completely initialized.

    return;
}

ExtensionSystem::IPlugin::ShutdownFlag IrdlEditorPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void IrdlEditorPlugin::initializeEditor(IrdlEditorWidget* editorWidget)
{
    QTC_CHECK(_instance);
    this->_d->_actionHandler->setupActions(editorWidget);
    TextEditorSettings::instance()->initializeEditor(editorWidget);

    return;
}

void IrdlEditorPlugin::initializeGraphicEditor(IrdlGraphicWidget* editorWidget)
{
    Q_UNUSED(editorWidget);
    //    QTC_CHECK(_instance);
    //    this->_d->_actionHandler->setupActions(editorWidget);
    //    TextEditorSettings::instance()->initializeEditor(editorWidget);

    return;
}

const QString& IrdlEditorPlugin::defaultStyleSheets() const
{
    return this->_d->_defaultStyleSheets;
}

}///end namespace Internal
}///end namespace irdleditor

//using namespace distesteditor;
//using namespace distesteditor::Internal;

Q_EXPORT_PLUGIN(distesteditor::Internal::IrdlEditorPlugin)

