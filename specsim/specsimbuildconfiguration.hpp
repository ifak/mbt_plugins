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

#include <projectexplorer/basebuildconfiguration.hpp>

namespace specsim {

class SpecSimRuntime;
class SpecSimBuildConfigurationFactory;

class SpecSimBuildConfiguration
    : public ProjectExplorer::BaseBuildConfiguration
{
  Q_OBJECT

  friend class SpecSimBuildConfigurationFactory;

public:
  explicit SpecSimBuildConfiguration(ProjectExplorer::Target* parent);
  virtual ~SpecSimBuildConfiguration();

protected:
  SpecSimBuildConfiguration(ProjectExplorer::Target* parent,
                            const Core::Id& buildId);

public:
  void            setSpecSimRuntime(SpecSimRuntime* specSimRuntime);
  SpecSimRuntime* specSimRuntime();

private:
  Q_DISABLE_COPY(SpecSimBuildConfiguration)
  class Private;
  Private* _d;
};

class SpecSimBuildConfigurationFactory
    : public ProjectExplorer::BaseBuildConfigurationFactory
{
  Q_OBJECT

public:
  explicit SpecSimBuildConfigurationFactory(QObject *parent = 0);
  virtual ~SpecSimBuildConfigurationFactory();

public:
  SpecSimBuildConfiguration* create(ProjectExplorer::Target* parent,
                                    const Core::Id id,
                                    const QString &name = QString());
  SpecSimBuildConfiguration* restore(ProjectExplorer::Target* parent,
                                     const QVariantMap &map);
};

} // namespace specsim
