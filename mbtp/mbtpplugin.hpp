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
#ifndef MBTP_MBTPPLUGIN_HPP
#define MBTP_MBTPPLUGIN_HPP

#include <extensionsystem/iplugin.h>

class QAction;

namespace ProjectExplorer
{
class Project;
class Node;
}

namespace mbtp {

class MbtpPlugin : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "mbtp.json")

public:
  MbtpPlugin();
  ~MbtpPlugin();

public:
  bool initialize(const QStringList &arguments, QString *errorString);
  void extensionsInitialized();
  bool delayedInitialize();
  ShutdownFlag aboutToShutdown();

private slots:
  void registerToolChain();
  void showContextMenu(ProjectExplorer::Project* project,
                       ProjectExplorer::Node* node);

private:
  Q_DISABLE_COPY(MbtpPlugin)
  class Private;
  Private*  _d;
};

} // namespace mbtp

#endif // MBTP_MBTPPLUGIN_HPP

