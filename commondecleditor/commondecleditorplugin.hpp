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

#include "commondecleditor_global.hpp"

#include <extensionsystem/iplugin.h>

namespace Core{
class IEditor;
}

namespace commondecleditor{

class CommonDeclEditorWidget;
class CommonDeclGraphicWidget;

namespace Internal{

class CommonDeclEditorPlugin
    : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "commondecleditor.json")

public:
  CommonDeclEditorPlugin();
  ~CommonDeclEditorPlugin();

public:
  static CommonDeclEditorPlugin* instance() { return _instance; }

public:
  bool          initialize(const QStringList &arguments,
                           QString *errorString);
  void          extensionsInitialized();

  ShutdownFlag  aboutToShutdown();

public:
  const QString&  defaultStyleSheets() const;
  void            initializeEditor(CommonDeclEditorWidget* editorWidget);
  void            initializeGraphicEditor(CommonDeclGraphicWidget* editorWidget);

private:
  static CommonDeclEditorPlugin* _instance;

private:
  Q_DISABLE_COPY(CommonDeclEditorPlugin)
  class Private;
  Private*  _d;
};

} // namespace Internal
} // namespace commondecleditor
