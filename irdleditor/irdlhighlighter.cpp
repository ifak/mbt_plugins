#include "irdlhighlighter.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace irdleditor {

IrdlHighlighter::IrdlHighlighter(QTextDocument *parent)
  : DslHighlighter(parent)
{}

IrdlHighlighter::~IrdlHighlighter()
{}

void IrdlHighlighter::highlightBlock(const QString& text)
{
  return DslHighlighter::highlightBlock(text);
}

} // namespace irdleditor
