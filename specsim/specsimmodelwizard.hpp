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

#include <coreplugin/basefilewizard.h>
#include <utils/filewizarddialog.h>

#include <QWizardPage>

namespace ProjectExplorer{
class Node;
class Project;
}


namespace specsim {
namespace Internal {

class SpecSimModelWizardDialog : public Utils::FileWizardDialog
{
  Q_OBJECT

public:
  explicit SpecSimModelWizardDialog(QWidget *parent = 0);

public:
  QString title() const    { return QStringLiteral("Simulation Model"); }

  void  setSpecSimModelType(int specSimModelType);
  int   specSimModelType() const;

private:
  int _specSimModelType;
};


class SpecSimModelWizard : public Core::BaseFileWizard
{
  Q_OBJECT

public:
  explicit SpecSimModelWizard(const Core::BaseFileWizardParameters &parameters,
                              QObject *parent = 0);

public:
  void setContextNode(ProjectExplorer::Node* node);
  void setContextProject(ProjectExplorer::Project* project);

protected:
  virtual QWizard*              createWizardDialog(QWidget *parent,
                                                   const Core::WizardDialogParameters &wizardDialogParameters) const;
  virtual Core::GeneratedFiles  generateFiles(const QWizard *wizard,
                                              QString* errorString) const;
  virtual bool                  postGenerateFiles(QWizard const* wizard,
                                                  const Core::GeneratedFiles& files,
                                                  QString *errorMessage);

private:
  ProjectExplorer::Project* _contextProject;
  ProjectExplorer::Node*    _contextNode;
};

} // namespace Internal
} // namespace specsim
