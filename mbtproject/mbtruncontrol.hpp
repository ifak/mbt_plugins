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

#include <projectexplorer/runconfiguration.h>

#include <QUuid>
#include <QIcon>

namespace mbtproject {

class MbtRunConfiguration;

class MBTPROJECTSHARED_EXPORT MbtRunControl
    : public ProjectExplorer::RunControl
{
  Q_OBJECT
public:
  MbtRunControl(MbtRunConfiguration* mbtRunConfiguration,
                ProjectExplorer::RunMode mode,
                const QIcon& runControlIcon = QIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN_SMALL)));
  virtual ~MbtRunControl();

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
  Q_DISABLE_COPY(MbtRunControl)
  class Private;
  Private* _d;
};

class MBTPROJECTSHARED_EXPORT MbtRunControlFactory
    : public ProjectExplorer::IRunControlFactory
{
  Q_OBJECT
public:
  MbtRunControlFactory(QObject *parent = 0);
  virtual ~MbtRunControlFactory();

public:
  bool            canRun(ProjectExplorer::RunConfiguration* runConfiguration,
                         ProjectExplorer::RunMode mode) const;
  MbtRunControl*  create(ProjectExplorer::RunConfiguration*runConfiguration,
                         ProjectExplorer::RunMode mode,
                         QString* errorMessage);
private:
  Q_DISABLE_COPY(MbtRunControlFactory)
  class Private;
  Private* _d;
};

} // namespace mbtproject
