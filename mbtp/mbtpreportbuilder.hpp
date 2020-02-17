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
#include <QQuickImageProvider>
#include <runtime/mbtpruntime.hpp>

namespace mbtp {

class MbtpTableModel;

class MbtpReportBuilder : public QObject
{
  Q_OBJECT
public:
  MbtpReportBuilder(QObject* parent = nullptr);
  ~MbtpReportBuilder();

private:
  void reset();

public:
  void buildReport(MbtpRuntime* mbtpRuntime,
                   QString* errorString = nullptr);

public:
  Q_INVOKABLE int getSucceededCount();
  Q_INVOKABLE int getFailedCount();
  Q_INVOKABLE int getExcludetCount();
  Q_INVOKABLE int getNotExecutedCount();

  MbtpTableModel* mbtpRuntimeModel();


private:
  int _succeededCount;
  int _failedCount;
  int _excludetCount;
  int _notExecuedCount;

  MbtpTableModel* _mbtpRuntimeModel;
};

class MbtpReportImageProvider : public QQuickImageProvider
{
public:
  MbtpReportImageProvider();
  QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
  void setImage(QImage image);
private:
  QImage _image;
};

} // mbtp
