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

#include "irdleditor_global.hpp"

#include <mobata/model/requirement/requirementsmodel.hpp>
#include <mobata/model/requirement/requirementitem.hpp>
#include <mobata/model/requirement/req.hpp>

#include <dslparser/dslparser_types.hpp>

namespace irdleditor {

class IRDLEDITOR_EXPORT IrdlOutlineModel
    : public model::irdl::RequirementsModel
{
    friend class ComBuildIrdlOutlineModel;
    friend class ComBuildIrdlDeclOutlineModel;

public:
  IrdlOutlineModel(QObject* parent = 0);
  virtual ~IrdlOutlineModel();

public:
  virtual void reset();

public:
  dslparser::TokenTextLocation const* modelTextLocation(const QModelIndex& modelIntex) const;

public:
  const dslparser::ModelTextLocations& modelTextSelections() const;
  const dslparser::TokenTextLocations& keywordTextSelections() const;
  const QModelIndex uuidToIndex(const QUuid& uuid) const;
private:
  void setModelTextLocations(const dslparser::ModelTextLocations& modelTextLocations);
  void setKeywordTextLocations(const dslparser::TokenTextLocations& keywordTextLocations);
  const QModelIndex uuidToIndexRec(const QUuid& uuid, QList<model::msc::MscSequenceItem const*> itemList) const;
private:
  Q_DISABLE_COPY(IrdlOutlineModel)
  class Private;
  Private* _d;
};

} // namespace irdleditor
