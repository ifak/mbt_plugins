#include "simsystemautocompleter.hpp"

#include <mobata/memory_leak_start.hpp>

namespace simsystemeditor {

SimSystemAutoCompleter::SimSystemAutoCompleter()
{}

SimSystemAutoCompleter::~SimSystemAutoCompleter()
{}

bool SimSystemAutoCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor,
                                                       const QString &textToInsert) const
{
  return DslAutoCompleter::contextAllowsAutoParentheses(cursor, textToInsert);
}

bool SimSystemAutoCompleter::contextAllowsElectricCharacters(const QTextCursor &cursor) const
{
  return DslAutoCompleter::contextAllowsElectricCharacters(cursor);
}

bool SimSystemAutoCompleter::isInComment(const QTextCursor &cursor) const
{
  return DslAutoCompleter::isInComment(cursor);
}

QString SimSystemAutoCompleter::insertMatchingBrace(const QTextCursor &cursor,
                                                 const QString &text,
                                                 QChar la,
                                                 int *skippedChars) const
{
  return DslAutoCompleter::insertMatchingBrace(cursor, text, la, skippedChars);
}

QString SimSystemAutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
  return DslAutoCompleter::insertParagraphSeparator(cursor);
}

} // namespace simsystemeditor

