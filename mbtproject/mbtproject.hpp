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

#include "mbtproject_global.hpp"

#include <projectexplorer/baseproject.hpp>

#include <mobata/model/ts/testcaseitem.hpp>
#include <mobata/model/ts/testsuiteitem.hpp>

#include <dslparser/dslerror.hpp>

#include <QMenu>

namespace Core{
class IEditor;
}

namespace mbtproject {

class MbtSpecModel;
class MbtProjectModel;
class MbtProjectConfiguration;
class TcgCoverageProperty;
class ComExportTestSuite;

class ExportTestSuiteMenu : public QMenu
{
public:
  ExportTestSuiteMenu(const QString& title)
    : QMenu(title), _testSuiteItem(nullptr), _mbtSpecModel(nullptr)
  {}

public:
  void  setExportTestSuiteItem(model::ts::TestSuiteItem const* testSuiteItem)
  {this->_testSuiteItem=testSuiteItem;}

  model::ts::TestSuiteItem const* exportTestSuiteItem() const
  {return this->_testSuiteItem;}

  void  setMbtSpecModel(MbtSpecModel const* mbtSpecModel)
  {this->_mbtSpecModel=mbtSpecModel;}

  MbtSpecModel const* mbtSpecModel() const
  {return this->_mbtSpecModel;}

private:
  model::ts::TestSuiteItem const* _testSuiteItem;
  MbtSpecModel const*             _mbtSpecModel;
};

class MBTPROJECTSHARED_EXPORT MbtProject
    : public ProjectExplorer::BaseProject
{
  Q_OBJECT

public:
  explicit MbtProject(ProjectExplorer::BaseProjectManager *projectManager,
                      const QString& fileName);
  virtual ~MbtProject();

public:
  MbtProjectConfiguration*        mbtProjectConfiguration();
  MbtProjectConfiguration const*  mbtProjectConfiguration() const;

public:
  bool        supportsKit(ProjectExplorer::Kit *kit,
                          QString *errorMessage = 0) const;
  bool        needsConfiguration() const;
  bool        supportsNoTargetPanel() const;
  bool        reload(QString* errorString);
  QStringList files(FilesMode fileMode) const;

public:
  MbtProjectModel const*  mbtProjectModel() const;
  MbtProjectModel*        mbtProjectModel();
  const QStringList&      declFiles() const;
  const QStringList&      specModelFiles() const;
  quint16                 specModelCount() const;
  bool                    addSpecModelFile(const QString& specModelFileName,
                                           QString* errorMessage);

public:
  ExportTestSuiteMenu*        exportTestSuiteMenu() const;
  void                        addExportTestSuiteCommand(ComExportTestSuite* exportTestSuiteCommand);
  QList<ComExportTestSuite*>  exportTestSuiteCommands() const;

public:
  static void addDslError(const dslparser::DslError& newDslError,
                          const QString& errorFileName,
                          QList<ProjectExplorer::BaseError>* baseErrorList);

public:
  const QString&  tcgAppFilePath() const;
  void            setTcgAppFilePath(const QString& tcgAppFilePath);
  void            showTestCaseGraphic(model::ts::TestCaseItem const* testCaseItem,
                                      TcgCoverageProperty const* tcgCoverageProperty);
protected:
  virtual bool        fromMap(const QVariantMap &map);
  virtual QVariantMap toMap() const;

private:
  void  reset();

private slots:
  void dispatchExportTestSuiteCommand();
  void mbtSpecModelFileRemoved(const QString& mbtSpecModelFileName);

private:
  Q_DISABLE_COPY(MbtProject)
  class Private;
  Private*  _d;
};

class MBTPROJECTSHARED_EXPORT MbtProjectManager
    : public ProjectExplorer::BaseProjectManager
{
  Q_OBJECT
public:
  explicit MbtProjectManager();
  virtual ~MbtProjectManager();

protected:
  MbtProject* createProject(const QString &fileName);
};

} // namespace mbtproject
