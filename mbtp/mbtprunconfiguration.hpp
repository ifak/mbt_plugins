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
#ifndef MBTP_MBTPRUNCONFIGURATION_HPP
#define MBTP_MBTPRUNCONFIGURATION_HPP

#include "mbtp_global.hpp"

#include <projectexplorer/runconfiguration.h>
#include <QUuid>
#include <QIcon>

namespace mbtp {

class MBTPSHARED_EXPORT MbtpRunConfiguration
    : public ProjectExplorer::RunConfiguration
{
  Q_OBJECT

public:
  MbtpRunConfiguration(ProjectExplorer::Target* parent);
  virtual ~MbtpRunConfiguration();

public:
  virtual QWidget*  createConfigurationWidget();
  virtual QString   title() const;
  virtual bool      isEnabled() const;

public slots:
  void setEnabled(const bool enabled);

private:
  Q_DISABLE_COPY(MbtpRunConfiguration)
  class Private;
  Private* _d;
};

class MbtpConfigurationWidget : public QWidget
{
  Q_OBJECT
public:
  explicit MbtpConfigurationWidget(MbtpRunConfiguration* mbtpRunConfiguration,
                                   QWidget *parent = nullptr);

private:
  MbtpRunConfiguration* _mbtpRunConfiguration;
};

class MBTPSHARED_EXPORT MbtpRunControl
    : public ProjectExplorer::RunControl
{
  Q_OBJECT
public:
  MbtpRunControl(MbtpRunConfiguration* mbtpRunConfiguration,
                 ProjectExplorer::RunMode mode,
                 const QIcon& runControlIcon = QIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN_SMALL)));
  virtual ~MbtpRunControl();

public:
  virtual void        start();
  virtual StopResult  stop();
  void showResult();

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
  Q_DISABLE_COPY(MbtpRunControl)
  class Private;
  Private* _d;
};

class MBTPSHARED_EXPORT MbtpRunControlFactory
    : public ProjectExplorer::IRunControlFactory
{
  Q_OBJECT
public:
  MbtpRunControlFactory(QObject *parent = nullptr);
  ~MbtpRunControlFactory();

public:
  bool            canRun(ProjectExplorer::RunConfiguration* runConfiguration,
                         ProjectExplorer::RunMode mode) const;
  MbtpRunControl*  create(ProjectExplorer::RunConfiguration*runConfiguration,
                          ProjectExplorer::RunMode mode,
                          QString* errorMessage);

private:
  Q_DISABLE_COPY(MbtpRunControlFactory)
  class Private;
  Private* _d;
};


} // namespace mbtp

#endif // MBTP_MBTPRUNCONFIGURATION_HPP
