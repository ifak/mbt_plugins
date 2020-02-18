/*
 * This file is part of mbt_plugins.
 *
 * mbt_plugins is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mbt_plugins is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mbt_plugins.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QtGlobal>

namespace irdleditor {
namespace Constants {

static const char IRDLEDITOR_CONTEXT[]  = "Irdl Editor";
static const char	IRDLEDITOR_NAME[]     = "irdl editor";
static const char	IRDLEDITOR_ID[]       = "IrdlEditor";
static const char	REQUIREMENT_MIME_TYPE[]      = "application/irdl";
static const char	REQUIREMENTDECL_MIME_TYPE[]      = "application/irdlDecl";

static const char	IRDLGRAPHIC_NAME[] = "irdl graphic";
static const char	IRDLGRAPHIC_ID[] = "IrdlGraphic";
static const char	IRDLGRAPHIC_MIME_TYPE[] = "application/irdlgraphic";

// Languages
static const char LANG_IRDL[]           = "IRDL";

// Settings page
const char IRDL_SETTINGS_CATEGORY[]  = "T.IrdlEditor";
const char IRDL_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("IrdlEditor", "Irdl Editor");
const char IRDL_SETTINGS_CATEGORY_ICON[]  = ":/mobata/images/msc/manymsc.png";

//
const char IRDL_SETTINGS_ID[] = "A.IrdlEditor";

} // namespace irdleditor
} // namespace Constants

