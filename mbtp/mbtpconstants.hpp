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
#ifndef MBTP_MBTPCONSTANTS_HPP
#define MBTP_MBTPCONSTANTS_HPP

namespace mbtp {
namespace Constants {

const char *const MBTP_PROJECT            = "MbtpProject.MbtpProject";
const char *const MBTP_PROJECTCONTEXT     = "MbtpProject.ProjectContext";

const char *const MBTP_PROJECT_MIME_TYPE = "application/mbtpproject";
//mbtp project mode
static const char MBTP_PROJECT_MODE_CONTEXT[]  = "MbtpProject.MbtpModeContext";
static const char MBTP_PROJECT_MODE_ID[]       = "MbtpProject.MbtpModeId";
const int         MBTP_PROJECT_MODE_PRIORITY   = 65;

static const char MBTP_PROJECTNODE_TYPE[]       = "Mbtp.ProjectNodeType";
static const char MBTP_PROJECTNODE_MODEL[]      = "Mbtp.SpenatJson";
static const char MBTP_PROJECTNODE_TRACE[]      = "Mbtp.Trace";
static const char MBTP_PROJECTNODE_ATTRIBUTES[] = "Mbtp.Attributes";

static const char MBTP_PRIOREPORTGRAPHIC[] = "Mbtp.ResultGraphic";
static const char MBTP_GRAPHIC[] = "Mbtp.Graphic";
static const char MBTP_GRAPHIC_NAME[] = "Mbtp.TestPrio";
static const char MBTPGRAPHIC_MIME_TYPE[] = ".prioreport";

const char *const MBTP_ATTRIBUTES_MIME_TYPE = "application/attributeseditor";
static const char MBTP_ATTRIBUTES[] = "Mbtp.Attributes";
static const char MBTP_ATTRIBUTES_NAME[] = "Mbtp.Attributes";
static const char MBTP_ATTRIBUTES_MIME[] = ".attributes";

static const char MBTP_SETTINGS[] = "Mbtp.Settings";
static const char MBTP_SETTINGS_CATEGORY[] = "Mbtp.Settings.Category";
static const char MBTP_SETTINGS_TR_CATEGORY[] = "Mbtp Settings";

// Actions:
const char ACTION_BUILD_CONTEXT[] = "mbtp.BuildCtx";
const char ACTION_BUILD[] = "mbtp.Build";
const char ACTION_ADDCONFIGFILE[] = "mbtp.AddConfigFile";

const char MBTP_TOOLCHAIN[] = "mbtp.MbtpToolChain";
const char MBTP_KIT[] = "mbtp.MbtpKit";
const char MBTP_BUILD[] = "mbtp.MbtpBuild";
const char MBTP_RUN[] = "mbtp.MbtpRun";

} // namespace mbtp
} // namespace Constants

#endif // MBTP_MBTPCONSTANTS_HPP
