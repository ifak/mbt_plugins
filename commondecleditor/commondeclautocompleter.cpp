#include "commondeclautocompleter.hpp"

#include <mobata/memory_leak_start.hpp>

namespace commondecleditor {

CommonDeclAutoCompleter::CommonDeclAutoCompleter()
{}

CommonDeclAutoCompleter::~CommonDeclAutoCompleter()
{}

bool CommonDeclAutoCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor,
                                                       const QString &textToInsert) const
{
  return DslAutoCompleter::contextAllowsAutoParentheses(cursor, textToInsert);
}

bool CommonDeclAutoCompleter::contextAllowsElectricCharacters(const QTextCursor &cursor) const
{
  return DslAutoCompleter::contextAllowsElectricCharacters(cursor);
}

bool CommonDeclAutoCompleter::isInComment(const QTextCursor &cursor) const
{
  return DslAutoCompleter::isInComment(cursor);
}

QString CommonDeclAutoCompleter::insertMatchingBrace(const QTextCursor &cursor,
                                                 const QString &text,
                                                 QChar la,
                                                 int *skippedChars) const
{
  return DslAutoCompleter::insertMatchingBrace(cursor, text, la, skippedChars);
}

QString CommonDeclAutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
  return DslAutoCompleter::insertParagraphSeparator(cursor);
}

} // namespace commondecleditor

