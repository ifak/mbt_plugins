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
#ifndef SPENATEDITOR_COMBUILDSPENATOUTLINEMODEL_HPP
#define SPENATEDITOR_COMBUILDSPENATOUTLINEMODEL_HPP

#include <dslparser/spenat/combuildspenatmodel.hpp>

namespace spenateditor {

class SpenatOutlineModel;

class ComBuildSpenatOutlineModel
    : public dslparser::spenat::ComBuildSpenatModel
{
public:
  ComBuildSpenatOutlineModel(const QString& spenatDocText,
                             SpenatOutlineModel* spenatOutlineModel,
                             QObject* parent = 0);
  virtual ~ComBuildSpenatOutlineModel();

public:
  bool execute(QString* errorString = 0);
};

} // namespace spenateditor

#endif // SPENATEDITOR_COMBUILDSPENATOUTLINEMODEL_HPP
