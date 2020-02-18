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

#include <projectexplorer/basemodewidget.hpp>

namespace mbtp {

class MbtpMode : public Core::IMode
{
public:
  MbtpMode();
};

class MbtpMainModeWidgetPrivate;

class MbtpMainModeWidget
    : public ProjectExplorer::BaseMainModeWidget
{
  Q_OBJECT

public:
  MbtpMainModeWidget(MbtpMode* mbtpMode);
  virtual ~MbtpMainModeWidget();

  //private slots:
  //    void showViewsMenu();

private:
  Q_DISABLE_COPY(MbtpMainModeWidget)
  MbtpMainModeWidgetPrivate* _d;
};

class MbtpModeWidget
    : public ProjectExplorer::BaseModeWidget
{
  Q_OBJECT

public:
  MbtpModeWidget(MbtpMode* mbtpMode,
                   MbtpMainModeWidget* mbtpMainModeWidget);
};

} // namespace mbtp
