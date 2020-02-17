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
#ifndef IRDLEDITOR_combuildirdlOUTLINEMODEL_HPP
#define IRDLEDITOR_combuildirdlOUTLINEMODEL_HPP

#include <dslparser/irdl/combuildirdlmodel.hpp>
#include <dslparser/irdldecl/combuildirdldeclmodel.hpp>

namespace irdleditor {

class IrdlOutlineModel;

class ComBuildIrdlOutlineModel
    : public dslparser::irdl::ComBuildIrdlModel
{
public:
  ComBuildIrdlOutlineModel(const QString& irdlDocText,
                             IrdlOutlineModel* irdlOutlineModel,
                             QString prefix = QString(""),
                             QObject* parent = 0);
  virtual ~ComBuildIrdlOutlineModel();

public:
  bool execute(QString* errorString = 0, bool deleteOldModel = true);
};

class ComBuildIrdlDeclOutlineModel
    : public dslparser::irdl::ComBuildIrdlDeclModel
{
public:
  ComBuildIrdlDeclOutlineModel(const QString& irdlDocText,
                             IrdlOutlineModel* irdlOutlineModel,
                             QString prefix = QString(""),
                             QObject* parent = 0);
  virtual ~ComBuildIrdlDeclOutlineModel();

public:
  bool execute(QString* errorString = 0, bool deleteOldModel = true);
};

} // namespace irdleditor

#endif // IRDLEDITOR_combuildirdlOUTLINEMODEL_HPP
