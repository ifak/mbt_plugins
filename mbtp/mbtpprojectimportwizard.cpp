#include "mbtpprojectimportwizard.hpp"

#include <utils/filewizarddialog.h>
#include <utils/qtcassert.h>

#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorer.h>

#include <QXmlStreamWriter>

#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QFileInfo>
#include <QDir>
#include <QDebug>


#include <mobata/memory_leak_start.hpp>

namespace mbtp {

MbtpProjectImportWizardPage::MbtpProjectImportWizardPage(QWidget *parent) :
  QWizardPage(parent),
  _complete(true)
{
  setTitle(tr("Import Files as Mbtp-Project"));

  speLabel = new QLabel(tr("Spenatjson-File:"));
  speLineEdit = new QLineEdit;
  speLabel->setBuddy(speLineEdit);
  speButton = new QPushButton(tr("Browse"));

  QObject::connect(speButton, &QPushButton::clicked,
                   this, &MbtpProjectImportWizardPage::speFileDialog);
  QObject::connect(speLineEdit, &QLineEdit::textChanged,
                   this, &MbtpProjectImportWizardPage::pathChanged);


  traLabel = new QLabel(tr("Testcasetrace-File:"));
  traLineEdit = new QLineEdit;
  traLabel->setBuddy(traLineEdit);
  traButton = new QPushButton(tr("Browse"));

  QObject::connect(traButton, &QPushButton::clicked,
                   this, &MbtpProjectImportWizardPage::traFileDialog);
  QObject::connect(traLineEdit, &QLineEdit::textChanged,
                   this, &MbtpProjectImportWizardPage::pathChanged);


  proLabel = new QLabel(tr("Projectpath:"));
  proLineEdit = new QLineEdit;
  proLabel->setBuddy(proLineEdit);
  proButton = new QPushButton(tr("Browse"));
  QObject::connect(proButton, &QPushButton::clicked,
                   this, &MbtpProjectImportWizardPage::proFileDialog);
  QObject::connect(proLineEdit, &QLineEdit::textChanged,
                   this, &MbtpProjectImportWizardPage::pathChanged);

  proNameLabel = new QLabel(tr("Projectpath:"));
  proNameLineEdit = new QLineEdit;
  proNameLabel->setBuddy(proNameLineEdit);
  QObject::connect(proNameLineEdit, &QLineEdit::textChanged,
                   this, &MbtpProjectImportWizardPage::pathChanged);


  QGridLayout *layout = new QGridLayout;
  layout->addWidget(speLabel, 0, 0);
  layout->addWidget(speLineEdit, 0, 1);
  layout->addWidget(speButton, 0, 2);
  layout->addWidget(traLabel, 1, 0);
  layout->addWidget(traLineEdit, 1, 1);
  layout->addWidget(traButton, 1, 2);
  layout->addWidget(proLabel, 2, 0);
  layout->addWidget(proLineEdit, 2, 1);
  layout->addWidget(proButton, 2, 2);
  layout->addWidget(proNameLabel, 3, 0);
  layout->addWidget(proNameLineEdit, 3, 1);
  setLayout(layout);
}

void MbtpProjectImportWizardPage::speFileDialog()
{
  QString file = QFileDialog::getOpenFileName(this,"Select spenatjson-File",QDir::homePath(),"spenatjson-file (*.spenatjson)");
  this->speLineEdit->setText(file);
}

void MbtpProjectImportWizardPage::traFileDialog()
{
  QString file = QFileDialog::getOpenFileName(this,"Select testcasetrace-File",QDir::homePath(),"testcasetrace-file (*.testcasetrace)");
  this->traLineEdit->setText(file);
}

void MbtpProjectImportWizardPage::proFileDialog()
{
  QString dir = QFileDialog::getExistingDirectory(this,"Select Project Directory",QDir::homePath(),QFileDialog::ShowDirsOnly);
  this->proLineEdit->setText(dir);
}

void MbtpProjectImportWizardPage::pathChanged()
{
  this->setProperty("spepath",this->speLineEdit->text());
  this->setProperty("trapath",this->traLineEdit->text());
  this->setProperty("propath",this->proLineEdit->text());
  this->setProperty("proname", this->proNameLineEdit->text());
}

void MbtpProjectImportWizardPage::slotChanged()
{
  ///example code
  const bool completeNow=true;
  if (completeNow != this->_complete)
  {
    _complete = completeNow;
    emit completeChanged();
  }

  return;
}


MbtpProjectImportWizardDialog::MbtpProjectImportWizardDialog(QString* spepath, QString* trapath, QString* propath, QString* proname, QWidget *parent) :
  QWizard(parent)
{
  this->speString = spepath;
  this->traString = trapath;
  this->proString = propath;
  this->proNameString = proname;
  _contentPage = new MbtpProjectImportWizardPage();
  QObject::connect(this, &MbtpProjectImportWizardDialog::finished,
                   this, &MbtpProjectImportWizardDialog::wirteProperties);
  setPage(0, _contentPage);
}

void MbtpProjectImportWizardDialog::wirteProperties(bool ok)
{
  *speString = qvariant_cast<QString>(_contentPage->property("spepath"));
  *traString = qvariant_cast<QString>(_contentPage->property("trapath"));
  *proString = qvariant_cast<QString>(_contentPage->property("propath"));
  *proNameString = qvariant_cast<QString>(_contentPage->property("proname"));
  emit finished2(ok);
}

MbtpProjectImportWizard::MbtpProjectImportWizard(const QString& title,
                                                 const QString& projectFileExtension,
                                                 const Core::BaseFileWizardParameters &parameters,
                                                 QObject *parent)
  : Core::BaseFileWizard(parameters, parent),
    _title(title),
    _projectFileExtension(projectFileExtension)
{}

MbtpProjectImportWizard::~MbtpProjectImportWizard()
{
  delete this->_spePath;
  delete this->_traPath;
  delete this->_proPath;
  delete this->_proName;
}

QWizard* MbtpProjectImportWizard::createWizardDialog(QWidget *parent,
                                                     const Core::WizardDialogParameters& wizardDialogParameters) const
{
  Q_UNUSED(wizardDialogParameters);
  Q_UNUSED(parent);
  MbtpProjectImportWizardDialog *dialog = new MbtpProjectImportWizardDialog(_spePath, _traPath, _proPath, _proName);

  QObject::connect(dialog, &MbtpProjectImportWizardDialog::finished2,
                   this, &MbtpProjectImportWizard::generateNow);

  return dialog;
}

void MbtpProjectImportWizard::generateNow(bool ok)
{
  if(!ok){
    return;
  }
  QString error;

  this->generateFiles(nullptr , &error);
}

Core::GeneratedFiles MbtpProjectImportWizard::generateFiles(QWizard const* wizard,
                                                            QString* errorString) const
{
  Q_UNUSED(wizard)
  Core::GeneratedFiles files;

  QTC_ASSERT(errorString, return files);

  QString projectPath = *this->_proPath;
  QString spePath = *this->_spePath;
  QString traPath = *this->_traPath;
  QString proName = *this->_proName;

  if(projectPath==""){
    return files;
  }
  if(spePath==""){
    return files;
  }
  if(traPath==""){
    return files;
  }

  if(proName==""){
    return files;
  }

  if(projectPath.right(1) != "/"){
    projectPath += "/";
  }

  QDir proDir(projectPath);

  QFile file(proName + ".mbtp");
  QString filecontent;
  filecontent += QStringLiteral("[TestcaseTrace]\n");
  filecontent += QStringLiteral("1/file=");
  filecontent += proDir.relativeFilePath(traPath);
  filecontent += QStringLiteral("\nsize=1\n");

  filecontent += QStringLiteral("[SpenatJson]\n");
  filecontent += QStringLiteral("1/file=");
  filecontent += proDir.relativeFilePath(spePath);
  filecontent += QStringLiteral("\nsize=1\n");

  filecontent += QStringLiteral("[Attributes]\n");
  filecontent += QStringLiteral("1/file=");
  filecontent += proName + ".attributes";
  filecontent += QStringLiteral("\nsize=1\n");

  QString error;
  Core::GeneratedFile f(projectPath + proName + ".mbtp");
  f.setContents(filecontent);
  f.write(&error);

  QString attContent;

  Core::GeneratedFile f2(projectPath + proName + ".attributes");
  f2.setContents(attContent);
  f2.write(&error);

  files.append(f);
  files.append(f2);

  ProjectExplorer::ProjectExplorerPlugin::instance()->openProject(projectPath + proName + ".mbtp", errorString);

  return files;
}

bool MbtpProjectImportWizard::postGenerateFiles(const QWizard* wizard,
                                                const Core::GeneratedFiles& files,
                                                QString* errorMessage)
{
  Q_UNUSED(wizard);

  return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}



} // namespace mbtp
