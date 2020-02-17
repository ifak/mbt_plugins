/*
 *  This file is part of Irdl Editor Plugin for AppCreator based application.
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

#ifndef IRDLEDITOR_IRDLHIGHLIGHTER_HPP
#define IRDLEDITOR_IRDLHIGHLIGHTER_HPP

#include "irdleditor_global.hpp"

#include <dsleditor/dslhighlighter.hpp>

namespace irdleditor {

class IRDLEDITOR_EXPORT IrdlHighlighter
    : public dsleditor::DslHighlighter
{
  Q_OBJECT

public:
  IrdlHighlighter(QTextDocument* parent = 0);
  virtual ~IrdlHighlighter();

protected:
  void highlightBlock(const QString& text);
};

} // namespace irdleditor

#endif // IRDLEDITOR_IRDLHIGHLIGHTER_HPP
