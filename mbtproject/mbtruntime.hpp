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

#include <QObject>
#include <QProcess>

namespace mbtproject {

class MbtProject;
class MbtSpecModel;

class MbtRuntime : public QObject
{
  Q_OBJECT

  enum MbtRuntimeState
  {
    Init,
    Prepared,
    Started,
    Stopped,
    Finished,
    Error
  };

public:
  explicit MbtRuntime(MbtProject* mbtProject,
                      QObject *parent = nullptr);
  virtual ~MbtRuntime();

private:
  void resetTcgProcesses();

public:
  void addMbtSpecModel(MbtSpecModel* mbtSpecModel);

public:
  bool prepare(QString* errorMessage);
  void start();
  void stop();

signals:
  void started(bool result,
               const QStringList& messages = QStringList());
  void stopped(bool result,
               const QStringList& messages = QStringList());
  void finished(bool result,
                const QStringList& messages = QStringList());
  void runtimeMessage(bool success,
                      const QString& message);

private slots:
  void tcgProcessStartedSlot();
  void tcgProcessFinishedSlot(int exitCode, QProcess::ExitStatus exitStatus);
  void tcgProcessErrorSlot(QProcess::ProcessError processError);
  void tcgProcessStandardOutputSlot();
  void tcgProcessStandardErrorSlot();

private:
  Q_DISABLE_COPY(MbtRuntime)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
