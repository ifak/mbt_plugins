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

#include "mbtspecmodelsettings.hpp"
//#include "mbtspenatsettings.hpp"

#include <QObject>
#include <QVariantMap>

namespace mbtproject
{

class MbtProject;

class MBTPROJECTSHARED_EXPORT MbtProjectConfiguration
    : public QObject
{
  Q_OBJECT
public:
  MbtProjectConfiguration(MbtProject* mbtProject);
  ~MbtProjectConfiguration();

public:
  void setUseGlobalSettings(bool use);
  bool useGlobalSettings() const;
  void cloneGlobalSettings();

public:
  MbtSpecModelSettings*               mbtSpecModelSettings(const QString& mbtSpecModelFilePath) const;
  const QList<MbtSpecModelSettings>&  allMbtSpecModelSettings() const;
  void                                addMbtSpecModelSettings(const MbtSpecModelSettings& newMbtSpecModelSettings);
  void                                removeMbtSpecModelSettings(const MbtSpecModelSettings& mbtSpecModelSettings);

public:
  QVariantMap toMap() const;
  void        fromMap(const QVariantMap &map);

private:
  Q_DISABLE_COPY(MbtProjectConfiguration)
  class Private;
  Private*  _d;
};

}// end namespace mbtproject
