#include "spenathighlighter.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace spenateditor {

SpenatHighlighter::SpenatHighlighter(QTextDocument *parent)
  : DslHighlighter(parent)
{}

SpenatHighlighter::~SpenatHighlighter()
{}

void SpenatHighlighter::highlightBlock(const QString& text)
{
  return DslHighlighter::highlightBlock(text);
}

} // namespace spenateditor
