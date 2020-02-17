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

#include <QSettings>

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtSpecModelSettings
{
public:
  MbtSpecModelSettings(const QString& mbtSpecModelFilePath = QStringLiteral(""));
  MbtSpecModelSettings(const MbtSpecModelSettings& source);

public:
  const QString&      mbtSpecModelFilePath() const;
  void                setMbtSpecModelFilePath(const QString& mbtSpecModelFilePath);
  bool                isEnabled() const;
  void                setEnabled(bool enabled);

  void                setTcgCoverages(const QStringList& tcgCoverages);
  const QStringList&  tcgCoverages() const;

  bool                addTcgCoverage(const QString& tcgCoverage);
  bool                removeTcgCoverage(const QString& tcgCoverage);

public:
  QVariantMap toMap() const;
  void        fromMap(const QVariantMap& map);

private:
  QString     _mbtSpecModelFilePath;
  bool        _enabled;
  QStringList _tcgCoverages;
};

inline bool operator==(const MbtSpecModelSettings &p1, const MbtSpecModelSettings &p2)
{
    return ((p1.mbtSpecModelFilePath().compare(p2.mbtSpecModelFilePath(), Qt::CaseInsensitive) == 0)
            /*&& (p1.isEnabled() == p2.isEnabled())*/);
}

} // namespace mbtproject
