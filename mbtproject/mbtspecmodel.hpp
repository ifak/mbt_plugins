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

#include <dslparser/dslerror.hpp>

#include <projectexplorer/baseerror.hpp>

#include <QObject>

class QStandardItemModel;

namespace mbtproject {

class TcgCoverageProperty;

class MBTPROJECTSHARED_EXPORT MbtSpecModel
    : public QObject
{
public:
  enum SpecModelType
  {
    SpenatModel,
    StateMachineModel,
    UserModel
  };

protected:
  explicit MbtSpecModel(const QString& mbtSpecModelFilePath,
                        const QIcon& mbtSpecModelIcon,
                        QObject* parent = nullptr);

public:
  virtual ~MbtSpecModel();

public:
  virtual SpecModelType             specModelType() const = 0;
  virtual bool                      rebuildSpecModel(QString* errorMessage) =0;
  virtual QStandardItemModel const* specModel() const =0;
  virtual QString                   toJson() const =0;

public:
  const QList<ProjectExplorer::BaseError>&  errors() const;

public:
  QString         name() const;
  const QString&  mbtSpecModelFilePath() const;
  const QIcon&    icon() const;
  void            setEnabled(bool enabled);
  bool            isEnabled() const;

public:
  const QString&                      tcgInputFilePath() const;
  void                                setTcgInputFilePath(const QString& tcgInputFilePath);

  void                                clearTcgCoverageProperties();
  bool                                addTcgCoverageProperty(TcgCoverageProperty* tcgCoverageProperty,
                                                             QString* errorMessage);
  TcgCoverageProperty*                addTcgCoverageProperty(const QString& tcgCoverage);
  bool                                removeTcgCoverageProperty(const QString& tcgCoverage);
  TcgCoverageProperty*                tcgCoverageProperty(const QString& tcgCoverage) const;
  const QList<TcgCoverageProperty*>&  tcgCoverageProperties() const;
  bool                                hasTcgCoverageProperty(const QString& tcgCoverage);

protected:
  void clearErrors();
  void addDslError(const dslparser::DslError& dslError);

private:
  Q_DISABLE_COPY(MbtSpecModel)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
