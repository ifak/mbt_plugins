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

#include "mbtproject_global.hpp"

#include <extensionsystem/iplugin.h>
#include "mbtproject_global.hpp"

class QAction;
class QMenu;

namespace ProjectExplorer
{
class Project;
class Node;
}

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtProjectPlugin : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "mbtproject.json")
  
public:
  MbtProjectPlugin();
  ~MbtProjectPlugin();
  
public:  
  bool initialize(const QStringList &arguments, QString *errorString);
  void extensionsInitialized();
  bool delayedInitialize();
  ShutdownFlag aboutToShutdown();
  static MbtProjectPlugin* instance();
  QMenu* specModelsMenu();

private slots:
  void showContextMenu(ProjectExplorer::Project *project,
                       ProjectExplorer::Node *node);
  void addSpecModel();
  void addExistingSpecModel();

private:
  void hideAction(QAction* action);

private slots:
  void registerToolChain();

private:
  Q_DISABLE_COPY(MbtProjectPlugin)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
