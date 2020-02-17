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

#include "isrdleditor_global.hpp"

#include <mobata/model/isrdl/isrdlmodel.hpp>

#include <dslparser/dslparser_types.hpp>

namespace isrdleditor {

class ISRDLEDITOR_EXPORT IsrdlOutlineModel
    : public model::isrdl::IsrdlModel
{
    friend class ComBuildIsrdlOutlineModel;
    friend class ComBuildIsrdlDeclOutlineModel;

public:
  IsrdlOutlineModel(QObject* parent = 0);
  virtual ~IsrdlOutlineModel();

public:
  virtual void reset();

public:
  dslparser::TokenTextLocation const* modelTextLocation(const QModelIndex& modelIntex) const;

public:
  const dslparser::ModelTextLocations& modelTextSelections() const;
  const dslparser::TokenTextLocations& keywordTextSelections() const;

private:
  void setModelTextLocations(const dslparser::ModelTextLocations& modelTextLocations);
  void setKeywordTextLocations(const dslparser::TokenTextLocations& keywordTextLocations);

private:
  Q_DISABLE_COPY(IsrdlOutlineModel)
  class Private;
  Private* _d;
};

} // namespace isrdleditor
