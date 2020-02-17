#include "statemachinehoverhandler.hpp"

#include "statemachineeditor.hpp"
#include "statemachineeditorwidget.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace Core;

namespace statemachineeditor {

StateMachineHoverHandler::StateMachineHoverHandler(QObject *parent)
  : BaseHoverHandler(parent)
{}

StateMachineHoverHandler::~StateMachineHoverHandler()
{}

bool StateMachineHoverHandler::acceptEditor(IEditor *editor)
{
  if (qobject_cast<StateMachineEditor*>(editor))
    return true;
  return false;
}

void StateMachineHoverHandler::identifyMatch(TextEditor::ITextEditor *editor, int pos)
{
  if (StateMachineEditorWidget* stateMachineEditorWidget = qobject_cast<StateMachineEditorWidget *>(editor->widget()))
  {
    if (! stateMachineEditorWidget->extraSelectionTooltip(pos).isEmpty())
      this->setToolTip(stateMachineEditorWidget->extraSelectionTooltip(pos));
  }
}

void StateMachineHoverHandler::decorateToolTip()
{
  if (Qt::mightBeRichText(toolTip()))
    this->setToolTip(Qt::escape(toolTip()));
}

} // namespace statemachineeditor
