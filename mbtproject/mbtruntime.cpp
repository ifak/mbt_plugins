#include "mbtruntime.hpp"

#include "mbtproject.hpp"
#include "mbtprojectmodel.hpp"
#include "mbtspenatmodel.hpp"
#include "mbtstatemachinemodel.hpp"
#include "tcgcoverageproperty.hpp"

#include <mobata/model/ts/testsuiteitem.hpp>
#include <mobata/model/ts/io/comreadxmltestsuiteitem.hpp>

#include <mobata/model/spenat/spenatmodel.hpp>
#include <mobata/model/spenat/comcreatespenatmodel.hpp>
#include <mobata/convert/statemachine/comstatemachinetospenat.hpp>
#include <libs/dslparser/dslparser/spenat/combuildspenatfile.hpp>
#include <modtrace/comcreatemodetrace.hpp>

#include <mobata/utils/fileutils.hpp>

#include <projectexplorer/target.h>
#include <projectexplorer/buildconfiguration.h>

#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <QJsonDocument>

namespace mbtproject {

class MbtRuntime::Private
{
  friend class MbtRuntime;

  MbtRuntime::MbtRuntimeState _mbtRuntimeState;
  MbtProject*                 _mbtProject;
  QString                     _tcgApp;
  QString                     _buildDirectory;
  QString                     _projectDirectory;
  QList<MbtSpecModel*>        _mbtSpecModels;
  QStringList                 _errorMessages;

  QList<TcgCoverageProperty*> _tcgCoverageProperties;
  QList<QProcess*>            _tcgProcesses;
  QList<QProcess*>            _startedTcgProcesses;
  QList<QProcess*>            _runningTcgProcesses;

  Private(MbtProject* mbtProject)
    : _mbtRuntimeState(MbtRuntime::Init),
      _mbtProject(mbtProject)
  {}

  TcgCoverageProperty* tcgCoveragePropertyForTcgProcess(QProcess* tcgProcess);
};

TcgCoverageProperty* MbtRuntime::Private::tcgCoveragePropertyForTcgProcess(QProcess* tcgProcess)
{
  Q_ASSERT(tcgProcess);

  int indexOfProcess = this->_tcgProcesses.indexOf(tcgProcess);
  Q_ASSERT(indexOfProcess>=0);
  Q_ASSERT(indexOfProcess<this->_tcgCoverageProperties.size());
  TcgCoverageProperty* tcgCoverageProperty = this->_tcgCoverageProperties.at(indexOfProcess);
  Q_ASSERT(tcgCoverageProperty);

  return tcgCoverageProperty;
}

MbtRuntime::MbtRuntime(MbtProject* mbtProject,
                       QObject* parent)
  : QObject(parent),
    _d(new Private(mbtProject))
{
  Q_ASSERT(this->_d->_mbtProject);
  Q_ASSERT(this->_d->_mbtProject->mbtProjectModel());
  Q_ASSERT(this->_d->_mbtProject->activeTarget());
  Q_ASSERT(this->_d->_mbtProject->activeTarget()->activeBuildConfiguration());

  this->_d->_tcgApp = this->_d->_mbtProject->tcgAppFilePath();
  this->_d->_buildDirectory = this->_d->_mbtProject
                              ->activeTarget()->activeBuildConfiguration()
                              ->buildDirectory();
}

MbtRuntime::~MbtRuntime()
{
  delete this->_d;
}

void MbtRuntime::resetTcgProcesses()
{
  for(QProcess* tcgProcess : this->_d->_tcgProcesses)
  {
    tcgProcess->disconnect(this);
    tcgProcess->kill();
  }
  qDeleteAll(this->_d->_tcgProcesses);
  this->_d->_tcgProcesses.clear();

  this->_d->_startedTcgProcesses.clear();
  this->_d->_runningTcgProcesses.clear();

  return;
}

void MbtRuntime::addMbtSpecModel(MbtSpecModel* mbtSpecModel)
{
  if(!mbtSpecModel)
    return;

  this->_d->_mbtSpecModels.append(mbtSpecModel);

  return;
}

bool MbtRuntime::prepare(QString *errorMessage)
{
  this->_d->_errorMessages.clear();
  this->resetTcgProcesses();
  this->_d->_tcgCoverageProperties.clear();

  QDir buildDir(this->_d->_buildDirectory);
  if(!buildDir.exists())
  {
    utils::AddPtrString(errorMessage)<<tr("build directory '%1' does not exist!")
                                       .arg(this->_d->_buildDirectory);
    return false;
  }
  utils::FileUtils::removeDir(buildDir, true);///clear build directory

  for(MbtSpecModel* mbtSpecModel : this->_d->_mbtSpecModels)
  {
    //check spec type
    if(mbtSpecModel->specModelType()!=MbtSpecModel::SpenatModel
        && mbtSpecModel->specModelType()!=MbtSpecModel::StateMachineModel)
    {
      utils::AddPtrString(errorMessage)<<tr("file type of spec model file '%1' is not supported! "
                                            "currently only spenat and statemachine files "
                                            "are allowed as input files for test generation!")
                                         .arg(mbtSpecModel->mbtSpecModelFilePath());
      return false;
    }


    if(mbtSpecModel->tcgCoverageProperties().isEmpty())
    {
      utils::AddPtrString(errorMessage)<<tr("there is no tcg coverage specified for spec model '%1'!")
                                         .arg(mbtSpecModel->name());
      return false;
    }

    QDir specModelDir = buildDir;

    //create mbt spec model directory
    bool result = specModelDir.mkdir(mbtSpecModel->name());
    if(!result)
    {
      utils::AddPtrString(errorMessage)<<tr("could not create spec model dir '%1'"
                                            " within build dir '%2'!")
                                         .arg(mbtSpecModel->name())
                                         .arg(this->_d->_buildDirectory);
      return false;
    }

    result=specModelDir.cd(mbtSpecModel->name());
    if(!result)
    {
      utils::AddPtrString(errorMessage)<<tr("could not enter spec model dir '%1'"
                                            " within build dir '%2'!")
                                         .arg(mbtSpecModel->name())
                                         .arg(this->_d->_buildDirectory);
      return false;
    }

    //create input file for tcgapp
    QString tcgInputFileType;
    if(mbtSpecModel->specModelType()==MbtSpecModel::SpenatModel)
      tcgInputFileType = QLatin1String(".spenatjson");
    else if(mbtSpecModel->specModelType()==MbtSpecModel::StateMachineModel)
      tcgInputFileType = QLatin1String(".spenatjson");
    else
    {
      utils::AddPtrString(errorMessage)<<tr("file type of spec model file '%1' is not supported! "
                                            "currently only spenat and statemachine files "
                                            "are allowed as input files for test generation!")
                                         .arg(mbtSpecModel->mbtSpecModelFilePath());
      return false;
    }
    QString tcgInputFileName = mbtSpecModel->name()
                               + tcgInputFileType;
    QFile tcgInputFile(specModelDir.absoluteFilePath(tcgInputFileName));
    if (!tcgInputFile.open(QIODevice::WriteOnly
                           | QIODevice::Text))
    {
      utils::AddPtrString(errorMessage)<<tr("could not open input file '%1'"
                                            " for test generation app!")
                                         .arg(tcgInputFile.fileName());
      return false;
    }

    QTextStream out(&tcgInputFile);

    if(mbtSpecModel->specModelType()==MbtSpecModel::SpenatModel)
       out << mbtSpecModel->toJson();
    else if(mbtSpecModel->specModelType()==MbtSpecModel::StateMachineModel)
    {
      model::spenat::SpenatDeclModel spenatModel;
      model::statemachine::StateMachineModel const* stateMachineModel = qobject_cast<model::statemachine::StateMachineModel const*>(mbtSpecModel->specModel());
      Q_ASSERT(stateMachineModel);
      if(stateMachineModel == nullptr)
      {
        utils::AddPtrString(errorMessage)<<tr("could not cast mbtSpecModel '%1'")
                                              .arg(mbtSpecModel->mbtSpecModelFilePath());
        return false;
      }
      this->_d->_projectDirectory = QFileInfo(mbtSpecModel->mbtSpecModelFilePath()).absolutePath();
      convert::statemachine::ComStateMachineToSpenat comCreateSpenat(&spenatModel,
                                                                     stateMachineModel,
                                                                     mbtSpecModel->name(),
                                                                     this->_d->_projectDirectory);
      result = comCreateSpenat.execute(errorMessage);
      if(result == false)
      {
        utils::AddPtrString(errorMessage)<< "Conversion of statemachine to spenat failed!";
        return false;
      }

      if(comCreateSpenat.hasTrace())
      {
        result = comCreateSpenat.saveSpenatTrace(errorMessage);
        if(result == false)
        {
          utils::AddPtrString(errorMessage)<< "Creation of spenat trace file failed!";
          return false;
        }
      }

      model::spenat::ComCreateSpenatModel command(&spenatModel);
      result = command.execute(errorMessage);
      if(result)
      {
        dslparser::spenat::ComBuildSpenatFile commandWrite(specModelDir.absolutePath() + mbtSpecModel->name() + QStringLiteral(".spenat"), &spenatModel);
        result = commandWrite.execute(errorMessage);
        model::spenat::ComCreateSpenatModel::SpenatModelPtr spenatModelPtr = command.spenatModel();
        Q_ASSERT(spenatModelPtr);
        model::base::JsonDocPtr tcgJsonDocPtr =spenatModelPtr->createJsonDocument();
        Q_ASSERT(tcgJsonDocPtr);
        out << tcgJsonDocPtr->toJson();
//        qDebug()<< tcgJsonDocPtr->toJson();
      }
    }
    else
    {
      utils::AddPtrString(errorMessage)<<tr("file type of spec model file '%1' is not supported! "
                                            "currently only spenat and statemachine files "
                                            "are allowed as input files for test generation!")
                                         .arg(mbtSpecModel->mbtSpecModelFilePath());
      return false;
    }

    tcgInputFile.close();

    mbtSpecModel->setTcgInputFilePath(tcgInputFile.fileName());

    for(TcgCoverageProperty* tcgCoverageProp : mbtSpecModel->tcgCoverageProperties())
    {
      this->_d->_tcgCoverageProperties.append(tcgCoverageProp);

      QString tcgOutputFileName = mbtSpecModel->name()
                                  + QLatin1String("_")
                                  + tcgCoverageProp->tcgCoverage()
                                  + QLatin1String(".ats");
      tcgCoverageProp->setTcgOutputFilePath(specModelDir.absoluteFilePath(tcgOutputFileName));
    }
  }


  this->_d->_mbtRuntimeState = Prepared;

  return true;
}

void MbtRuntime::start()
{
//  qDebug()<<"MbtRuntime::start() executed!";

  if(this->_d->_mbtRuntimeState != Prepared)
  {
    QString errorMessage;
    bool result = this->prepare(&errorMessage);
    if(!result)
    {
      emit started(false,
                   QStringList()<<tr("mbt runtime is not prepared! error: %1").arg(errorMessage));
      return;
    }
  }

  if(this->_d->_tcgCoverageProperties.isEmpty())
  {
    emit started(false,
                 QStringList()<<tr("for any enabled spec model "
                                   "there is no tcg coverage specified!"));
    return;
  }

  for(TcgCoverageProperty const* tcgCoverageProp
      : this->_d->_tcgCoverageProperties)
  {
    //create new tcg-process
    QProcess* newTcgProcess = new QProcess(this);
    newTcgProcess->closeWriteChannel();
    newTcgProcess->setProcessChannelMode(QProcess::MergedChannels);
    this->_d->_tcgProcesses.append(newTcgProcess);

    //connect process-signals to respective slots
    connect(newTcgProcess, &QProcess::started,
            this, &MbtRuntime::tcgProcessStartedSlot,
            Qt::QueuedConnection);
    connect(newTcgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MbtRuntime::tcgProcessFinishedSlot,
            Qt::QueuedConnection);
    connect(newTcgProcess, &QProcess::errorOccurred,
            this, &MbtRuntime::tcgProcessErrorSlot,
            Qt::QueuedConnection);
    connect(newTcgProcess, &QProcess::readyReadStandardOutput,
            this, &MbtRuntime::tcgProcessStandardOutputSlot,
            Qt::QueuedConnection);
    connect(newTcgProcess, &QProcess::readyReadStandardError,
            this, &MbtRuntime::tcgProcessStandardErrorSlot,
            Qt::QueuedConnection);

    MbtSpecModel* mbtSpecModel = tcgCoverageProp->mbtSpecModel();
    Q_ASSERT(mbtSpecModel);

    //start each process (with timeout?)
    QStringList tcgAppArguments;
    tcgAppArguments << QLatin1String("--input") << mbtSpecModel->tcgInputFilePath();
    tcgAppArguments << QLatin1String("--coverage") << tcgCoverageProp->tcgCoverage();
    tcgAppArguments << QLatin1String("--output") << tcgCoverageProp->tcgOutputFilePath();
    tcgAppArguments << QLatin1String("--log") << (this->_d->_buildDirectory
                                                  + QLatin1String("/") + mbtSpecModel->name()
                                                  + QLatin1String("/")+ mbtSpecModel->name()
                                                  + QLatin1String("_%N.log"));
    tcgAppArguments << QLatin1String("--logLevel") << QLatin1String("debug");
    tcgAppArguments << QLatin1String("--report") << (this->_d->_buildDirectory
                                                  + QLatin1String("/") + mbtSpecModel->name()
                                                  + QLatin1String("/")+ mbtSpecModel->name()
                                                  + QLatin1String(".testreport"));

    this->_d->_startedTcgProcesses.append(newTcgProcess);
    this->_d->_runningTcgProcesses.append(newTcgProcess);

    newTcgProcess->start(this->_d->_tcgApp, tcgAppArguments);
  }

  return;
}

void MbtRuntime::tcgProcessStartedSlot()
{
//  qDebug()<<"MbtRuntime::tcgProcessStartedSlot() executed!";

  QProcess* startedTcgProcess = qobject_cast<QProcess*>(this->sender());
  Q_ASSERT(startedTcgProcess);

  this->_d->_startedTcgProcesses.removeAll(startedTcgProcess);

  if(this->_d->_startedTcgProcesses.isEmpty())
  {
    emit started(true);
    this->_d->_mbtRuntimeState = Started;
  }

  return;
}

void MbtRuntime::stop()
{
  if(this->_d->_mbtRuntimeState != Started)
  {
    emit stopped(false,
                 QStringList()<<tr("mbt runtime is not started yet!"));
    return;
  }

  this->resetTcgProcesses();

  emit stopped(true, QStringList()<<tr("test generation was succesfully stopped!"));

  this->_d->_mbtRuntimeState = Stopped;

  return;
}

void MbtRuntime::tcgProcessFinishedSlot(int exitCode, QProcess::ExitStatus exitStatus)
{
//  qDebug()<<"MbtRuntime::tcgProcessFinishedSlot() executed!";

  QProcess* finishedTcgProcess = qobject_cast<QProcess*>(this->sender());
  if(!finishedTcgProcess)
    return;//error slot was called before...

  TcgCoverageProperty* tcgCoverageProp = this->_d->tcgCoveragePropertyForTcgProcess(finishedTcgProcess);
  Q_ASSERT(tcgCoverageProp);

  MbtSpecModel* tcgCoverageSpecModel = tcgCoverageProp->mbtSpecModel();
  Q_ASSERT(tcgCoverageSpecModel);

  if(exitStatus == QProcess::CrashExit)
  {
    this->_d->_mbtRuntimeState = Error;
    this->_d->_errorMessages.append(tr("tcg process for tcg coverage '%1' "
                                       "of spec model '%2' is crashed!")
                                    .arg(tcgCoverageProp->tcgCoverage())
                                    .arg(tcgCoverageSpecModel->name())
                                    +finishedTcgProcess->errorString());
  }
  else//exitStatus == QProcess::NormalExit
  {
    if(exitCode != 0)
    {
      this->_d->_mbtRuntimeState = Error;
      this->_d->_errorMessages.append(tr("tcg process for tcg coverage '%1' "
                                         "of spec model '%2' is not normally exited!")
                                      .arg(tcgCoverageProp->tcgCoverage())
                                      .arg(tcgCoverageSpecModel->name())
                                      +finishedTcgProcess->errorString());
    }
    else
    {//create generated test suite item

      QString errorMessage;
      for(MbtSpecModel* mbtSpecModel : this->_d->_mbtSpecModels)
      {
        QString traceFilePath = QFileInfo(mbtSpecModel->mbtSpecModelFilePath()).absolutePath()
                                + QStringLiteral("/")
                                + mbtSpecModel->name()
                                + QLatin1String(".spenattrace");

        QString tcTraceName = QFileInfo(tcgCoverageProp->tcgOutputFilePath()).baseName();
        modtrace::ComCreateModeTrace comCreateModTrace(traceFilePath,
                                                       tcgCoverageProp->tcgOutputFilePath(),
                                                       tcTraceName,
                                                       this->_d->_projectDirectory);
        if(!comCreateModTrace.execute(&errorMessage))
        {
          this->_d->_errorMessages.append(tr("Error while generating test case trace file:")
                                          +errorMessage);
        }
      }

      model::ts::TestSuiteItem* generatedTestSuiteItem = new model::ts::TestSuiteItem();
      model::ts::io::ComReadXmlTestSuiteItem readCommand(tcgCoverageProp->tcgOutputFilePath(),
                                                         generatedTestSuiteItem);
      bool result = readCommand.execute(&errorMessage);
      if(result)
      {
        generatedTestSuiteItem->setName(tcgCoverageSpecModel->name()
                                        + QLatin1String("_testsuite"));
        int testStepsCount=generatedTestSuiteItem->calcTestStepsCount();
        generatedTestSuiteItem->setText(generatedTestSuiteItem->toString()
                                        +QString(QLatin1String(", %1 test steps")).arg(testStepsCount));
        tcgCoverageProp->setTcgTestSuiteItem(generatedTestSuiteItem);
        this->_d->_mbtProject->mbtProjectModel()->updateMbtSpecModel(tcgCoverageSpecModel);
      }
      else
      {
        delete generatedTestSuiteItem; generatedTestSuiteItem = nullptr;
        this->_d->_mbtRuntimeState = Error;

        this->_d->_errorMessages.append(tr("Error while reading generated test suite file '%1' "
                                           "for tcg coverage '%2' "
                                           "of spec model '%3': ")
                                        .arg(tcgCoverageProp->tcgOutputFilePath())
                                        .arg(tcgCoverageProp->tcgCoverage())
                                        .arg(tcgCoverageSpecModel->name()));
        this->_d->_errorMessages.append(errorMessage);
      }
    }
  }

  finishedTcgProcess->disconnect(this);
  finishedTcgProcess->kill();
  this->_d->_runningTcgProcesses.removeAll(finishedTcgProcess);

  if(this->_d->_runningTcgProcesses.isEmpty())
  {
    if(this->_d->_mbtRuntimeState == Started)
    {
      emit finished(true);
    }
    else if(this->_d->_mbtRuntimeState == Error)
    {
      emit finished(false, this->_d->_errorMessages);
    }
  }

  return;
}

void MbtRuntime::tcgProcessErrorSlot(QProcess::ProcessError processError)
{
//  qDebug()<<"MbtRuntime::tcgProcessErrorSlot() executed!";
  Q_UNUSED(processError);

  QProcess* errorTcgProcess = qobject_cast<QProcess*>(this->sender());
  Q_ASSERT(errorTcgProcess);

  TcgCoverageProperty const* tcgCoverageProp = this->_d->tcgCoveragePropertyForTcgProcess(errorTcgProcess);
  Q_ASSERT(tcgCoverageProp);

  MbtSpecModel const* tcgCoverageSpecModel = tcgCoverageProp->mbtSpecModel();
  Q_ASSERT(tcgCoverageSpecModel);

  this->_d->_mbtRuntimeState = Error;
  this->_d->_errorMessages.append(tr("an error occured within tcg process "
                                     "for tcg coverage '%1' "
                                     "of spec model '%2'!")
                                  .arg(tcgCoverageProp->tcgCoverage())
                                  .arg(tcgCoverageSpecModel->name())
                                  +errorTcgProcess->errorString());

  errorTcgProcess->disconnect(this);
  errorTcgProcess->kill();
  this->_d->_runningTcgProcesses.removeAll(errorTcgProcess);

  if(this->_d->_runningTcgProcesses.isEmpty())
  {
    emit finished(false, this->_d->_errorMessages);
  }

  return;
}

void MbtRuntime::tcgProcessStandardOutputSlot()
{
//  qDebug()<<"MbtRuntime::tcgProcessStandardOutputSlot() executed!";

  QProcess* tcgProcess = qobject_cast<QProcess*>(this->sender());
  Q_ASSERT(tcgProcess);

  TcgCoverageProperty const* tcgCoverageProp = this->_d->tcgCoveragePropertyForTcgProcess(tcgProcess);
  Q_ASSERT(tcgCoverageProp);

  MbtSpecModel const* tcgCoverageSpecModel = tcgCoverageProp->mbtSpecModel();
  Q_ASSERT(tcgCoverageSpecModel);

  emit this->runtimeMessage(true,
                            tr("tcg coverage '%1' of spec model '%2':\n")
                            .arg(tcgCoverageProp->tcgCoverage())
                            .arg(tcgCoverageSpecModel->name())
                            +QLatin1String(tcgProcess->readAllStandardOutput()));

  return;
}

void MbtRuntime::tcgProcessStandardErrorSlot()
{
//  qDebug()<<"MbtRuntime::tcgProcessStandardErrorSlot() executed!";

  QProcess* tcgProcess = qobject_cast<QProcess*>(this->sender());
  Q_ASSERT(tcgProcess);

  TcgCoverageProperty const* tcgCoverageProp = this->_d->tcgCoveragePropertyForTcgProcess(tcgProcess);
  Q_ASSERT(tcgCoverageProp);

  MbtSpecModel const* tcgCoverageSpecModel = tcgCoverageProp->mbtSpecModel();
  Q_ASSERT(tcgCoverageSpecModel);

  emit this->runtimeMessage(false,
                            tr("tcg coverage '%1' of spec model '%2': ")
                            .arg(tcgCoverageProp->tcgCoverage())
                            .arg(tcgCoverageSpecModel->name())
                            +QLatin1String(tcgProcess->readAllStandardError()));

  return;
}

} // namespace mbtproject
