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

#include "mbtproject_global.hpp"

#include <projectexplorer/runconfiguration.h>

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtRunConfiguration
    : public ProjectExplorer::RunConfiguration
{
  Q_OBJECT

public:
  MbtRunConfiguration(ProjectExplorer::Target* parent);
  virtual ~MbtRunConfiguration();

public:
  virtual QWidget*  createConfigurationWidget();
  virtual QString   title() const;
  virtual bool      isEnabled() const;

public slots:
  void setEnabled(const bool enabled);

private:
  Q_DISABLE_COPY(MbtRunConfiguration)
  class Private;
  Private* _d;
};

} // namespace mbtproject
