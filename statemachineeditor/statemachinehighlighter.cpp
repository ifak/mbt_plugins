#include "statemachinehighlighter.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace statemachineeditor {

StateMachineHighlighter::StateMachineHighlighter(QTextDocument *parent)
  : DslHighlighter(parent)
{}

StateMachineHighlighter::~StateMachineHighlighter()
{}

void StateMachineHighlighter::highlightBlock(const QString& text)
{
  return DslHighlighter::highlightBlock(text);
}

} // namespace statemachineeditor
