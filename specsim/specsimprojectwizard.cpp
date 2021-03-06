#include "specsimprojectwizard.hpp"

#include "specsimconstants.hpp"

#include <utils/filewizarddialog.h>
#include <utils/qtcassert.h>

#include <projectexplorer/customwizard/customwizard.h>

//#include <QVBoxLayout>
//#include <QLabel>
//#include <QLineEdit>
//#include <QPlainTextEdit>
//#include <QFileInfo>
#include <QDir>
//#include <QDebug>

namespace specsim {

SpecSimProjectWizardPage::SpecSimProjectWizardPage(const QString& title,
                                                   QWidget *parent) :
  QWizardPage(parent),
  _complete(true)
{
  this->setTitle(title);
}

void SpecSimProjectWizardPage::slotChanged()
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


SpecSimProjectWizardDialog::SpecSimProjectWizardDialog(const QString& title,
                                                       QWidget *parent) :
  Utils::FileWizardDialog(parent),
  _contentPage(new SpecSimProjectWizardPage(title))
{}


SpecSimProjectWizard::SpecSimProjectWizard(const Core::BaseFileWizardParameters &parameters,
                                           QObject *parent)
  : Core::BaseFileWizard(parameters, parent),
    _title(QStringLiteral("Model Simulation Project")),
    _projectFileExtension(QStringLiteral("specsimproject"))
{}

QWizard* SpecSimProjectWizard::createWizardDialog(QWidget *parent,
                                                  const Core::WizardDialogParameters& wizardDialogParameters) const
{
  SpecSimProjectWizardDialog *dialog = new SpecSimProjectWizardDialog(this->_title, parent);
  dialog->setPath(QDir::homePath());
  foreach (QWizardPage *p, wizardDialogParameters.extensionPages())
    dialog->addPage(p);

  return dialog;
}

Core::GeneratedFiles
SpecSimProjectWizard::generateFiles(QWizard const* wizard,
                                    QString* errorString) const
{
  Core::GeneratedFiles files;
  const SpecSimProjectWizardDialog *dialog = qobject_cast<const SpecSimProjectWizardDialog*>(wizard);
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
  QFile exampleDeclFile(QLatin1String(":/specsim/template_decl.decl"));
  if(exampleDeclFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString content(QLatin1String(exampleDeclFile.readAll()));
    declFile.setContents(content);
  }
  else
    *errorString+=QLatin1String("example decl file could not be written!");
  files.push_back(declFile);

  ////state machine example file///////////
  const QString stateMachineFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                     dialog->fileName(),
                                                                     QLatin1String("statemachine"));
  Core::GeneratedFile stateMachineFile(stateMachineFileName);
  QFile exampleStateMachineFile(QLatin1String(":/specsim/template_statemachine.statemachine"));
  if(exampleStateMachineFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString content(QLatin1String(exampleStateMachineFile.readAll()));
    content.replace(QStringLiteral("$${ProjectName}"), dialog->fileName());
    stateMachineFile.setContents(content);
  }
  else
    *errorString+=QLatin1String("example state machine file could not be written!");
  files.push_back(stateMachineFile);

  return files;
}

QString SpecSimProjectWizard::generateExampleProjectFileContent(const QString& projectFileName,
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
  exampleProjectFileContent+=QLatin1String(Constants::SPECSIM_PROJECT_DECLFILES_KEY);
  exampleProjectFileContent+=QLatin1String("]\n");
  exampleProjectFileContent+=QLatin1String("size=1\n");
  exampleProjectFileContent+=QLatin1String("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QLatin1String(".decl\n");
  exampleProjectFileContent+=QLatin1String("\n");

  exampleProjectFileContent+=QLatin1String("[");
  exampleProjectFileContent+=QLatin1String(Constants::SPECSIM_PROJECT_SPECSIMMODELFILES_KEY);
  exampleProjectFileContent+=QLatin1String("]\n");
  exampleProjectFileContent+=QLatin1String("size=1\n");
  exampleProjectFileContent+=QLatin1String("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QLatin1String(".statemachine\n");
  exampleProjectFileContent+=QLatin1String("\n");

  return exampleProjectFileContent;
}

bool SpecSimProjectWizard::postGenerateFiles(const QWizard* wizard,
                                             const Core::GeneratedFiles& files,
                                             QString* errorMessage)
{
  Q_UNUSED(wizard);

  return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files,
                                                                errorMessage);
}

} // namespace specsim
