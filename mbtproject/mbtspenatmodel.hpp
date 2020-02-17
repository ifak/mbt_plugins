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

#include <mobata/model/spenat/spenatdeclmodel.hpp>

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtSpenatModel
    : public MbtSpecModel
{
public:
  explicit MbtSpenatModel(const QString& spenatFilePath,
                          QObject* parent = nullptr);
  virtual ~MbtSpenatModel();

public:
  SpecModelType specModelType() const override {return SpenatModel;}

public:
  model::spenat::SpenatDeclModel const* specModel() const override;
  bool                            rebuildSpecModel(QString* errorMessage) override;
  QString                         toJson() const override;

private:
  Q_DISABLE_COPY(MbtSpenatModel)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
