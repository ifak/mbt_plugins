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

#include <projectexplorer/baseproject.hpp>

#include <QMenu>

namespace Core{
class IEditor;
}

namespace specsim {

class SpecSimProjectModel;
class SpecSimProjectConfiguration;

class SPECSIMSHARED_EXPORT SpecSimProject
    : public ProjectExplorer::BaseProject
{
  Q_OBJECT

public:
  explicit SpecSimProject(ProjectExplorer::BaseProjectManager *projectManager,
                          const QString& fileName);
  virtual ~SpecSimProject();

public:
  SpecSimProjectConfiguration*        specSimProjectConfiguration();
  SpecSimProjectConfiguration const*  specSimProjectConfiguration() const;

public:
  bool        supportsKit(ProjectExplorer::Kit *kit,
                          QString *errorMessage = 0) const;
  bool        needsConfiguration() const;
  bool        supportsNoTargetPanel() const;
  bool        reload(QString* errorString);
  QStringList files(FilesMode fileMode) const;

public:
  SpecSimProjectModel const*  specSimProjectModel() const;
  SpecSimProjectModel*        specSimProjectModel();
  const QStringList&          declFiles() const;
  const QStringList&          specSimModelFiles() const;
  quint16                     specSimModelCount() const;
  bool                        addSpecSimModelFile(const QString& specSimModelFileName,
                                                  QString* errorMessage);

public:
  static void addDslError(const dslparser::DslError& newDslError,
                          const QString& errorFileName,
                          QList<ProjectExplorer::BaseError>* baseErrorList);

protected:
  virtual bool        fromMap(const QVariantMap &map);
  virtual QVariantMap toMap() const;

private:
  void  reset();

private slots:
  void specSimModelFileRemoved(const QString& specSimModelFileName);

private:
  Q_DISABLE_COPY(SpecSimProject)
  class Private;
  Private*  _d;
};

class SPECSIMSHARED_EXPORT SpecSimProjectManager
    : public ProjectExplorer::BaseProjectManager
{
  Q_OBJECT
public:
  explicit SpecSimProjectManager();
  virtual ~SpecSimProjectManager();

protected:
  SpecSimProject* createProject(const QString& fileName);
};

} // namespace specsim
