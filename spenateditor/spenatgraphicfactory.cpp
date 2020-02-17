#include "spenatgraphicfactory.hpp"

#include "spenateditorconstants.hpp"
#include "spenatgraphiceditor.hpp"
#include "spenatgraphicwidget.hpp"
#include "spenateditorplugin.hpp"

#include <QDebug>

namespace spenateditor {
namespace Internal{

SpenatGraphicFactory::SpenatGraphicFactory(QObject* parent)
  : Core::IEditorFactory(parent)
{}

SpenatGraphicFactory::~SpenatGraphicFactory()
{}

Core::IEditor* SpenatGraphicFactory::createEditor(QWidget* parent)
{
  SpenatGraphicWidget* spenatGraphicWidget = new SpenatGraphicWidget(parent);
  SpenatEditorPlugin::instance()->initializeGraphicEditor(spenatGraphicWidget);
  QObject::connect(this, &SpenatGraphicFactory::refreshGraphicOut, spenatGraphicWidget, &SpenatGraphicWidget::updateSpenatGraphic);
  return spenatGraphicWidget->spenatGraphicEditor();
}

QStringList SpenatGraphicFactory::mimeTypes() const
{
  return QStringList()<< QLatin1String(Constants::SPENATGRAPHIC_MIME_TYPE);
}

Core::Id SpenatGraphicFactory::id() const
{
  return Core::Id(Constants::SPENATGRAPHIC_ID);
}

QString SpenatGraphicFactory::displayName() const
{
  return QLatin1String(Constants::SPENATGRAPHIC_NAME);
}
void SpenatGraphicFactory::refreshGraphicIn(){
    emit this->refreshGraphicOut();
}
} // namespace Internal
} // namespace spenateditor
