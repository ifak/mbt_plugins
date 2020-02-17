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

#include <libs/modsim/modsim/combuildsimsystemmodel.hpp>

namespace specsim {

class SimSystemOutlineModel;

class ComBuildSimSystemOutlineModel
    : public modsim::ComBuildSimSystemModel
{
public:
  ComBuildSimSystemOutlineModel(const QString& simsystemDocText,
                             SimSystemOutlineModel* simSystemOutlineModel,const QString &praefix = QString(""),
                             QObject* parent = 0);
  virtual ~ComBuildSimSystemOutlineModel();

public:
  bool execute(QString* errorString = 0);

private:
  SimSystemOutlineModel* _outlineModel;
};

} // namespace simsystemeditor
