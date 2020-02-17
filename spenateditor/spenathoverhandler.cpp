#include "spenathoverhandler.hpp"

#include "spenateditor.hpp"
#include "spenateditorwidget.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace Core;

namespace spenateditor {

SpenatHoverHandler::SpenatHoverHandler(QObject *parent)
  : BaseHoverHandler(parent)
{}

SpenatHoverHandler::~SpenatHoverHandler()
{}

bool SpenatHoverHandler::acceptEditor(IEditor *editor)
{
  if (qobject_cast<SpenatEditor*>(editor))
    return true;
  return false;
}

void SpenatHoverHandler::identifyMatch(TextEditor::ITextEditor *editor, int pos)
{
  if (SpenatEditorWidget* spenatEditorWidget = qobject_cast<SpenatEditorWidget *>(editor->widget()))
  {
    if (! spenatEditorWidget->extraSelectionTooltip(pos).isEmpty())
      this->setToolTip(spenatEditorWidget->extraSelectionTooltip(pos));
  }
}

void SpenatHoverHandler::decorateToolTip()
{
  if (Qt::mightBeRichText(toolTip()))
    this->setToolTip(Qt::escape(toolTip()));
}

} // namespace spenateditor
