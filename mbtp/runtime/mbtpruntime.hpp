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

#include "mbtp_global.hpp"

#include <QObject>
#include <QProcess>
#include <QTimer>

namespace mbtp {

class MbtpProject;

class MBTPSHARED_EXPORT MbtpRuntime
    : public QObject
{
    Q_OBJECT

  friend class ComBuildMbtpRuntime;

public:
  MbtpRuntime(MbtpProject* mbtpProject,
                   QObject* parent = nullptr);
  virtual ~MbtpRuntime();
  MbtpProject* project();

private:
  virtual void reset();

public:
  bool prepare(QString* errorMessage);
  virtual bool start();
  virtual bool stop();

private slots:
  void emitFinished(int exitCode);
  void emitError();

signals:
  void started(bool result,
               const QStringList& messages = QStringList());
  void stopped(bool result,
               const QStringList& messages = QStringList());
  void finished(bool result,
                const QStringList& messages = QStringList());
  void runtimeMessage(bool success,
                      const QString& message);

private:
  QProcess* _process;
  QTimer* _timer;
  Q_DISABLE_COPY(MbtpRuntime)
  class Private;
  Private* _d;

};

} // namespace mbtp
