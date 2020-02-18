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

#include <QSettings>

namespace specsim {

class SPECSIMSHARED_EXPORT SpecSimModelSettings
{
public:
  SpecSimModelSettings(const QString& specSimModelFilePath = QStringLiteral(""));
  SpecSimModelSettings(const SpecSimModelSettings& source);

public:
  const QString&      specSimModelFilePath() const;
  void                setSpecSimModelFilePath(const QString& specSimModelFilePath);
  bool                isEnabled() const;
  void                setEnabled(bool enabled);

public:
  QVariantMap toMap() const;
  void        fromMap(const QVariantMap& map);

private:
  QString     _specSimModelFilePath;
  bool        _enabled;
};

inline bool operator==(const SpecSimModelSettings &p1, const SpecSimModelSettings &p2)
{
    return ((p1.specSimModelFilePath().compare(p2.specSimModelFilePath(), Qt::CaseInsensitive) == 0)
            /*&& (p1.isEnabled() == p2.isEnabled())*/);
}

} // namespace specsim
