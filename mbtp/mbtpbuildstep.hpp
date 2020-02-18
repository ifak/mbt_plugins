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
#ifndef MBTP_MBTPBUILDSTEP_HPP
#define MBTP_MBTPBUILDSTEP_HPP

#include "mbtp_global.hpp"

#include <projectexplorer/basebuildstep.hpp>
#include "mbtpproject.hpp"
#include "mbtpbuildconfiguration.hpp"

namespace mbtp {

class MBTPSHARED_EXPORT MbtpBuildStep : public ProjectExplorer::BaseBuildStep
{
  Q_OBJECT

  friend class MbtpBuildStepFactory;

public:
  MbtpBuildStep(ProjectExplorer::BuildStepList* parentStepList);
  virtual ~MbtpBuildStep();

public:
  virtual bool init();
  virtual void run(QFutureInterface<bool> &fi);

public:
  virtual ProjectExplorer::BuildStepConfigWidget* createConfigWidget();
  virtual bool runInGuiThread() const;

protected:
  MbtpBuildStep(ProjectExplorer::BuildStepList* parentStepList,
                ProjectExplorer::BuildStep* step);

protected:
  MbtpProject*             baseProject();
  MbtpBuildConfiguration*  mbtpBuildConfiguration();

private:
  bool buildMbtpTestRuntime(QString *errorMessage);

};

class MBTPSHARED_EXPORT MbtpBuildStepFactory
    : public ProjectExplorer::BaseBuildStepFactory
{
  Q_OBJECT

public:
  explicit MbtpBuildStepFactory(QObject *parent = nullptr);
  virtual ~MbtpBuildStepFactory();

public:
  virtual ProjectExplorer::BuildStep* create(ProjectExplorer::BuildStepList* parentStepList,
                                             const Core::Id id);
  virtual ProjectExplorer::BuildStep* restore(ProjectExplorer::BuildStepList *parentStepList,
                                              const QVariantMap &map);

private:
  Q_DISABLE_COPY(MbtpBuildStepFactory)
  class Private;
  Private* _d;
};

} // namespace mbtp

#endif // MBTP_MBTPBUILDSTEP_HPP
