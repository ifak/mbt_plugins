#include "irdlgraphicfactory.hpp"

#include "irdleditorconstants.hpp"
#include "irdlgraphiceditor.hpp"
#include "irdlgraphicwidget.hpp"
#include "irdleditorplugin.hpp"

#include <QDebug>

namespace irdleditor {
namespace Internal{

IrdlGraphicFactory::IrdlGraphicFactory(QObject* parent)
    : Core::IEditorFactory(parent)
{}

IrdlGraphicFactory::~IrdlGraphicFactory()
{}

Core::IEditor* IrdlGraphicFactory::createEditor(QWidget* parent)
{
    IrdlGraphicWidget* irdlGraphicWidget = new IrdlGraphicWidget(parent);
    IrdlEditorPlugin::instance()->initializeGraphicEditor(irdlGraphicWidget);
    connect(this, &IrdlGraphicFactory::refreshGraphicOut,
            irdlGraphicWidget, &IrdlGraphicWidget::updateIrdlGraphic);

    return irdlGraphicWidget->irdlGraphicEditor();
}

QStringList IrdlGraphicFactory::mimeTypes() const
{
    return QStringList()<< QLatin1String(Constants::IRDLGRAPHIC_MIME_TYPE);
}

Core::Id IrdlGraphicFactory::id() const
{
    return Core::Id(Constants::IRDLGRAPHIC_ID);
}

QString IrdlGraphicFactory::displayName() const
{
    return QLatin1String(Constants::IRDLGRAPHIC_NAME);
}
void IrdlGraphicFactory::refreshGraphicIn(){
    emit this->refreshGraphicOut();
}
} // namespace Internal
} // namespace irdleditor
