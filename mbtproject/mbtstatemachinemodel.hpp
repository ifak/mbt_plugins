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

#include "mbtspecmodel.hpp"

#include <mobata/model/statemachine/statemachinemodel.hpp>

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtStateMachineModel
    : public MbtSpecModel
{
public:
  explicit MbtStateMachineModel(const QString& stateMachineFilePath,
                          QObject* parent = nullptr);
  virtual ~MbtStateMachineModel();

public:
  SpecModelType specModelType() const override {return StateMachineModel;}

public:
  bool                                          rebuildSpecModel(QString* errorMessage) override;
  model::statemachine::StateMachineModel const* specModel() const override;
  QString                                       toJson() const override;

private:
  Q_DISABLE_COPY(MbtStateMachineModel)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
