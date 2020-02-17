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

#include "specsim_global.hpp"

#include <projectexplorer/basebuildstep.hpp>

#include "specsimproject.hpp"
#include "specsimbuildconfiguration.hpp"

namespace specsim {

class SPECSIMSHARED_EXPORT SpecSimBuildStep
    : public ProjectExplorer::BaseBuildStep
{
  Q_OBJECT

  friend class SpecSimBuildStepFactory;

public:
  SpecSimBuildStep(ProjectExplorer::BuildStepList* parentStepList);
  virtual ~SpecSimBuildStep();

public:
  virtual bool init();
  virtual void run(QFutureInterface<bool>& fi);

public:
  virtual ProjectExplorer::BuildStepConfigWidget* createConfigWidget();
  virtual bool runInGuiThread() const;

protected:
  SpecSimBuildStep(ProjectExplorer::BuildStepList* parentStepList,
               ProjectExplorer::BuildStep* step);

protected:
  SpecSimProject*             baseProject();
  SpecSimBuildConfiguration*  baseBuildConfiguration();

private:
  bool buildSpecSimRuntime(QString* errorMessage);
};

class SPECSIMSHARED_EXPORT SpecSimBuildStepFactory
    : public ProjectExplorer::BaseBuildStepFactory
{
  Q_OBJECT

public:
  explicit SpecSimBuildStepFactory(QObject *parent = 0);
  virtual ~SpecSimBuildStepFactory();

public:
  virtual ProjectExplorer::BuildStep* create(ProjectExplorer::BuildStepList* parentStepList,
                                             const Core::Id id);
  virtual ProjectExplorer::BuildStep* restore(ProjectExplorer::BuildStepList *parentStepList,
                                              const QVariantMap &map);

private:
  Q_DISABLE_COPY(SpecSimBuildStepFactory)
  class Private;
  Private* _d;
};

} // namespace specsim
