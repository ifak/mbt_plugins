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

#include <projectexplorer/basebuildstep.hpp>

#include "mbtproject.hpp"
#include "mbtbuildconfiguration.hpp"

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtBuildStep
    : public ProjectExplorer::BaseBuildStep
{
  Q_OBJECT

  friend class MbtBuildStepFactory;

public:
  MbtBuildStep(ProjectExplorer::BuildStepList* parentStepList);
  virtual ~MbtBuildStep();

public:
  virtual bool init();
  virtual void run(QFutureInterface<bool>& fi);

public:
  virtual ProjectExplorer::BuildStepConfigWidget* createConfigWidget();
  virtual bool runInGuiThread() const;

protected:
  MbtBuildStep(ProjectExplorer::BuildStepList* parentStepList,
               ProjectExplorer::BuildStep* step);

protected:
  MbtProject*             baseProject();
  MbtBuildConfiguration*  baseBuildConfiguration();

private:
  bool buildMbtRuntime(QString* errorMessage);
};

class MBTPROJECTSHARED_EXPORT MbtBuildStepFactory
    : public ProjectExplorer::BaseBuildStepFactory
{
  Q_OBJECT

public:
  explicit MbtBuildStepFactory(QObject *parent = 0);
  virtual ~MbtBuildStepFactory();

public:
  virtual ProjectExplorer::BuildStep* create(ProjectExplorer::BuildStepList* parentStepList,
                                             const Core::Id id);
  virtual ProjectExplorer::BuildStep* restore(ProjectExplorer::BuildStepList *parentStepList,
                                              const QVariantMap &map);

private:
  Q_DISABLE_COPY(MbtBuildStepFactory)
  class Private;
  Private* _d;
};

} // namespace mbtproject
