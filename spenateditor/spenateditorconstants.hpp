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

#ifndef SPENATEDITOR_SPENATEDITORCONSTANTS_HPP
#define SPENATEDITOR_SPENATEDITORCONSTANTS_HPP

#include <QtGlobal>

namespace spenateditor {
namespace Constants {

static const char SPENATEDITOR_CONTEXT[]  = "Spenat Editor";
static const char	SPENATEDITOR_NAME[]     = "spenat editor";
static const char	SPENATEDITOR_ID[]       = "SpenatEditor";
static const char	SPENAT_MIME_TYPE[]      = "application/spenat";

static const char	SPENATGRAPHIC_NAME[] = "spenat graphic";
static const char	SPENATGRAPHIC_ID[] = "SpenatGraphic";
static const char	SPENATGRAPHIC_MIME_TYPE[] = "application/spenatgraphic";

// Languages
static const char LANG_SPENAT[]           = "SPENAT";

// Settings page
const char SPENAT_SETTINGS_CATEGORY[]  = "T.SpenatEditor";
const char SPENAT_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("SpenatEditor", "Spenat Editor");
const char SPENAT_SETTINGS_CATEGORY_ICON[]  = ":/mobata/images/spenat/spenat_blue.png";

//
const char SPENAT_SETTINGS_ID[] = "A.SpenatEditor";

} // namespace spenateditor
} // namespace Constants

#endif // SPENATEDITOR_SPENATEDITORCONSTANTS_HPP

