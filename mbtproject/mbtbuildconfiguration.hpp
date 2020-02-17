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

#include <projectexplorer/basebuildconfiguration.hpp>

namespace mbtproject {

class MbtRuntime;
class MbtBuildConfigurationFactory;

class MbtBuildConfiguration
    : public ProjectExplorer::BaseBuildConfiguration
{
  Q_OBJECT

  friend class MbtBuildConfigurationFactory;

public:
  explicit MbtBuildConfiguration(ProjectExplorer::Target* parent);
  virtual ~MbtBuildConfiguration();

protected:
  MbtBuildConfiguration(ProjectExplorer::Target* parent,
                        const Core::Id& buildId);

public:
  void        setMbtRuntime(MbtRuntime* mbtRuntime);
  MbtRuntime* mbtRuntime();

private:
  Q_DISABLE_COPY(MbtBuildConfiguration)
  class Private;
  Private* _d;
};

class MbtBuildConfigurationFactory
    : public ProjectExplorer::BaseBuildConfigurationFactory
{
  Q_OBJECT

public:
  explicit MbtBuildConfigurationFactory(QObject *parent = 0);
  virtual ~MbtBuildConfigurationFactory();

public:
  MbtBuildConfiguration* create(ProjectExplorer::Target* parent,
                                const Core::Id id,
                                const QString &name = QString());
  MbtBuildConfiguration* restore(ProjectExplorer::Target* parent,
                                 const QVariantMap &map);
};

} // namespace mbtproject
