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

#include <QStandardItemModel>

namespace mbtproject {

class MbtProject;
class MbtSpecModel;

class MBTPROJECTSHARED_EXPORT MbtProjectModel
    : public QStandardItemModel
{
  Q_OBJECT
public:
  explicit MbtProjectModel(MbtProject* mbtProject);
  virtual ~MbtProjectModel();

public:
  void reset();

public:
  void                  addMbtSpecModel(MbtSpecModel* mbtSpecModel);
  void                  updateMbtSpecModel(MbtSpecModel* mbtSpecModel);
  MbtSpecModel*         mbtSpecModel(const QString& mbtSpecModelFilePath) const;
  QModelIndex           mbtSpecModelIndexOf(const QString& mbtSpecModelFilePath) const;
  MbtSpecModel*         mbtSpecModelForIndex(const QModelIndex& modelIndex) const;
  QList<MbtSpecModel*>  mbtSpecModels() const;

protected:
  Qt::DropActions supportedDropActions() const;

private:
  enum DataRole
  {
    MbtProjectItemRole = Qt::UserRole + 2
  };

  enum MbtProjectItem
  {
    TcgCoverageParentItem,
    TcgCoverageItem,
    TestSuiteItem
  };

private slots:
  void handleItemChange(QStandardItem* item);

private:
  Q_DISABLE_COPY(MbtProjectModel)
  class Private;
  Private*  _d;
};

} // namespace mbtproject
