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

#include "mbtp_global.hpp"

#include <coreplugin/basefilewizard.h>
#include <utils/filewizarddialog.h>

#include <QWizardPage>

class QLineEdit;
class QPlainTextEdit;
class QFile;

namespace mbtp {

class MBTPSHARED_EXPORT MbtpWizardPage
    : public QWizardPage
{
  Q_OBJECT
public:
  explicit MbtpWizardPage(const QString& title,
                                QWidget* parent = 0);

public:
  virtual bool isComplete() const { return _complete; }

private slots:
  void slotChanged();

private:
  bool _complete;
};


class MBTPSHARED_EXPORT MbtpWizardDialog
    : public Utils::FileWizardDialog
{
  Q_OBJECT
public:
  explicit MbtpWizardDialog(const QString& title,
                                  QWidget* parent = 0);

  QString title() const    { return this->_contentPage->title(); }

private:
  MbtpWizardPage* _contentPage;
};


class MBTPSHARED_EXPORT MbtpWizard
    : public Core::BaseFileWizard
{
public:
  explicit MbtpWizard(const Core::BaseFileWizardParameters &parameters,
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

} // namespace mbtp
