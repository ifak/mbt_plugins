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

#include <projectexplorer/runconfiguration.h>

#include <QUuid>
#include <QIcon>

namespace specsim {

class SpecSimRunConfiguration;

class SPECSIMSHARED_EXPORT SpecSimRunControl
    : public ProjectExplorer::RunControl
{
  Q_OBJECT
public:
  SpecSimRunControl(SpecSimRunConfiguration* specSimRunConfiguration,
                    ProjectExplorer::RunMode mode,
                    const QIcon& runControlIcon = QIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN_SMALL)));
  virtual ~SpecSimRunControl();

public:
  virtual void        start();
  virtual StopResult  stop();

public:
  bool  isRunning() const;
  QIcon icon() const;

protected slots:
  void startedSlot(bool result,
                   const QStringList& messages = QStringList());
  void stoppedSlot(bool result,
                   const QStringList& messages = QStringList());
  void finishedSlot(bool result,
                    const QStringList& messages = QStringList());
  void runtimeMessageSlot(bool success,
                          const QString& message);

private:
  Q_DISABLE_COPY(SpecSimRunControl)
  class Private;
  Private* _d;
};

class SPECSIMSHARED_EXPORT SpecSimRunControlFactory
    : public ProjectExplorer::IRunControlFactory
{
  Q_OBJECT
public:
  SpecSimRunControlFactory(QObject *parent = 0);
  virtual ~SpecSimRunControlFactory();

public:
  bool                canRun(ProjectExplorer::RunConfiguration* runConfiguration,
                             ProjectExplorer::RunMode mode) const;
  SpecSimRunControl*  create(ProjectExplorer::RunConfiguration*runConfiguration,
                             ProjectExplorer::RunMode mode,
                             QString* errorMessage);
private:
  Q_DISABLE_COPY(SpecSimRunControlFactory)
  class Private;
  Private* _d;
};

} // namespace specsim
