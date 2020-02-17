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

namespace model{ namespace ts{
class TestSuiteItem;
}}


namespace mbtproject {

class MbtSpecModel;

class MBTPROJECTSHARED_EXPORT TcgCoverageProperty
    : public QObject
{
public:
  TcgCoverageProperty(const QString& tcgCoverage,
                      MbtSpecModel* mbtSpecModel);
  virtual ~TcgCoverageProperty();

public:
  MbtSpecModel*             mbtSpecModel() const;

public:
  void                      setTcgCoverage(const QString& tcgCoverage);
  const QString&            tcgCoverage() const;

  void                      setTcgOutputFilePath(const QString& tcgOutputFilePath);
  const QString&            tcgOutputFilePath() const;

  void                      setTcgTestSuiteItem(model::ts::TestSuiteItem* testSuiteItem);
  model::ts::TestSuiteItem* tcgTestSuiteItem() const;

private:
  Q_DISABLE_COPY(TcgCoverageProperty)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
