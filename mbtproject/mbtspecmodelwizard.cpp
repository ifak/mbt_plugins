#include "mbtspecmodelwizard.hpp"

#include "mbtproject.hpp"
#include "mbtspecmodel.hpp"

#include <mobata/utils/functors.hpp>

#include <utils/qtcassert.h>
#include <projectexplorer/customwizard/customwizard.h>

#include <QFileInfo>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

namespace Internal {

//MbtSpecModelWizardDialog//
MbtSpecModelWizardDialog::MbtSpecModelWizardDialog(QWidget *parent)
  : Utils::FileWizardDialog(parent),
    _specModelType(MbtSpecModel::SpenatModel)
{}

void MbtSpecModelWizardDialog::setSpecModelType(int specModelType)
{
  this->_specModelType = specModelType;

  return;
}

int MbtSpecModelWizardDialog::specModelType() const
{
  return this->_specModelType;
}

//MbtSpecModelWizard//
MbtSpecModelWizard::MbtSpecModelWizard(const Core::BaseFileWizardParameters &parameters,
                                       QObject *parent)
  : Core::BaseFileWizard(parameters, parent), _contextProject(0), _contextNode(0)
{}

void MbtSpecModelWizard::setContextNode(ProjectExplorer::Node* node)
{
  this->_contextNode=node;

  return;
}

void MbtSpecModelWizard::setContextProject(ProjectExplorer::Project *project)
{
  this->_contextProject=project;

  return;
}

QWizard* MbtSpecModelWizard::createWizardDialog(QWidget *parent,
                                                const Core::WizardDialogParameters& wizardDialogParameters) const
{
  MbtSpecModelWizardDialog* dialog = new MbtSpecModelWizardDialog(parent);
  dialog->setPath(wizardDialogParameters.defaultPath());
  foreach (QWizardPage *p, wizardDialogParameters.extensionPages())
    dialog->addPage(p);

  return dialog;
}

Core::GeneratedFiles
MbtSpecModelWizard::generateFiles(QWizard const* wizard,
                                  QString* errorString) const
{
  Core::GeneratedFiles files;
  const MbtSpecModelWizardDialog* dialog = qobject_cast<const MbtSpecModelWizardDialog*>(wizard);
  QTC_ASSERT(dialog, return files);
  QTC_ASSERT(errorString, return files);

  //get mbt project
  MbtProject* mbtProject=qobject_cast<MbtProject*>(this->_contextProject);
  // error handling............
  if(!mbtProject)
  {
    QString errorMessage=tr("No MBT Project for new specification model!");
    utils::AddPtrString(errorString)<<errorMessage;

    return files;
  }

  QString fileName = dialog->fileName();

  if(fileName.endsWith(".spenat")) // no need for if(dialog->specSimModelType() == MbtSpecModel::SpenatModel)???
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
    QFile templateSpenatFile(QStringLiteral(":/mbtproject/template_spenat.spenat"));
    if(templateSpenatFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString content(QLatin1String(templateSpenatFile.readAll()));
      content.replace(QStringLiteral("$${ProjectName}"), mbtProject->displayName());
      content.replace(QStringLiteral("$${NewSpenatName}"),
                      QString(QLatin1String("spenat_%1")).arg(mbtProject->specModelCount()+1));
      newSpenatFile.setContents(content);
    }
    else
      *errorString+=QLatin1String("example new spenat file could not be written!");
    files.push_back(newSpenatFile);
  }
  else /// it should be state machine model
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

    // state machine content/////////
    QFile templateStateMachineFile(QStringLiteral(":/mbtproject/template_statemachine.statemachine"));
    if(templateStateMachineFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString content(QLatin1String(templateStateMachineFile.readAll()));
      content.replace(QStringLiteral("$${ProjectName}"), mbtProject->displayName());
      content.replace(QStringLiteral("$${NewStateMachineName}"),
                      QString(QLatin1String("stateMachine_%1")).arg(mbtProject->specModelCount()+1));
      newStateMachineFile.setContents(content);
    }
    else
      *errorString+=QLatin1String("example new statemachine file could not be written!");
    files.push_back(newStateMachineFile);
  }

  return files;
}

bool MbtSpecModelWizard::postGenerateFiles(const QWizard* wizard,
                                           const Core::GeneratedFiles& files,
                                           QString* errorMessage)
{
  qDebug()<<"MbtSpecModelWizard::postGenerateFiles() executed()";
  Q_UNUSED(wizard);

  //get mbt project
  MbtProject* mbtProject=qobject_cast<MbtProject*>(this->_contextProject);
  // error handling............
  if(!mbtProject)
  {
    QString errorString=tr("No MBT Project for new specification model!");
    if(errorMessage)
      *errorMessage+=errorString;
    else
      qCritical()<<"error: "<<errorString;

    return false;
  }

  for(const Core::GeneratedFile& specModelFile : files)
  {
    if(!mbtProject->addSpecModelFile(specModelFile.path(), errorMessage))
      return false;
  }

  return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files,
                                                                errorMessage);
}


} // namespace Internal
} // namespace mbtproject

