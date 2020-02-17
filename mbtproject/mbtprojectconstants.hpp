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

namespace mbtproject {
namespace Constants {

const char *const	MBT_PROJECTEDITOR_NAME = "MBT project editor";
const char *const	MBT_PROJECTEDITOR_ID = "MbtProject.MbtProjectEditor";
const char *const	MBT_PROJECT = "MbtProject.MbtProject";
const char *const MBT_PROJECTCONTEXT = "MbtProject.ProjectContext";

const char *const	MBT_PROJECT_MIME_TYPE = "application/mbtproject";

const char* const MBT_PROJECT_DECLFILES_KEY       = "DeclFiles";
const char* const MBT_PROJECT_SPECMODELFILES_KEY  = "SpecModelFiles";

//project node types
static const char MBT_PROJECTNODE_TYPE[]          = "MbtProject.ProjectNodeType";
static const char MBT_PROJECTNODE_DECLARATIONS[]  = "MbtProject.Declarations";
static const char MBT_PROJECTNODE_SPECMODELS[]    = "MbtProject.SpecModels";

//mbt project mode
static const char MBT_PROJECT_MODE_CONTEXT[]  = "MbtProject.MbtModeContext";
static const char MBT_PROJECT_MODE_ID[]       = "MbtProject.MbtModeId";
const int         MBT_PROJECT_MODE_PRIORITY   = 70;

// Actions:
static const char MBT_REPARSE_MBTPROJECT[]  = "MbtProject.Reparse";
static const char MBT_BUILD_CONTEXT[]       = "MbtProject.BuildCtx";
static const char MBT_BUILD[]               = "MbtProject.Build";
static const char MBT_BUILD_CLEAN[]         = "MbtProject.Build.Clean";
static const char MBT_ADDSPECMODEL[]        = "MbtProject.AddSpecModel";
static const char MBT_ADDEXISTSPECMODEL[]   = "MbtProject.AddExistSpecModel";

//toolchain, kit, run:
static const char MBT_TOOLCHAIN[]  = "MbtProject.MbtProjectToolChain";
static const char MBT_KIT[]        = "MbtProject.MbtProjectKit";
static const char MBT_RUN[]        = "MbtProject.MbtProjectRun";

// Settings
static const char MBT_RY[]  = "T.MBT";
static const char MBT_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("MBT", "Model Based Testing");
static const char MBT_SETTINGS_CATEGORY_ICON[]  = ":/core/images/filenew.png";
static const char MBT_SETTINGS_PREFIX[] = "MBT";

static const char MBT_PROJECT_SETTINGS_KEY[] = "mbtproject.MbtProject.MbtProjectSettings";

// TCG coverages
static const char MBT_TCGCOVERAGE_ALLPLACES[]         = "all-places";
static const char MBT_TCGCOVERAGE_ALLTRANSITIONS[]    = "all-transitions";
static const char MBT_TCGCOVERAGE_ALLDECISIONS[]      = "all-decisions";
static const char MBT_TCGCOVERAGE_ALLPATHS[]          = "all-paths";
static const char MBT_TCGCOVERAGE_ALLROUNDTRIPPATHS[] = "all-round-trip-paths";

//attributeNames
static const char FILENAME_ATTRIBUTE[] = "fileName";

} // namespace mbtproject
} // namespace Constants
