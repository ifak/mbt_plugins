#include "mbtpprojectwizard.hpp"

#include "mbtpconstants.hpp"

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

namespace mbtp {

MbtpWizardPage::MbtpWizardPage(const QString& title,
                                           QWidget *parent) :
  QWizardPage(parent),
  _complete(true)
{
  this->setTitle(title);
}

void MbtpWizardPage::slotChanged()
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


MbtpWizardDialog::MbtpWizardDialog(const QString& title,
                                               QWidget *parent) :
  Utils::FileWizardDialog(parent),
  _contentPage(new MbtpWizardPage(title))
{}


MbtpWizard::MbtpWizard(const Core::BaseFileWizardParameters &parameters,
                                   QObject *parent)
  : Core::BaseFileWizard(parameters, parent),
    _title(QStringLiteral("MBTP Project")),
    _projectFileExtension(QStringLiteral("mbtp"))
{}

QWizard* MbtpWizard::createWizardDialog(QWidget *parent,
                                              const Core::WizardDialogParameters& wizardDialogParameters) const
{
  MbtpWizardDialog *dialog = new MbtpWizardDialog(this->_title, parent);
  dialog->setPath(QDir::homePath());
  for (QWizardPage *p : wizardDialogParameters.extensionPages())
    dialog->addPage(p);

  return dialog;
}

Core::GeneratedFiles
MbtpWizard::generateFiles(QWizard const* wizard,
                                QString* errorString) const
{
  Core::GeneratedFiles files;
  const MbtpWizardDialog *dialog = qobject_cast<const MbtpWizardDialog*>(wizard);
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

  ////trace example file///////////
  const QString traceFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                   dialog->fileName(),
                                                                   QLatin1String("testcasetrace"));
  Core::GeneratedFile traceFile(traceFileName);
  QString traceContent(QString("{\n\"iteration\": 1,\n\"testcases\": [\n{\n\"name\": \"testcase_01\",\n\"places\": [\n\"init\"\n],\n\"req_changed\": false,\n\"requirements\": [\n\"Req_BetriebsmodusFeldbus\",\n\"Req_Betriebsmodus\"\n],\n\"transitions\": [\n \"setzeModus_T4\"\n ]\n },\n {\n \"name\": \"testcase_02\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_BetriebsmodusFeldbus\",\n \"Req_Betriebsmodus\"\n ],\n \"transitions\": [\n \"setzeModus_T4\",\n \"setzeModus_T5\",\n \"noTrigger_T8\"\n ]\n },\n {\n \"name\": \"testcase_03\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_BetriebsmodusFeldbus\",\n \"Req_Betriebsmodus\"\n ],\n \"transitions\": [\n \"setzeModus_T4\",\n \"setzeModus_T6\",\n \"noTrigger_T8\"\n ]\n },\n {\n \"name\": \"testcase_04\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_BetriebsmodusFeldbus\",\n \"Req_Betriebsmodus\"\n ],\n \"transitions\": [\n \"setzeModus_T4\",\n \"setzeModus_T7\",\n \"noTrigger_T8\"\n ]\n },\n {\n \"name\": \"testcase_05\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T5\",\n \"noTrigger_T8\"\n ]\n },\n {\n \"name\": \"testcase_06\",\n \"places\": [\n \"init\",\n \"ZU\",\n \"AUF\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_AufBetrieb\",\n \"Req_BetriebsmodusFeldbus\",\n \"Req_Betriebsmodus\",\n \"Req_ZuBetrieb\"\n ],\n \"transitions\": [\n \"StellbefehlAufZu_T2\",\n \"StellbefehlAufZu_T3\",\n \"StellbefehlAufZu_T1\"\n ]\n },\n {\n \"name\": \"testcase_07\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T6\",\n \"noTrigger_T8\",\n \"setzeModus_T4\"\n ]\n },\n {\n \"name\": \"testcase_08\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T6\",\n \"noTrigger_T8\",\n \"setzeModus_T5\"\n ]\n },\n {\n \"name\": \"testcase_09\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T6\",\n \"noTrigger_T8\"\n ]\n },\n {\n \"name\": \"testcase_10\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T6\",\n \"noTrigger_T8\",\n \"setzeModus_T7\"\n ]\n },\n {\n \"name\": \"testcase_11\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T7\",\n \"noTrigger_T8\",\n \"setzeModus_T4\"\n ]\n },\n {\n \"name\": \"testcase_12\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T7\",\n \"noTrigger_T8\",\n \"setzeModus_T5\"\n ]\n },\n {\n \"name\": \"testcase_13\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T7\",\n \"noTrigger_T8\",\n \"setzeModus_T6\"\n ]\n },\n {\n \"name\": \"testcase_14\",\n \"places\": [\n \"init\",\n \"S1\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\"\n ],\n \"transitions\": [\n \"setzeModus_T7\",\n \"noTrigger_T8\"\n ]\n },\n {\n \"name\": \"testcase_15\",\n \"places\": [\n \"init\",\n \"ZU\",\n \"AUF\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_BetriebsmodusFeldbus\",\n \"Req_Betriebsmodus\",\n \"Req_AufBetrieb\",\n \"Req_ZuBetrieb\"\n ],\n \"transitions\": [\n \"setzeModus_T4\",\n \"StellbefehlAufZu_T2\",\n \"StellbefehlAufZu_T3\",\n \"StellbefehlAufZu_T1\"\n ]\n },\n {\n \"name\": \"testcase_16\",\n \"places\": [\n \"init\",\n \"S1\",\n \"ZU\",\n \"AUF\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\",\n \"Req_AufBetrieb\",\n \"Req_ZuBetrieb\"\n ],\n \"transitions\": [\n \"setzeModus_T6\",\n \"noTrigger_T8\",\n \"StellbefehlAufZu_T2\",\n \"StellbefehlAufZu_T3\",\n \"StellbefehlAufZu_T1\"\n ]\n },\n {\n \"name\": \"testcase_17\",\n \"places\": [\n \"init\",\n \"S1\",\n \"ZU\",\n \"AUF\"\n ],\n \"req_changed\": false,\n \"requirements\": [\n \"Req_Betriebsmodus\",\n \"Req_BetriebsmodusFeldbus\",\n \"Req_AufBetrieb\",\n \"Req_ZuBetrieb\"\n ],\n \"transitions\": [\n \"setzeModus_T7\",\n \"noTrigger_T8\",\n \"StellbefehlAufZu_T2\",\n \"StellbefehlAufZu_T3\",\n \"StellbefehlAufZu_T1\"\n ]\n }\n ]\n}\n"));
  traceFile.setContents(traceContent);
  files.push_back(traceFile);


  ////model example file///////////
  const QString modelFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                   dialog->fileName(),
                                                                   QLatin1String("spenatjson"));
  Core::GeneratedFile modelFile(modelFileName);
  QString modelContent(QString("{\n \"arcs\": [\n {\n \"sourceUuid\": \"{9643ec82-1f4b-4946-b802-a2bbb1fd77b9}\",\n \"targetUuid\": \"{9fca9e0b-c61f-40e4-b86e-83d471135893}\",\n \"uuid\": \"{3baa6b34-1053-4609-a79f-d454f30fc9b1}\"\n },\n {\n \"sourceUuid\": \"{9fca9e0b-c61f-40e4-b86e-83d471135893}\",\n \"targetUuid\": \"{7e95a6d2-a208-4809-8b0e-d71565e7b526}\",\n \"uuid\": \"{f91ebadc-9c7e-442f-a59d-73b405c354b9}\"\n },\n {\n \"sourceUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"targetUuid\": \"{abdd541a-5426-4afa-8b00-92a40bdb9a4c}\",\n \"uuid\": \"{4cf58319-3d18-493b-bb43-eece7888f42d}\"\n },\n {\n \"sourceUuid\": \"{abdd541a-5426-4afa-8b00-92a40bdb9a4c}\",\n \"targetUuid\": \"{7e95a6d2-a208-4809-8b0e-d71565e7b526}\",\n \"uuid\": \"{2f4c1297-c93b-4aae-a599-11983c392e7b}\"\n },\n {\n \"sourceUuid\": \"{7e95a6d2-a208-4809-8b0e-d71565e7b526}\",\n \"targetUuid\": \"{fec61c3f-026d-4a3e-994f-7b606e54f18a}\",\n \"uuid\": \"{3c0294ef-0c78-44df-bab9-f1cf4a5d6847}\"\n },\n {\n \"sourceUuid\": \"{fec61c3f-026d-4a3e-994f-7b606e54f18a}\",\n \"targetUuid\": \"{9643ec82-1f4b-4946-b802-a2bbb1fd77b9}\",\n \"uuid\": \"{cfab7975-8822-4a37-ae0a-7731a096aa6f}\"\n },\n {\n \"sourceUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"targetUuid\": \"{d44aebcd-7dec-41fa-96b8-94da4df95d42}\",\n \"uuid\": \"{04679fd6-db48-441b-96b7-c4b7b65d42df}\"\n },\n {\n \"sourceUuid\": \"{d44aebcd-7dec-41fa-96b8-94da4df95d42}\",\n \"targetUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"uuid\": \"{a7813b91-ce35-4a8e-8bdd-a96b19b183a8}\"\n },\n {\n \"sourceUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"targetUuid\": \"{a68f6964-f9ae-47b8-81a7-48f07fc92e22}\",\n \"uuid\": \"{9aa5f4c2-3d24-4d1f-8b36-9557b39f3dbc}\"\n },\n {\n \"sourceUuid\": \"{a68f6964-f9ae-47b8-81a7-48f07fc92e22}\",\n \"targetUuid\": \"{7099ae3a-761a-46fe-adcb-51d6690cedee}\",\n \"uuid\": \"{7d58adac-8ca6-478b-9ab4-99d74bd572e1}\"\n },\n {\n \"sourceUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"targetUuid\": \"{f9aee48a-832a-4a07-809d-a780b8d9d2b1}\",\n \"uuid\": \"{2854d7b8-6ed5-4268-9878-693eec32d355}\"\n },\n {\n \"sourceUuid\": \"{f9aee48a-832a-4a07-809d-a780b8d9d2b1}\",\n \"targetUuid\": \"{7099ae3a-761a-46fe-adcb-51d6690cedee}\",\n \"uuid\": \"{e8e24a90-9002-4d1d-8c82-c359f3442ff8}\"\n },\n {\n \"sourceUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"targetUuid\": \"{fac4afb3-c7d5-4f1e-871e-21687865ee79}\",\n \"uuid\": \"{59fc0be8-0548-46fc-82b6-874f0fc8990c}\"\n },\n {\n \"sourceUuid\": \"{fac4afb3-c7d5-4f1e-871e-21687865ee79}\",\n \"targetUuid\": \"{7099ae3a-761a-46fe-adcb-51d6690cedee}\",\n \"uuid\": \"{ee3f69c8-f6db-4068-a7a9-75fb76e67943}\"\n },\n {\n \"sourceUuid\": \"{7099ae3a-761a-46fe-adcb-51d6690cedee}\",\n \"targetUuid\": \"{1d5ad3c8-ba78-46c6-8544-356d61655c40}\",\n \"uuid\": \"{ae38e606-5e7f-4bca-b121-0c1238660e4b}\"\n },\n {\n \"sourceUuid\": \"{1d5ad3c8-ba78-46c6-8544-356d61655c40}\",\n \"targetUuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\",\n \"uuid\": \"{8d5f1f0a-cac9-403d-b328-9d0c7ac25312}\"\n }\n ],\n \"attributes\": [\n {\n \"initValue\": \"1\",\n \"name\": \"modus\",\n \"type\": \"int\"\n }\n ],\n \"functions\": [\n ],\n \"initPlaces\": [\n \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\"\n ],\n \"name\": \"requirements_model\",\n \"places\": [\n {\n \"name\": \"AUF\",\n \"uuid\": \"{9643ec82-1f4b-4946-b802-a2bbb1fd77b9}\"\n },\n {\n \"name\": \"ZU\",\n \"uuid\": \"{7e95a6d2-a208-4809-8b0e-d71565e7b526}\"\n },\n {\n \"name\": \"init\",\n \"uuid\": \"{1d073117-517a-43a2-8843-ce5f7118fcf3}\"\n },\n {\n \"name\": \"S1\",\n \"uuid\": \"{7099ae3a-761a-46fe-adcb-51d6690cedee}\"\n }\n ],\n \"ports\": [\n {\n \"name\": \"user\"\n }\n ],\n \"signals\": [\n {\n \"name\": \"EndlageAuf\",\n \"params\": [\n {\n \"name\": \"zustand\",\n \"type\": \"bool\"\n }\n ],\n \"uuid\": \"{1a7ed6fa-88d8-48a7-b0f2-2d7f1a9d7022}\"\n },\n {\n \"name\": \"EndlageZu\",\n \"params\": [\n {\n \"name\": \"zustand\",\n \"type\": \"bool\"\n }\n ],\n \"uuid\": \"{5bba76b3-6401-42f5-bf39-d459551b4460}\"\n },\n {\n \"name\": \"StellbefehlAufZu\",\n \"params\": [\n {\n \"name\": \"modus\",\n \"type\": \"int\"\n },\n {\n \"name\": \"auf\",\n \"type\": \"bool\"\n },\n {\n \"name\": \"zu\",\n \"type\": \"bool\"\n }\n ],\n \"uuid\": \"{fe964390-f368-4ba5-aa6b-f691851c4c1c}\"\n },\n {\n \"name\": \"ModusGesetzt\",\n \"params\": [\n {\n \"name\": \"modus\",\n \"type\": \"int\"\n }\n ],\n \"uuid\": \"{67e44a2c-81bc-44d3-ac98-407f6d2d21eb}\"\n },\n {\n \"name\": \"setzeModus\",\n \"params\": [\n {\n \"name\": \"modus\",\n \"type\": \"int\"\n }\n ],\n \"uuid\": \"{3c7771d3-7bc5-4779-b235-72aa68cace13}\"\n }\n ],\n \"transitions\": [\n {\n \"actions\": \"send EndlageAuf(false) to user;send EndlageZu(true) to user;\",\n \"guard\": \"msg.modus==this.modus && msg.auf==false && msg.zu==true\",\n \"name\": \"StellbefehlAufZu_T1\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{fe964390-f368-4ba5-aa6b-f691851c4c1c}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{9fca9e0b-c61f-40e4-b86e-83d471135893}\"\n },\n {\n \"actions\": \"send EndlageAuf(false) to user;send EndlageZu(true) to user;\",\n \"guard\": \"msg.modus==this.modus && msg.auf==false && msg.zu==true\",\n \"name\": \"StellbefehlAufZu_T2\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{fe964390-f368-4ba5-aa6b-f691851c4c1c}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{abdd541a-5426-4afa-8b00-92a40bdb9a4c}\"\n },\n {\n \"actions\": \"send EndlageAuf(true) to user;send EndlageZu(false) to user;\",\n \"guard\": \"msg.modus==this.modus && msg.auf==true && msg.zu==false\",\n \"name\": \"StellbefehlAufZu_T3\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{fe964390-f368-4ba5-aa6b-f691851c4c1c}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{fec61c3f-026d-4a3e-994f-7b606e54f18a}\"\n },\n {\n \"actions\": \"this.modus=3;send ModusGesetzt(3) to user;\",\n \"guard\": \"modus==3\",\n \"name\": \"setzeModus_T4\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{3c7771d3-7bc5-4779-b235-72aa68cace13}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{d44aebcd-7dec-41fa-96b8-94da4df95d42}\"\n },\n {\n \"actions\": \"this.modus=1;send ModusGesetzt(1) to user;\",\n \"guard\": \"modus==1\",\n \"name\": \"setzeModus_T5\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{3c7771d3-7bc5-4779-b235-72aa68cace13}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{a68f6964-f9ae-47b8-81a7-48f07fc92e22}\"\n },\n {\n \"actions\": \"this.modus=2;send ModusGesetzt(2) to user;\",\n \"guard\": \"modus==2\",\n \"name\": \"setzeModus_T6\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{3c7771d3-7bc5-4779-b235-72aa68cace13}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{f9aee48a-832a-4a07-809d-a780b8d9d2b1}\"\n },\n {\n \"actions\": \"this.modus=4;send ModusGesetzt(4) to user;\",\n \"guard\": \"modus==4\",\n \"name\": \"setzeModus_T7\",\n \"trigger\": {\n \"port\": \"user\",\n \"signalUuid\": \"{3c7771d3-7bc5-4779-b235-72aa68cace13}\",\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{fac4afb3-c7d5-4f1e-871e-21687865ee79}\"\n },\n {\n \"actions\": \"\",\n \"guard\": \"\",\n \"name\": \"noTrigger_T8\",\n \"trigger\": {\n \"type\": \"EventTrigger\"\n },\n \"uuid\": \"{1d5ad3c8-ba78-46c6-8544-356d61655c40}\"\n }\n ],\n \"types\": [\n {\n \"highest\": \"\",\n \"lowest\": \"\",\n \"name\": \"int\",\n \"typeEnum\": \"int\",\n \"uuid\": \"{39b30fff-97fd-49dd-a13a-3afa20ea6621}\"\n },\n {\n \"highest\": \"\",\n \"lowest\": \"\",\n \"name\": \"real\",\n \"typeEnum\": \"real\",\n \"uuid\": \"{8a9cbcf7-ed19-4715-89ae-b75d1ab1e611}\"\n },\n {\n \"highest\": \"\",\n \"lowest\": \"\",\n \"name\": \"bool\",\n \"typeEnum\": \"bool\",\n \"uuid\": \"{fd368fe0-fffe-4934-8fa8-d725349da232}\"\n }\n ]\n}\n"));
  modelFile.setContents(modelContent);
  files.push_back(modelFile);

  ////attributes example file///////////
  const QString attributesFileName = Core::BaseFileWizard::buildFileName(projectPath,
                                                                   dialog->fileName(),
                                                                   QLatin1String("attributes"));
  Core::GeneratedFile attributesFile(attributesFileName);
  QString attributesContent(QString("{\n \"attributes\": [\n {\n \"name\": \"Volatility\",\n \"weight\": 0.2\n },\n {\n \"name\": \"Customer_priority\",\n \"weight\": 0.8\n }\n ],\n \"requirements\": [\n {\n \"name\": \"Req_AufBetrieb\",\n \"attributes\": [\n {\n \"name\": \"Volatility\",\n \"value\": 0.8\n },\n {\n \"name\": \"Customer_priority\",\n \"value\": 0.2\n }\n ]\n },\n {\n \"name\": \"Req_ZuBetrieb\",\n \"attributes\": [\n {\n \"name\": \"Volatility\",\n \"value\": 0.4\n },\n {\n \"name\": \"Customer_priority\",\n \"value\": 0.6\n }\n ]\n },\n {\n \"name\": \"Req_Betriebsmodus\",\n \"attributes\": [\n {\n \"name\": \"Volatility\",\n \"value\": 0.8\n },\n {\n \"name\": \"Customer_priority\",\n \"value\": 0.2\n }\n ]\n },\n {\n \"name\": \"Req_BetriebsmodusFeldbus\",\n \"attributes\": [\n {\n \"name\": \"Volatility\",\n \"value\": 0.54\n },\n {\n \"name\": \"Customer_priority\",\n \"value\": 0.46\n }\n ]\n }\n ]\n}\n"));
  attributesFile.setContents(attributesContent);
  files.push_back(attributesFile);

  return files;
}

QString MbtpWizard::generateExampleProjectFileContent(const QString& projectFileName,
                                                            QString* errorMessage) const
{
  QTC_ASSERT(errorMessage, return QStringLiteral(""));

  if(projectFileName.isEmpty())
  {
    *errorMessage=QLatin1String("empty project file name!");
    return  QStringLiteral("");
  }

  QFileInfo exampleProjectFileInfo(projectFileName);

  QString exampleProjectFileContent=QStringLiteral("[TestcaseTrace]\n");
  exampleProjectFileContent+=QStringLiteral("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QStringLiteral(".testcasetrace\n");
  exampleProjectFileContent+=QStringLiteral("size=1\n");
  exampleProjectFileContent+=QStringLiteral("\n");

  exampleProjectFileContent+=QStringLiteral("[SpenatJson]\n");
  exampleProjectFileContent+=QStringLiteral("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QStringLiteral(".spenatjson\n");
  exampleProjectFileContent+=QStringLiteral("size=1\n");
  exampleProjectFileContent+=QStringLiteral("\n");

  exampleProjectFileContent+=QStringLiteral("[Attributes]\n");
  exampleProjectFileContent+=QStringLiteral("1/file=")
                             +exampleProjectFileInfo.baseName()
                             +QStringLiteral(".attributes\n");
  exampleProjectFileContent+=QStringLiteral("size=1\n");
  exampleProjectFileContent+=QStringLiteral("\n");

  return exampleProjectFileContent;
}

bool MbtpWizard::postGenerateFiles(const QWizard* wizard,
                                         const Core::GeneratedFiles& files,
                                         QString* errorMessage)
{
  Q_UNUSED(wizard);

  return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files,
                                                                errorMessage);
}

} // namespace mbtp
