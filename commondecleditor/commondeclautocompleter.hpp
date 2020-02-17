/*
 *  This file is part of CommonDecl Editor Plugin for AppCreator based application.
 *
 *  Copyright (C) 2013 Jan Krause <jan.krause.no19@gmail.com>
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
*/

#pragma once

#include "commondecleditor_global.hpp"

#include <dsleditor/dslautocompleter.hpp>

namespace commondecleditor {

class COMMONDECLEDITOR_EXPORT CommonDeclAutoCompleter
        : public dsleditor::DslAutoCompleter
{
public:
  CommonDeclAutoCompleter();
  virtual ~CommonDeclAutoCompleter();

public:
  virtual bool    contextAllowsAutoParentheses(const QTextCursor &cursor,
                                               const QString &textToInsert = QString()) const;
  virtual bool    contextAllowsElectricCharacters(const QTextCursor &cursor) const;
  virtual bool    isInComment(const QTextCursor &cursor) const;
  virtual QString insertMatchingBrace(const QTextCursor &tc,
                                      const QString &text,
                                      QChar la,
                                      int *skippedChars) const;
  virtual QString insertParagraphSeparator(const QTextCursor &tc) const;
};

} // namespace commondecleditor
