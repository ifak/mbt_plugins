#include "specsimmodelwizard.hpp"

#include "specsimproject.hpp"
#include "specsimmodel.hpp"

#include <mobata/utils/functors.hpp>

#include <utils/qtcassert.h>
#include <projectexplorer/customwizard/customwizard.h>

#include <QFileInfo>
#include <QDebug>

namespace specsim {

namespace Internal {

//SpecSimModelWizardDialog//
SpecSimModelWizardDialog::SpecSimModelWizardDialog(QWidget *parent)
  : Utils::FileWizardDialog(parent),
    _specSimModelType(SpecSimModel::StateMachineModel)
{}

void SpecSimModelWizardDialog::setSpecSimModelType(int specSimModelType)
{
  this->_specSimModelType = specSimModelType;

  return;
}

int SpecSimModelWizardDialog::specSimModelType() const
{
  return this->_specSimModelType;
}

//SpecSimModelWizard//
SpecSimModelWizard::SpecSimModelWizard(const Core::BaseFileWizardParameters &parameters,
                                       QObject *parent)
  : Core::BaseFileWizard(parameters, parent), _contextProject(0), _contextNode(0)
{}

void SpecSimModelWizard::setContextNode(ProjectExplorer::Node* node)
{
  this->_contextNode=node;

  return;
}

void SpecSimModelWizard::setContextProject(ProjectExplorer::Project *project)
{
  this->_contextProject=project;

  return;
}

QWizard* SpecSimModelWizard::createWizardDialog(QWidget *parent,
                                                const Core::WizardDialogParameters& wizardDialogParameters) const
{
  SpecSimModelWizardDialog* dialog = new SpecSimModelWizardDialog(parent);
  dialog->setPath(wizardDialogParameters.defaultPath());
  foreach (QWizardPage *p, wizardDialogParameters.extensionPages())
    dialog->addPage(p);

  return dialog;
}

Core::GeneratedFiles
SpecSimModelWizard::generateFiles(QWizard const* wizard,
                                  QString* errorString) const
{
  Core::GeneratedFiles files;
  const SpecSimModelWizardDialog* dialog = qobject_cast<const SpecSimModelWizardDialog*>(wizard);
  QTC_ASSERT(dialog, return files);
  QTC_ASSERT(errorString, return files);

  //get spec sim project
  SpecSimProject* specSimProject=qobject_cast<SpecSimProject*>(this->_contextProject);
  // error handling............
  if(!specSimProject)
  {
    QString errorMessage=tr("No Model Simulation Project for new simulation model!");
    utils::AddPtrString(errorString)<<errorMessage;

    return files;
  }

  QString fileName = dialog->fileName();
  if(fileName.endsWith(".spenat")) // no need for if(dialog->specSimModelType() == SpecSimModel::SpenatModel)???
  {
    ////spenat example file///////////
    const QString spenatFileName = Core::BaseFileWizard::buildFileName(dialog->path(),
                                                                       dialog->fileName(),
                                                                       QLatin1String("spenat"));

    QFileInfo spenatFileInfo(spenatFileName);
    if(spenatFileInfo.exists())
    {
      *errorString+=tr("there is already a spenat file '%1' available, "
                       "please choose another spenat file name!").arg(spenatFileName);
      return files;
    }

    Core::GeneratedFile newSpenatFile(spenatFileName);
    newSpenatFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    // spenat content/////////
    QFile templateSpenatFile(QStringLiteral(":/specsim/template_spenat.spenat"));
    if(templateSpenatFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString content(QLatin1String(templateSpenatFile.readAll()));
      content.replace(QStringLiteral("$${ProjectName}"), specSimProject->displayName());
      content.replace(QStringLiteral("$${NewSpenatName}"),
                      QString(QLatin1String("spenat_%1")).arg(specSimProject->specSimModelCount()+1));
      newSpenatFile.setContents(content);
    }
    else
      *errorString+=QLatin1String("example new spenat file could not be written!");
    files.push_back(newSpenatFile);
  }
  else
  {
    ////statemachine example file///////////
    const QString stateMachineFileName = Core::BaseFileWizard::buildFileName(dialog->path(),
                                                                       dialog->fileName(),
                                                                       QLatin1String("statemachine"));

    QFileInfo stateMachineFileInfo(stateMachineFileName);
    if(stateMachineFileInfo.exists())
    {
      *errorString+=tr("there is already a statemachine file '%1' available, "
                       "please choose another statemachine file name!").arg(stateMachineFileName);
      return files;
    }

    Core::GeneratedFile newStateMachineFile(stateMachineFileName);
    newStateMachineFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    // spenat content/////////
    QFile templateStateMachineFile(QStringLiteral(":/specsim/template_statemachine.statemachine"));
    if(templateStateMachineFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString content(QLatin1String(templateStateMachineFile.readAll()));
      content.replace(QStringLiteral("$${ProjectName}"), specSimProject->displayName());
      content.replace(QStringLiteral("$${NewStateMachineName}"),
                      QString(QLatin1String("stateMachine_%1")).arg(specSimProject->specSimModelCount()+1));
      newStateMachineFile.setContents(content);
    }
    else
      *errorString+=QLatin1String("example new statemachine file could not be written!");
    files.push_back(newStateMachineFile);
  }

  return files;
}

bool SpecSimModelWizard::postGenerateFiles(const QWizard* wizard,
                                           const Core::GeneratedFiles& files,
                                           QString* errorMessage)
{
  qDebug()<<"SpecSimModelWizard::postGenerateFiles() executed()";
  Q_UNUSED(wizard);

  //get spec sim project
  SpecSimProject* specSimProject=qobject_cast<SpecSimProject*>(this->_contextProject);
  // error handling............
  if(!specSimProject)
  {
    QString errorString=tr("No Model Simulation Project for new simulation model!");
    if(errorMessage)
      *errorMessage+=errorString;
    else
      qCritical()<<"error: "<<errorString;

    return false;
  }

  for(const Core::GeneratedFile& specSimModelFile : files)
  {
    if(!specSimProject->addSpecSimModelFile(specSimModelFile.path(), errorMessage))
      return false;
  }

  return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files,
                                                                errorMessage);
}


} // namespace Internal
} // namespace specsim

