#include "isrdlautocompleter.hpp"

#include <mobata/memory_leak_start.hpp>

namespace isrdleditor {

IsrdlAutoCompleter::IsrdlAutoCompleter()
{}

IsrdlAutoCompleter::~IsrdlAutoCompleter()
{}

bool IsrdlAutoCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor,
                                                       const QString &textToInsert) const
{
  return DslAutoCompleter::contextAllowsAutoParentheses(cursor, textToInsert);
}

bool IsrdlAutoCompleter::contextAllowsElectricCharacters(const QTextCursor &cursor) const
{
  return DslAutoCompleter::contextAllowsElectricCharacters(cursor);
}

bool IsrdlAutoCompleter::isInComment(const QTextCursor &cursor) const
{
  return DslAutoCompleter::isInComment(cursor);
}

QString IsrdlAutoCompleter::insertMatchingBrace(const QTextCursor &cursor,
                                                 const QString &text,
                                                 QChar la,
                                                 int *skippedChars) const
{
  return DslAutoCompleter::insertMatchingBrace(cursor, text, la, skippedChars);
}

QString IsrdlAutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
  return DslAutoCompleter::insertParagraphSeparator(cursor);
}

} // namespace isrdleditor

