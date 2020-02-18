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
#ifndef MBTP_MBTPPROJECT_HPP
#define MBTP_MBTPPROJECT_HPP

#include "mbtp_global.hpp"

#include <projectexplorer/baseproject.hpp>

#include <QVariantMap>

namespace Core{
class IEditor;
}

namespace mbtp {

class MbtpProjectManager;

class MBTPSHARED_EXPORT MbtpProject
    : public ProjectExplorer::BaseProject
{
  Q_OBJECT
public:
  explicit MbtpProject(MbtpProjectManager* projectManager,
                         const QString &fileName);
  virtual ~MbtpProject();

public:
  bool supportsKit(ProjectExplorer::Kit *kit,
                   QString *errorMessage = nullptr) const;
  bool needsConfiguration() const;
  bool supportsNoTargetPanel() const;
  QVariantMap *settings();
  void setResult(QString result, QImage image);

  QImage resultImage();
  QString resultString();

protected:
  virtual bool        fromMap(const QVariantMap &map);
  virtual QVariantMap toMap() const;

private:
  Q_DISABLE_COPY(MbtpProject)
  QVariantMap _settingsMap;
  class Private;
  Private* _d;
};

class MBTPSHARED_EXPORT MbtpProjectManager
    : public ProjectExplorer::BaseProjectManager
{
  Q_OBJECT
public:
  explicit MbtpProjectManager();
  virtual ~MbtpProjectManager();

protected:
  MbtpProject* createProject(const QString &fileName);
};

} // namespace mbtp

#endif // MBTP_MBTPPROJECT_HPP
