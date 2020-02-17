#include "irdlhoverhandler.hpp"

#include "irdleditor.hpp"
#include "irdleditorwidget.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace Core;

namespace irdleditor {

IrdlHoverHandler::IrdlHoverHandler(QObject *parent)
  : BaseHoverHandler(parent)
{}

IrdlHoverHandler::~IrdlHoverHandler()
{}

bool IrdlHoverHandler::acceptEditor(IEditor *editor)
{
  if (qobject_cast<IrdlEditor*>(editor))
    return true;
  return false;
}

void IrdlHoverHandler::identifyMatch(TextEditor::ITextEditor *editor, int pos)
{
  if (IrdlEditorWidget* irdlEditorWidget = qobject_cast<IrdlEditorWidget *>(editor->widget()))
  {
    if (! irdlEditorWidget->extraSelectionTooltip(pos).isEmpty())
      this->setToolTip(irdlEditorWidget->extraSelectionTooltip(pos));
  }
}

void IrdlHoverHandler::decorateToolTip()
{
  if (Qt::mightBeRichText(toolTip()))
    this->setToolTip(Qt::escape(toolTip()));
}

} // namespace irdleditor
