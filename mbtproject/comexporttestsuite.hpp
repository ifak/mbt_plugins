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

#include "mbtproject_global.hpp"

#include <QObject>
#include <QIcon>

namespace model{ namespace ts{
class TestSuiteItem;
}}

namespace mbtproject {

class MbtSpecModel;

class MBTPROJECTSHARED_EXPORT ComExportTestSuite
    : public QObject
{
  Q_OBJECT

public:
  ComExportTestSuite(const QString& title,
                     const QIcon& icon = QIcon(QLatin1String(":/mobata/images/ts/testsuite.png")),
                     QObject* parent = nullptr);
  virtual ~ComExportTestSuite();

public:
  virtual bool exportTestSuite(model::ts::TestSuiteItem const* testSuiteItem,
                               MbtSpecModel const* mbtSpecModel,
                               QString* errorString) =0;

public:
  const QString&  title() const;
  void            setTitle(const QString& title);
  const QIcon&    icon() const;
  void            setIcon(const QIcon& icon);

private:
  Q_DISABLE_COPY(ComExportTestSuite)
  class Private;
  Private* _d;
};

} // namespace mbtproject
