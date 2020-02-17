#include "commondeclhighlighter.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace commondecleditor {

CommonDeclHighlighter::CommonDeclHighlighter(QTextDocument *parent)
  : DslHighlighter(parent)
{}

CommonDeclHighlighter::~CommonDeclHighlighter()
{}

void CommonDeclHighlighter::highlightBlock(const QString& text)
{
  return DslHighlighter::highlightBlock(text);
}

} // namespace commondecleditor
