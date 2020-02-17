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
#ifndef MBTP_COMBUILDMBTPTESTRUNTIME_HPP
#define MBTP_COMBUILDMBTPTESTRUNTIME_HPP

#include "mbtp_global.hpp"

#include <QDir>
#include <QObject>

namespace mbtp {

class MbtpProject;
class MbtpTestRuntime;

class MBTPSHARED_EXPORT ComBuildMbtpTestRuntime : public QObject
{
public:
  ComBuildMbtpTestRuntime(MbtpProject* mbtpProject,
                           MbtpTestRuntime* mbtpTestRuntime,
                           const QDir& buildDir,
                           QObject* parent = nullptr);
  virtual ~ComBuildMbtpTestRuntime();

public:
  bool execute(QString* errorString);

private:
  Q_DISABLE_COPY(ComBuildMbtpTestRuntime)
  class Private;
  Private* _d;
};

} // namespace mbtp

#endif // MBTP_COMBUILDMBTPTESTRUNTIME_HPP
