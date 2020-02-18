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

namespace specsim {

class SpecSimMode : public Core::IMode
{
public:
  SpecSimMode();
};

class SpecSimMainModeWidgetPrivate;

class SpecSimMainModeWidget
    : public ProjectExplorer::BaseMainModeWidget
{
  Q_OBJECT

public:
  SpecSimMainModeWidget(SpecSimMode* specSimMode);
  virtual ~SpecSimMainModeWidget();

private slots:
    void showViewsMenu();

private:
  Q_DISABLE_COPY(SpecSimMainModeWidget)
  SpecSimMainModeWidgetPrivate* _d;
};

class SpecSimModeWidget
    : public ProjectExplorer::BaseModeWidget
{
  Q_OBJECT

public:
  SpecSimModeWidget(SpecSimMode* specSimMode,
                    SpecSimMainModeWidget* specSimMainModeWidget);
};

} // namespace specsim
