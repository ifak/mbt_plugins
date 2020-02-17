#include "commondeclhoverhandler.hpp"

#include "commondecleditor.hpp"
#include "commondecleditorwidget.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace Core;

namespace commondecleditor {

CommonDeclHoverHandler::CommonDeclHoverHandler(QObject *parent)
  : BaseHoverHandler(parent)
{}

CommonDeclHoverHandler::~CommonDeclHoverHandler()
{}

bool CommonDeclHoverHandler::acceptEditor(IEditor *editor)
{
  if (qobject_cast<CommonDeclEditor*>(editor))
    return true;
  return false;
}

void CommonDeclHoverHandler::identifyMatch(TextEditor::ITextEditor *editor, int pos)
{
  if (CommonDeclEditorWidget* commondeclEditorWidget = qobject_cast<CommonDeclEditorWidget *>(editor->widget()))
  {
    if (! commondeclEditorWidget->extraSelectionTooltip(pos).isEmpty())
      this->setToolTip(commondeclEditorWidget->extraSelectionTooltip(pos));
  }
}

void CommonDeclHoverHandler::decorateToolTip()
{
  if (Qt::mightBeRichText(toolTip()))
    this->setToolTip(Qt::escape(toolTip()));
}

} // namespace commondecleditor
