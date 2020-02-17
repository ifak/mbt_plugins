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

#include <coreplugin/basefilewizard.h>
#include <utils/filewizarddialog.h>

#include <QWizardPage>

class QLineEdit;
class QPlainTextEdit;
class QFile;

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtProjectWizardPage
    : public QWizardPage
{
  Q_OBJECT
public:
  explicit MbtProjectWizardPage(const QString& title,
                                QWidget* parent = 0);

public:
  virtual bool isComplete() const { return _complete; }

private slots:
  void slotChanged();

private:
  bool _complete;
};


class MBTPROJECTSHARED_EXPORT MbtProjectWizardDialog
    : public Utils::FileWizardDialog
{
  Q_OBJECT
public:
  explicit MbtProjectWizardDialog(const QString& title,
                                  QWidget* parent = 0);

  QString title() const    { return this->_contentPage->title(); }

private:
  MbtProjectWizardPage* _contentPage;
};


class MBTPROJECTSHARED_EXPORT MbtProjectWizard
    : public Core::BaseFileWizard
{
public:
  explicit MbtProjectWizard(const Core::BaseFileWizardParameters &parameters,
                            QObject *parent = 0);

protected:
  virtual QWizard* createWizardDialog(QWidget *parent,
                                      const Core::WizardDialogParameters &wizardDialogParameters) const;

  virtual Core::GeneratedFiles generateFiles(const QWizard *wizard,
                                             QString* errorString) const;

  virtual QString generateExampleProjectFileContent(const QString& projectFileName,
                                                    QString* errorMessage) const;

  virtual bool postGenerateFiles(QWizard const* wizard,
                                 const Core::GeneratedFiles& files,
                                 QString *errorMessage);

protected:
  QString _title;
  QString _projectFileExtension;
};

} // namespace mbtproject
