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

#include "specsim_global.hpp"

#include <dslparser/dslerror.hpp>

#include <projectexplorer/baseerror.hpp>

#include <QObject>

#include <mobata/model/spenat/spenatdeclmodel.hpp>
#include <mobata/model/statemachine/statemachinemodel.hpp>

class QStandardItemModel;

namespace specsim {

class SPECSIMSHARED_EXPORT SpecSimModel
    : public QObject
{
public:
  enum SpecSimModelType
  {
    SpenatModel,
    StateMachineModel,
    UserModel
  };

protected:
  explicit SpecSimModel(const QString& specSimModelFilePath,
                        const QIcon& specSimModelIcon,
                        QObject* parent = nullptr);

public:
  virtual ~SpecSimModel();

public:
  virtual SpecSimModelType          specModelType() const = 0;
  virtual bool                      rebuildSpecModel(QString* errorMessage) =0;
  virtual QStandardItemModel const* specModel() const =0;
  virtual QString                   toJson() const =0;

public:
  const QList<ProjectExplorer::BaseError>&  errors() const;

public:
  QString         name() const;
  const QString&  specSimModelFilePath() const;
  const QIcon&    icon() const;
  void            setEnabled(bool enabled);
  bool            isEnabled() const;

protected:
  void clearErrors();
  void addDslError(const dslparser::DslError& dslError);

private:
  Q_DISABLE_COPY(SpecSimModel)
  class Private;
  Private*  _d;
};

class SPECSIMSHARED_EXPORT SpecSimSpenatModel
    : public SpecSimModel
{
public:
  explicit SpecSimSpenatModel(const QString& spenatFilePath,
                              QObject* parent = nullptr);
  virtual ~SpecSimSpenatModel();

public:
  SpecSimModelType specModelType() const override {return SpenatModel;}

public:
  model::spenat::SpenatDeclModel const* specModel() const override;
  bool                                  rebuildSpecModel(QString* errorMessage) override;
  QString                               toJson() const override;

private:
  Q_DISABLE_COPY(SpecSimSpenatModel)
  class Private;
  Private*  _d;
};

class SPECSIMSHARED_EXPORT SpecSimStateMachineModel
    : public SpecSimModel
{
public:
  explicit SpecSimStateMachineModel(const QString& stateMachineFilePath,
                              QObject* parent = nullptr);
  virtual ~SpecSimStateMachineModel();

public:
  SpecSimModelType specModelType() const override {return StateMachineModel;}

public:
  model::statemachine::StateMachineModel  const*  specModel() const override;
  bool                                            rebuildSpecModel(QString* errorMessage) override;
  QString                                         toJson() const override;

private:
  Q_DISABLE_COPY(SpecSimStateMachineModel)
  class Private;
  Private*  _d;
};

} // namespace specsim
