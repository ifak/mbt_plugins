#include "statemachineeditor.hpp"
#include "statemachineeditorwidget.hpp"
#include "statemachineeditorconstants.hpp"
#include "statemachineeditorplugin.hpp"

#include <coreplugin/coreconstants.h>
#include <texteditor/texteditorsettings.h>

#include <mobata/utils/functors.hpp>

#include <QDir>
#include <QTemporaryFile>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace statemachineeditor;
using namespace model::statemachine;

class StateMachineEditor::Private
{
  friend class StateMachineEditor;
};

StateMachineEditor::StateMachineEditor(StateMachineEditorWidget* StateMachineEditorWidget)
  : TextEditor::BaseTextEditor(StateMachineEditorWidget),
    _d(new Private)
{
  this->m_context.add(Constants::STATEMACHINEEDITOR_CONTEXT);
  this->m_context.add(Constants::LANG_statemachine);
  this->m_context.add(TextEditor::Constants::C_TEXTEDITOR);

  //graphic action///
  QAction* stateMachineGraphAction=new QAction(QIcon(QLatin1String(":/mobata/images/statemachine/statemachine_blue.png")),
                                         tr("update/create statemachine graphic..."),
                                         this);
  stateMachineGraphAction->setStatusTip(tr("statemachine graphic"));
  connect(stateMachineGraphAction,
          &QAction::triggered,
          StateMachineEditorWidget,
          &StateMachineEditorWidget::showStateMachineGraphic);
  this->toolBar()->addAction(stateMachineGraphAction);
}

StateMachineEditor::~StateMachineEditor()
{
  delete this->_d;
}

Core::Id StateMachineEditor::stateMachineEditorId()
{
  return Core::Id(Constants::STATEMACHINEEDITOR_ID);
}

bool StateMachineEditor::duplicateSupported() const
{
  return true;
}

Core::IEditor* StateMachineEditor::duplicate(QWidget *parent)
{
  StateMachineEditorWidget* statemachineEditorWidget = new StateMachineEditorWidget(parent);
  statemachineEditorWidget->duplicateFrom(this->editorWidget());
  StateMachineEditorPlugin::instance()->initializeEditor(statemachineEditorWidget);
  return statemachineEditorWidget->editor();
}

bool StateMachineEditor::isTemporary() const
{
  return false;
}

Core::Id StateMachineEditor::id() const
{
  return StateMachineEditor::stateMachineEditorId();
}

StateMachineDocument* StateMachineEditor::document()
{
  return qobject_cast<StateMachineDocument*>(TextEditor::BaseTextEditor::document());
}

StateMachineModel* StateMachineEditor::statemachineDeclModel()
{
  return this->document()?this->document()->statemachineDeclModel():0;
}

StateMachineOutlineModelPtr StateMachineEditor::statemachineOutlineModel()
{
  return this->document()?this->document()->statemachineOutlineModel():0;
}
