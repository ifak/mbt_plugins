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

namespace specsim {

class SpecSimProject;
class SpecSimModel;

class SpecSimRuntime : public QObject
{
  Q_OBJECT

  enum SpecSimRuntimeState
  {
    Init,
    Prepared,
    Started,
    Stopped,
    Finished,
    Error
  };

public:
  explicit SpecSimRuntime(SpecSimProject* specSimProject,
                          QObject *parent = nullptr);
  virtual ~SpecSimRuntime();

private:
  void resetTcgProcesses();

public:
  void addSpecSimModel(SpecSimModel* specSimModel);
  void addLink(const QString& sourceModel, const QString& sourcePort,
               const QString& targetModel, const QString& targetPort);

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
  void runtimeStartedSlot(bool result,
                          const QStringList& messages = QStringList());
  void runtimeFinishedSlot(bool result,
                           const QStringList& messages = QStringList());

private:
  Q_DISABLE_COPY(SpecSimRuntime)
  class Private;
  Private*  _d;
};

} // namespace specsim
