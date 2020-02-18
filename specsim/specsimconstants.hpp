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

namespace specsim {
namespace Constants {

const char *const	SPECSIM_PROJECTEDITOR_NAME  = "model simulation project editor";
const char *const	SPECSIM_PROJECTEDITOR_ID    = "SpecSim.SpecSimProjectEditor";
const char *const	SPECSIM_PROJECT             = "SpecSim.SpecSimProject";
const char *const SPECSIM_PROJECTCONTEXT      = "SpecSim.ProjectContext";

const char *const	SPECSIM_PROJECT_MIME_TYPE = "application/specsimproject";

const char* const SPECSIM_PROJECT_DECLFILES_KEY          = "DeclFiles";
const char* const SPECSIM_PROJECT_SPECSIMMODELFILES_KEY  = "SpecSimModelFiles";

//project node types
static const char SPECSIM_PROJECTNODE_TYPE[]          = "SpecSim.ProjectNodeType";
static const char SPECSIM_PROJECTNODE_DECLARATIONS[]  = "SpecSim.Declarations";
static const char SPECSIM_PROJECTNODE_SPECSIMMODELS[] = "SpecSim.SpecSimModels";

//spec sim project mode
static const char SPECSIM_MODE_CONTEXT[]  = "SpecSim.SpecSimModeContext";
static const char SPECSIM_MODE_ID[]       = "SpecSim.SpecSimModeId";
const int         SPECSIM_MODE_PRIORITY   = 75;

// Actions:
static const char SPECSIM_REPARSE_SPECSIMPROJECT[]  = "SpecSim.Reparse";
static const char SPECSIM_BUILD_CONTEXT[]           = "SpecSim.BuildCtx";
static const char SPECSIM_BUILD[]                   = "SpecSim.Build";
static const char SPECSIM_BUILD_CLEAN[]             = "SpecSim.Build.Clean";
static const char SPECSIM_ADDSIMMODEL[]             = "SpecSim.AddSimModel";
static const char SPECSIM_ADDEXISTSIMMODEL[]        = "SpecSim.AddExistSimModel";

//Settings
static const char SPECSIM_PROJECT_SETTINGS_KEY[] = "specsim.SpecSimProject.SpecSimProjectSettings";

//toolchain, kit, run:
static const char SPECSIM_TOOLCHAIN[]  = "SpecSim.SpecSimProjectToolChain";
static const char SPECSIM_KIT[]        = "SpecSim.SpecSimProjectKit";
static const char SPECSIM_RUN[]        = "SpecSim.SpecSimProjectRun";


static const char SIMSYSTEMEDITOR_CONTEXT[]  = "State Machine Editor";
static const char	SIMSYSTEMEDITOR_NAME[]     = "SimSystem editor";
static const char	SIMSYSTEMEDITOR_ID[]       = "SimSystemEditor";
static const char	SIMSYSTEM_MIME_TYPE[]      = "application/simsystem";

static const char	SIMSYSTEMGRAPHIC_NAME[] = "SimSystem graphic";
static const char	SIMSYSTEMGRAPHIC_ID[] = "SimSystemGraphic";
static const char	SIMSYSTEMGRAPHIC_MIME_TYPE[] = "application/simsystemgraphic";

// Languages
static const char LANG_SIMSYSTEM[]           = "SIMSYSTEM";

// Settings page
const char SIMSYSTEM_SETTINGS_CATEGORY[]  = "T.SimSystemEditor";
const char SIMSYSTEM_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("SimSystemEditor", "SimSystem Editor");
const char SIMSYSTEM_SETTINGS_CATEGORY_ICON[]  = ":/mobata/images/SIMSYSTEM/SIMSYSTEM_blue.png";

//
const char SIMSYSTEM_SETTINGS_ID[] = "A.SimSystemEditor";

} // namespace Constants
} // namespace specsim
