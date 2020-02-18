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

namespace mbtproject {

class MbtMode : public Core::IMode
{
public:
  MbtMode();
};

class MbtMainModeWidgetPrivate;

class MbtMainModeWidget
     : public ProjectExplorer::BaseMainModeWidget
{
  Q_OBJECT

public:
  MbtMainModeWidget(MbtMode* mbtMode);
  virtual ~MbtMainModeWidget();

//private slots:
//    void showViewsMenu();

private:
  Q_DISABLE_COPY(MbtMainModeWidget)
  MbtMainModeWidgetPrivate* _d;
};

class MbtModeWidget
    : public ProjectExplorer::BaseModeWidget
{
  Q_OBJECT

public:
  MbtModeWidget(MbtMode* mbtMode,
                MbtMainModeWidget* mbtMainModeWidget);
};

} // namespace mbtproject
