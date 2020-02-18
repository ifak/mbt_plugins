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

#include "statemachineeditor_global.hpp"

#include <graphlayout/graphquickwidget.hpp>
#include <QUuid>

class QToolBar;

namespace statemachineeditor {

class StateMachineGraphicEditor;

class STATEMACHINEEDITOR_EXPORT StateMachineGraphicWidget
    : public graphlayout::GraphQuickWidget
{
  Q_OBJECT

public:
  StateMachineGraphicWidget(QWidget *parent = 0);
  virtual ~StateMachineGraphicWidget();

public slots:
  void updateStateMachineGraphic(const QString &layoutFilepath);
  void updateStateMachineGraphic();

public:
  QToolBar*                   toolBar();
  StateMachineGraphicEditor*  stateMachineGraphicEditor() const;

private:
  void createToolBar();

private:
  Q_DISABLE_COPY(StateMachineGraphicWidget)
  class Private;
  Private*  _d;
};

} // namespace statemachineeditor
