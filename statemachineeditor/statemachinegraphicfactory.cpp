#include "statemachinegraphicfactory.hpp"

#include "statemachineeditorconstants.hpp"
#include "statemachinegraphiceditor.hpp"
#include "statemachinegraphicwidget.hpp"
#include "statemachineeditorplugin.hpp"

#include <mobata/memory_leak_start.hpp>

namespace statemachineeditor {

StateMachineGraphicFactory::StateMachineGraphicFactory(QObject* parent)
  : Core::IEditorFactory(parent)
{}

StateMachineGraphicFactory::~StateMachineGraphicFactory()
{}

Core::IEditor* StateMachineGraphicFactory::createEditor(QWidget* parent)
{
  StateMachineGraphicWidget* stateMachineGraphicWidget = new StateMachineGraphicWidget(parent);
  StateMachineEditorPlugin::instance()->initializeGraphicEditor(stateMachineGraphicWidget);
  QObject::connect(this,
                   &StateMachineGraphicFactory::refreshGraphicOut,
                   stateMachineGraphicWidget,
                   QOverload<>::of(&StateMachineGraphicWidget::updateStateMachineGraphic));
  return stateMachineGraphicWidget->stateMachineGraphicEditor();
}

QStringList StateMachineGraphicFactory::mimeTypes() const
{
  return QStringList()<< QLatin1String(Constants::statemachineGRAPHIC_MIME_TYPE);
}

Core::Id StateMachineGraphicFactory::id() const
{
  return Core::Id(Constants::statemachineGRAPHIC_ID);
}

QString StateMachineGraphicFactory::displayName() const
{
  return QLatin1String(Constants::statemachineGRAPHIC_NAME);
}

void StateMachineGraphicFactory::refreshGraphicIn(){
    emit this->refreshGraphicOut();
}


} // namespace statemachineeditor
