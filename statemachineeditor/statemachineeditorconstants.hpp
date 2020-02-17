/*
 * This file is part of mobata.
 *
 * mobata is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mobata is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mobata.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QtGlobal>

namespace statemachineeditor {
namespace Constants {

static const char STATEMACHINEEDITOR_CONTEXT[]  = "State Machine Editor";
static const char	STATEMACHINEEDITOR_NAME[]     = "statemachine editor";
static const char	STATEMACHINEEDITOR_ID[]       = "StateMachineEditor";
static const char	statemachine_MIME_TYPE[]      = "application/statemachine";

static const char	statemachineGRAPHIC_NAME[] = "statemachine graphic";
static const char	statemachineGRAPHIC_ID[] = "StateMachineGraphic";
static const char	statemachineGRAPHIC_MIME_TYPE[] = "application/statemachinegraphic";

// Languages
static const char LANG_statemachine[]           = "statemachine";

// Settings page
const char statemachine_SETTINGS_CATEGORY[]  = "T.StateMachineEditor";
const char statemachine_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("StateMachineEditor", "State Machine Editor");
const char statemachine_SETTINGS_CATEGORY_ICON[]  = ":/mobata/images/statemachine/statemachine_blue.png";

//
const char statemachine_SETTINGS_ID[] = "A.StateMachineEditor";

} // namespace statemachineeditor
} // namespace Constants

