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

#include <extensionsystem/iplugin.h>
#include "specsim_global.hpp"
#include "simsystemeditorwidget.hpp"

namespace ProjectExplorer{
class Project;
class Node;
}

namespace specsim {

class SPECSIMSHARED_EXPORT SpecSimPlugin
    : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "specsim.json")
  
public:
  SpecSimPlugin();
  ~SpecSimPlugin();

public:
  static SpecSimPlugin* instance();
  
public:  
  bool          initialize(const QStringList &arguments, QString *errorString);
  void          extensionsInitialized();
  void          initializeEditor(SimSystemEditorWidget* editorWidget);
  bool          delayedInitialize();
  ShutdownFlag  aboutToShutdown();

private slots:
  void registerToolChain();
  void showContextMenu(ProjectExplorer::Project* project,
                       ProjectExplorer::Node* node);
  void addSpecSimModel();
  void addExistingSpecSimModel();

private:
  Q_DISABLE_COPY(SpecSimPlugin)
  class Private;
  Private*  _d;
};

} // namespace specsim
