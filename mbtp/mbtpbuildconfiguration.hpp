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
#ifndef MBTP_MBTPBUILDCONFIGURATION_HPP
#define MBTP_MBTPBUILDCONFIGURATION_HPP

#include <projectexplorer/basebuildconfiguration.hpp>

namespace mbtp {

class MbtpRuntime;
class MbtpBuildConfigurationFactory;

class MbtpBuildConfiguration
    : public ProjectExplorer::BaseBuildConfiguration
{
  Q_OBJECT
  friend class MbtpBuildConfigurationFactory;

public:
  explicit MbtpBuildConfiguration(ProjectExplorer::Target* parent);
  virtual ~MbtpBuildConfiguration();

protected:
  MbtpBuildConfiguration(ProjectExplorer::Target* target, const Core::Id id);

public:
  void           setMbtpRuntime(MbtpRuntime* mbtpRuntime);
  MbtpRuntime* mbtpRuntime();

private:
  Q_DISABLE_COPY(MbtpBuildConfiguration)
  class Private;
  Private* _d;
};

class MbtpBuildConfigurationFactory
    : public ProjectExplorer::BaseBuildConfigurationFactory
{
  Q_OBJECT

public:
  explicit MbtpBuildConfigurationFactory(QObject *parent = nullptr);
  virtual ~MbtpBuildConfigurationFactory();

public:
  MbtpBuildConfiguration* create(ProjectExplorer::Target* parent,
                                  const Core::Id id,
                                  const QString &name = QString());
  MbtpBuildConfiguration* restore(ProjectExplorer::Target* parent,
                                   const QVariantMap &map);
};

} // namespace mbtp

#endif // MBTP_MBTPBUILDCONFIGURATION_HPP
