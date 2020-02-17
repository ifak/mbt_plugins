/*
 *  This file is part of Spenat Editor Plugin for AppCreator based application.
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

#ifndef SPENATEDITOR_SPENATHIGHLIGHTER_HPP
#define SPENATEDITOR_SPENATHIGHLIGHTER_HPP

#include "spenateditor_global.hpp"

#include <dsleditor/dslhighlighter.hpp>

namespace spenateditor {

class SPENATEDITOR_EXPORT SpenatHighlighter
    : public dsleditor::DslHighlighter
{
  Q_OBJECT

public:
  SpenatHighlighter(QTextDocument* parent = 0);
  virtual ~SpenatHighlighter();

protected:
  void highlightBlock(const QString& text);
};

} // namespace spenateditor

#endif // SPENATEDITOR_SPENATHIGHLIGHTER_HPP
