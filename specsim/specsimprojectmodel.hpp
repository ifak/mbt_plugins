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

#include <QStandardItemModel>

namespace specsim {

class SpecSimProject;
class SpecSimModel;

class SPECSIMSHARED_EXPORT SpecSimProjectModel
    : public QStandardItemModel
{
  Q_OBJECT
public:
  explicit SpecSimProjectModel(SpecSimProject* specSimProject);
  virtual ~SpecSimProjectModel();

public:
  void reset();

public:
  void                  addSpecSimModel(SpecSimModel* specSimModel);
  void                  updateSpecSimModel(SpecSimModel* specSimModel);
  SpecSimModel*         specSimModel(const QString& specSimModelFilePath) const;
  QModelIndex           specSimModelIndexOf(const QString& specSimModelFilePath) const;
  SpecSimModel*         specSimModelForIndex(const QModelIndex& modelIndex) const;
  QList<SpecSimModel*>  specSimModels() const;

protected:
  Qt::DropActions supportedDropActions() const;

private:
  enum DataRole
  {
    SpecSimProjectItemRole = Qt::UserRole + 3
  };

  enum SpecSimProjectItem
  {
    TcgCoverageParentItem,
    TcgCoverageItem,
    TestSuiteItem
  };

private slots:
  void handleItemChange(QStandardItem* item);

private:
  Q_DISABLE_COPY(SpecSimProjectModel)
  class Private;
  Private*  _d;
};

} // namespace specsim
