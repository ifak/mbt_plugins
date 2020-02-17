/*
 *  This file is part of Isrdl Editor Plugin for AppCreator based application.
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

#ifndef ISRDLEDITOR_ISRDLEDITORCONSTANTS_HPP
#define ISRDLEDITOR_ISRDLEDITORCONSTANTS_HPP

#include <QtGlobal>

namespace isrdleditor {
namespace Constants {

static const char ISRDLEDITOR_CONTEXT[]  = "Isrdl Editor";
static const char	ISRDLEDITOR_NAME[]     = "isrdl editor";
static const char	ISRDLEDITOR_ID[]       = "IsrdlEditor";
static const char	REQUIREMENT_MIME_TYPE[]      = "application/isrdl";
static const char	REQUIREMENTDECL_MIME_TYPE[]      = "application/isrdlDecl";

static const char	ISRDLGRAPHIC_NAME[] = "isrdl graphic";
static const char	ISRDLGRAPHIC_ID[] = "IsrdlGraphic";
static const char	ISRDLGRAPHIC_MIME_TYPE[] = "application/isrdlgraphic";

// Languages
static const char LANG_ISRDL[]           = "ISRDL";

// Settings page
const char ISRDL_SETTINGS_CATEGORY[]  = "T.IsrdlEditor";
const char ISRDL_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("IsrdlEditor", "Isrdl Editor");
const char ISRDL_SETTINGS_CATEGORY_ICON[]  = ":/mobata/images/isrdl/isrdl_blue.png";

//
const char ISRDL_SETTINGS_ID[] = "A.IsrdlEditor";

} // namespace isrdleditor
} // namespace Constants

#endif // ISRDLEDITOR_ISRDLEDITORCONSTANTS_HPP

