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

#include "irdleditor_global.hpp"

#include <extensionsystem/iplugin.h>

namespace Core{
class IEditor;
}

namespace irdleditor{

class IrdlEditorWidget;
class IrdlGraphicWidget;

namespace Internal{

class IrdlEditorPlugin
    : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "irdleditor.json")

public:
  IrdlEditorPlugin();
  ~IrdlEditorPlugin();

public:
  static IrdlEditorPlugin* instance() { return _instance; }

public:
  bool          initialize(const QStringList &arguments,
                           QString *errorString);
  void          extensionsInitialized();

  ShutdownFlag  aboutToShutdown();

public:
  const QString&  defaultStyleSheets() const;
  void            initializeEditor(IrdlEditorWidget* editorWidget);
  void            initializeGraphicEditor(IrdlGraphicWidget* editorWidget);

private:
  static IrdlEditorPlugin* _instance;

private:
  Q_DISABLE_COPY(IrdlEditorPlugin)
  class Private;
  Private*  _d;
};

} // namespace Internal
} // namespace irdleditor
