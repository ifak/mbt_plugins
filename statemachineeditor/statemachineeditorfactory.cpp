#include "statemachineeditorfactory.hpp"
#include "statemachineeditorconstants.hpp"
#include "statemachineeditorplugin.hpp"
#include "statemachineeditor.hpp"
#include "statemachineeditorwidget.hpp"

#include <coreplugin/id.h>
#include <texteditor/texteditorsettings.h>

#include <QStringList>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace statemachineeditor{

class StateMachineEditorFactory::Private
{
  friend class StateMachineEditorFactory;

  Private(StateMachineEditorPlugin*	StateMachineEditorPlugin)
    :	_stateMachineEditorPlugin(StateMachineEditorPlugin),
      _id(Constants::STATEMACHINEEDITOR_ID)
  {
    this->_mimeTypes<<QLatin1String(Constants::statemachine_MIME_TYPE);
  }

public:
  ~Private()
  {}

private:
  StateMachineEditorPlugin*       _stateMachineEditorPlugin;
  Core::Id                  _id;
  QStringList               _mimeTypes;
};

StateMachineEditorFactory::StateMachineEditorFactory(StateMachineEditorPlugin *owner)
  :Core::IEditorFactory(owner),
    _d(new Private(owner))
{
  Q_ASSERT(this->_d->_stateMachineEditorPlugin);
}

StateMachineEditorFactory::~StateMachineEditorFactory()
{
  delete this->_d;
}

QStringList StateMachineEditorFactory::mimeTypes() const
{
  return this->_d->_mimeTypes;
}

Core::Id StateMachineEditorFactory::id() const
{
  return this->_d->_id;
}

QString StateMachineEditorFactory::displayName() const
{
  return QLatin1String(Constants::STATEMACHINEEDITOR_NAME);
}

Core::IEditor* StateMachineEditorFactory::createEditor(QWidget *parent)
{
  StateMachineEditorWidget* editorWidget = new StateMachineEditorWidget(parent);
  StateMachineEditorPlugin::instance()->initializeEditor(editorWidget);

  return editorWidget->editor();
}

} //end namespace statemachineeditor
