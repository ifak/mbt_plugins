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
#ifndef ISRDLEDITOR_combuildisrdlOUTLINEMODEL_HPP
#define ISRDLEDITOR_combuildisrdlOUTLINEMODEL_HPP

#include <dslparser/isrdl/combuildisrdlmodel.hpp>

namespace isrdleditor {

class IsrdlOutlineModel;

class ComBuildIsrdlOutlineModel
    : public dslparser::isrdl::ComBuildIsrdlModel
{
public:
  ComBuildIsrdlOutlineModel(const QString& isrdlDocText,
                             IsrdlOutlineModel* isrdlOutlineModel,
                             QString prefix = QString(""),
                             QObject* parent = 0);
  virtual ~ComBuildIsrdlOutlineModel();

public:
  bool execute(QString* errorString = 0);
};

} // namespace isrdleditor

#endif // ISRDLEDITOR_combuildisrdlOUTLINEMODEL_HPP
