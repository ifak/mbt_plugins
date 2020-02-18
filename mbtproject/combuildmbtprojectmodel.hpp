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

#include <QObject>

namespace mbtproject {

class MbtProject;
class MbtProjectModel;

class MBTPROJECTSHARED_EXPORT ComBuildMbtProjectModel
    : public QObject
{
public:
  explicit ComBuildMbtProjectModel(MbtProject const* mbtProject,
                                   QObject* parent = nullptr);
  ~ComBuildMbtProjectModel();

public:
  bool execute(QString* errorMessage);

private:
  Q_DISABLE_COPY(ComBuildMbtProjectModel)
  class Private;
  Private* _d;
};

} // namespace mbtproject
