#include "mbtprojectwizard.hpp"

#include "mbtprojectconstants.hpp"

#include <utils/filewizarddialog.h>
#include <utils/qtcassert.h>

#include <projectexplorer/customwizard/customwizard.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

MbtProjectWizardPage::MbtProjectWizardPage(const QString& title,
                                           QWidget *parent) :
  QWizardPage(parent),
  _complete(true)
{
  this->setTitle(title);
}

void MbtProjectWizardPage::slotChanged()
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


MbtProjectWizardDialog::MbtProjectWizardDialog(const QString& title,
                                               QWidget *parent) :
  Utils::FileWizardDialog(parent),
  _contentPage(new MbtProjectWizardPage(title))
{}


MbtProjectWizard::MbtProjectWizard(const Core::BaseFileWizardParameters &parameters,
                                   QObject *parent)
  : Core::BaseFileWizard(parameters, parent),
    _title(QStringLiteral("MBT Project")),
    _projectFileExtension(QStringLiteral("mbtproject"))
{}

QWizard* MbtProjectWizard::createWizardDialog(QWidget *parent,
                                              const Core::WizardDialogParameters& wizardDialogParameters) const
{
  MbtProjectWizardDialog *dialog = new MbtProjectWizardDialog(this->_title, parent);
  dialog->setPath(QDir::homePath());
  foreach (QWizardPage *p, wizardDialogParameters.extensionPages())
    dialog->addPage(p);

  return dialog;
}

Core::GeneratedFiles
MbtProjectWizard::generateFiles(QWizard const* wizard,
                                QString* errorString) const
{
  Core::GeneratedFiles files;
  const MbtProjectWizardDialog *dialog = qobject_cast<const MbtProjectWizardDialog*>(wizard);
  QTC_ASSERT(dialog, return files);
  QTC_ASSERT(errorString, return files);

  QString projectPath=dialog->path()+
                      QLatin1String("/")+
                      dialog->fileName();

  ////distest project example file///////////
  const QString projectFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                      dialog->fileName(),
                                                                      this->_projectFileExtension);
  Core::GeneratedFile projectFile(projectFileName);
  projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);
  QString exampleProjectFileContent=this->generateExampleProjectFileContent(projectFileName,
                                                                            errorString);
  projectFile.setContents(exampleProjectFileContent);
  files.push_back(projectFile);

  ////decl example file///////////
  const QString declFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                   dialog->fileName(),
                                                                   QLatin1String("decl"));
  Core::GeneratedFile declFile(declFileName);
  QFile exampleDeclFile(QLatin1String(":/mbtproject/template_decl.decl"));
  if(exampleDeclFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString content(QLatin1String(exampleDeclFile.readAll()));
    declFile.setContents(content);
  }
  else
    *errorString+=QLatin1String("example decl file could not be written!");
  files.push_back(declFile);

  ////spenat example file///////////
  const QString spenatFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                   dialog->fileName(),
                                                                   QLatin1String("spenat"));
  Core::GeneratedFile spenatFile(spenatFileName);
  QFile exampleSpenatFile(QLatin1String(":/mbtproject/template_spenat.spenat"));
  if(exampleSpenatFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString content(QLatin1String(exampleSpenatFile.readAll()));
    content.replace(QStringLiteral("$${ProjectName}"), dialog->fileName());
    content.replace(QStringLiteral("$${NewSpenatName}"),
                    QString(QLatin1String("spenat_1")));
    spenatFile.setContents(content);
  }
  else
    *errorString+=QLatin1String("example spenat file could not be written!");
  files.push_back(spenatFile);

  return files;
}

QString MbtProjectWizard::generateExampleProjectFileContent(const QString& projectFileName,
                                                            QString* errorMessage) const
{
  QTC_ASSERT(errorMessage, return QStringLiteral(""));

  if(projectFileName.isEmpty())
  {
    *errorMessage=QLatin1String("empty project file name!");
    return  QStringLiteral("");
  }

  QFileInfo exampleProjectFileInfo(projectFileName);

  QString exampleProjectFileContent=QLatin1String("[");
  exampleProjectFileContent+=QLatin1String(Constants::MBT_PROJECT_DECLFILES_KEY);
  exampleProjectFileContent+=QLatin1String("]\n");
  exampleProjectFileContent+=QLatin1String("size=1\n");
  exampleProjectFileContent+=QLatin1String("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QLatin1String(".decl\n");
  exampleProjectFileContent+=QLatin1String("\n");

  exampleProjectFileContent+=QLatin1String("[");
  exampleProjectFileContent+=QLatin1String(Constants::MBT_PROJECT_SPECMODELFILES_KEY);
  exampleProjectFileContent+=QLatin1String("]\n");
  exampleProjectFileContent+=QLatin1String("size=1\n");
  exampleProjectFileContent+=QLatin1String("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QLatin1String(".spenat\n");
  exampleProjectFileContent+=QLatin1String("\n");

  return exampleProjectFileContent;
}

bool MbtProjectWizard::postGenerateFiles(const QWizard* wizard,
                                         const Core::GeneratedFiles& files,
                                         QString* errorMessage)
{
  Q_UNUSED(wizard);

  return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files,
                                                                errorMessage);
}

} // namespace mbtproject
