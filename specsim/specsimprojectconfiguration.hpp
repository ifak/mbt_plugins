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

#include "specsimmodelsettings.hpp"

#include <QObject>
#include <QVariantMap>

namespace specsim
{

class SpecSimProject;

class SPECSIMSHARED_EXPORT SpecSimProjectConfiguration
    : public QObject
{
  Q_OBJECT

public:
  typedef QList<SpecSimModelSettings> SpecSimModelSettingsSet;
public:
  SpecSimProjectConfiguration(SpecSimProject* specSimProject);
  ~SpecSimProjectConfiguration();

public:
  void setUseGlobalSettings(bool use);
  bool useGlobalSettings() const;
  void cloneGlobalSettings();

public:
  SpecSimModelSettings*           specSimModelSettings(const QString& specSimModelFilePath) const;
  const SpecSimModelSettingsSet&  allSpecSimModelSettings() const;
  void                            addSpecSimModelSettings(const SpecSimModelSettings& newSpecSimModelSettings);
  void                            removeSpecSimModelSettings(const SpecSimModelSettings& specSimModelSettings);

public:
  QVariantMap toMap() const;
  void        fromMap(const QVariantMap &map);

private:
  Q_DISABLE_COPY(SpecSimProjectConfiguration)
  class Private;
  Private*  _d;
};

}// end namespace specsim
