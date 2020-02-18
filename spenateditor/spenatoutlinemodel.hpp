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

#include "spenateditor_global.hpp"

#include <mobata/model/spenat/spenatdeclmodel.hpp>

#include <dslparser/dslparser_types.hpp>

namespace spenateditor {

class SPENATEDITOR_EXPORT SpenatOutlineModel
    : public model::spenat::SpenatDeclModel
{
  friend class ComBuildSpenatOutlineModel;

public:
  typedef QSharedPointer<SpenatOutlineModel> Ptr;

public:
  SpenatOutlineModel(QObject* parent = 0);
  virtual ~SpenatOutlineModel();

public:
  virtual void reset();

public:
  dslparser::TokenTextLocation const* modelTextLocation(const QModelIndex& modelIntex) const;

public:
  const dslparser::ModelTextLocations& modelTextSelections() const;
  const dslparser::TokenTextLocations& keywordTextSelections() const;

public:
 model::spenat::PlaceItem const*      placeFromIndex(const QModelIndex& modelIndex) const;
 model::spenat::TransitionItem const* transitionFromIndex(const QModelIndex& modelIndex) const;
 const QModelIndex uuidToIndex(const QUuid& uuid) const;

private:
  void setModelTextLocations(const dslparser::ModelTextLocations& modelTextLocations);
  void setKeywordTextLocations(const dslparser::TokenTextLocations& keywordTextLocations);

private:
  Q_DISABLE_COPY(SpenatOutlineModel)
  class Private;
  Private* _d;
};

typedef SpenatOutlineModel::Ptr SpenatOutlineModelPtr;

} // namespace spenateditor
