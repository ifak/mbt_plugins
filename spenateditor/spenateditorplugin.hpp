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

#include "spenateditor_global.hpp"

#include <extensionsystem/iplugin.h>

namespace Core{
class IEditor;
}

namespace spenateditor{

class SpenatEditorWidget;
class SpenatGraphicWidget;

namespace Internal{

class SpenatEditorPlugin
    : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "spenateditor.json")

public:
  SpenatEditorPlugin();
  ~SpenatEditorPlugin();

public:
  static SpenatEditorPlugin* instance() { return _instance; }

public:
  bool          initialize(const QStringList &arguments,
                           QString *errorString);
  void          extensionsInitialized();

  ShutdownFlag  aboutToShutdown();
    private slots:
  void appliedSettings();
signals:
  void refreshGraphic();
public:
  const QString&  defaultStyleSheets() const;
  void            initializeEditor(SpenatEditorWidget* editorWidget);
  void            initializeGraphicEditor(SpenatGraphicWidget* editorWidget);

private:
  static SpenatEditorPlugin* _instance;

private:
  Q_DISABLE_COPY(SpenatEditorPlugin)
  class Private;
  Private*  _d;
};

} // namespace Internal
} // namespace spenateditor
