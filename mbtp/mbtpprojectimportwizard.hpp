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
#ifndef MBTP_MbtpProjectImportWizard_HPP
#define MBTP_MbtpProjectImportWizard_HPP

#include "mbtp_global.hpp"

#include <coreplugin/basefilewizard.h>
#include <utils/filewizarddialog.h>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QWizardPage>
#include "mbtpproject.hpp"

namespace mbtp {

class MBTPSHARED_EXPORT MbtpProjectImportWizardPage
    : public QWizardPage
{
  Q_OBJECT
public:
  explicit MbtpProjectImportWizardPage(QWidget *parent = nullptr);

public:
  virtual bool isComplete() const { return _complete; }

private slots:
  void slotChanged();
  void speFileDialog();
  void traFileDialog();
  void proFileDialog();
  void pathChanged();
private:
  bool _complete;
  QLabel *speLabel;
  QLineEdit *speLineEdit;
  QPushButton *speButton;

  QLabel *traLabel;
  QLineEdit *traLineEdit;
  QPushButton *traButton;

  QLabel *proLabel;
  QLineEdit *proLineEdit;
  QPushButton *proButton;

  QLabel *proNameLabel;
  QLineEdit *proNameLineEdit;
};

class MBTPSHARED_EXPORT MbtpProjectImportWizardDialog
    : public QWizard
{
  Q_OBJECT
public:
  explicit MbtpProjectImportWizardDialog(QString *spepath, QString *trapath, QString *propath, QString *proname, QWidget *parent = nullptr);

  QString title() const    { return this->_contentPage->title(); }
private slots:
  void wirteProperties(bool ok);
signals:
  void finished2(bool ok);
private:
  MbtpProjectImportWizardPage* _contentPage;
  QString* speString;
  QString* traString;
  QString* proString;
  QString* proNameString;
};

class MBTPSHARED_EXPORT MbtpProjectImportWizard
    : public Core::BaseFileWizard
{
  Q_OBJECT
public:
  explicit MbtpProjectImportWizard(const QString& title,
                                const QString& projectFileExtension,
                                const Core::BaseFileWizardParameters &parameters,
                                QObject *parent = nullptr);
  ~MbtpProjectImportWizard();

private:
  virtual QWizard* createWizardDialog(QWidget *parent,
                                      const Core::WizardDialogParameters &wizardDialogParameters) const;

  virtual Core::GeneratedFiles generateFiles(const QWizard *wizard,
                                             QString* errorString) const;

  virtual bool postGenerateFiles(QWizard const* wizard,
                                 const Core::GeneratedFiles& files,
                                 QString *errorMessage);

private slots:
  void generateNow(bool ok);

private:
  QString _title;
  QString _projectFileExtension;
  QString* _spePath = new QString();
  QString* _traPath = new QString();
  QString* _proPath = new QString();
  QString* _proName = new QString();

};

} // namespace mbtp

#endif // MBTP_MbtpProjectImportWizard_HPP
