#include "specsimruntime.hpp"

//#include "mbtproject.hpp"
//#include "mbtprojectmodel.hpp"
//#include "mbtspenatmodel.hpp"
//#include "mbtstatemachinemodel.hpp"
//#include "tcgcoverageproperty.hpp"

//#include <mobata/model/ts/testsuiteitem.hpp>
//#include <mobata/model/ts/io/comreadxmltestsuiteitem.hpp>

//#include <mobata/model/spenat/spenatmodel.hpp>
//#include <mobata/model/spenat/comcreatespenatmodel.hpp>
//#include <mobata/convert/statemachine/comstatemachinetospenat.hpp>

//#include <mobata/utils/fileutils.hpp>

//#include <projectexplorer/target.h>
//#include <projectexplorer/buildconfiguration.h>

//#include <QDir>
//#include <QTextStream>
//#include <QDebug>

//#include <QJsonDocument>

namespace specsim {

class SpecSimRuntime::Private
{
  friend class SpecSimRuntime;

  SpecSimRuntime::SpecSimRuntimeState _specSimRuntimeState;
  SpecSimProject*                     _specSimProject;
  QList<SpecSimModel*>                _specSimModels;
  QStringList                         _errorMessages;

  Private(SpecSimProject* specSimProject)
    : _specSimRuntimeState(SpecSimRuntime::Init),
      _specSimProject(specSimProject)
  {}
};

SpecSimRuntime::SpecSimRuntime(SpecSimProject* specSimProject,
                               QObject* parent)
  : QObject(parent),
    _d(new Private(specSimProject))
{
  Q_ASSERT(this->_d->_specSimProject);
}

SpecSimRuntime::~SpecSimRuntime()
{
  delete this->_d;
}

void SpecSimRuntime::addSpecSimModel(SpecSimModel* specSimModel)
{
  if(!specSimModel)
    return;

  this->_d->_specSimModels.append(specSimModel);

  return;
}

bool SpecSimRuntime::prepare(QString *errorMessage)
{
  Q_UNUSED(errorMessage);
  this->_d->_errorMessages.clear();

//  for(MbtSpecModel* mbtSpecModel : this->_d->_mbtSpecModels)
//  {
//    //check spec type
//    if(mbtSpecModel->specModelType()!=MbtSpecModel::SpenatModel
//       && mbtSpecModel->specModelType()!=MbtSpecModel::StateMachineModel)
//    {
//      utils::AddPtrString(errorMessage)<<tr("file type of spec model file '%1' is not supported! "
//                                            "currently only spenat and statemachine files "
//                                            "are allowed as input files for test generation!")
//                                         .arg(mbtSpecModel->mbtSpecModelFilePath());
//      return false;
//    }


//    if(mbtSpecModel->tcgCoverageProperties().isEmpty())
//    {
//      utils::AddPtrString(errorMessage)<<tr("there is no tcg coverage specified for spec model '%1'!")
//                                         .arg(mbtSpecModel->name());
//      return false;
//    }

//    QDir specModelDir = buildDir;

//    //create mbt spec model directory
//    bool result = specModelDir.mkdir(mbtSpecModel->name());
//    if(!result)
//    {
//      utils::AddPtrString(errorMessage)<<tr("could not create spec model dir '%1'"
//                                            " within build dir '%2'!")
//                                         .arg(mbtSpecModel->name())
//                                         .arg(this->_d->_buildDirectory);
//      return false;
//    }

//    result=specModelDir.cd(mbtSpecModel->name());
//    if(!result)
//    {
//      utils::AddPtrString(errorMessage)<<tr("could not entered spec model dir '%1'"
//                                            " within build dir '%2'!")
//                                         .arg(mbtSpecModel->name())
//                                         .arg(this->_d->_buildDirectory);
//      return false;
//    }

//    //create input file for tcgapp
//    QString tcgInputFileType;
//    if(mbtSpecModel->specModelType()==MbtSpecModel::SpenatModel)
//      tcgInputFileType = QLatin1String(".spenatjson");
//    else if(mbtSpecModel->specModelType()==MbtSpecModel::StateMachineModel)
//      tcgInputFileType = QLatin1String(".statemachinejson");
//    else
//    {
//      utils::AddPtrString(errorMessage)<<tr("file type of spec model file '%1' is not supported! "
//                                            "currently only spenat and statemachine files "
//                                            "are allowed as input files for test generation!")
//                                         .arg(mbtSpecModel->mbtSpecModelFilePath());
//      return false;
//    }
//    QString tcgInputFileName = mbtSpecModel->name()
//                               + tcgInputFileType;
//    QFile tcgInputFile(specModelDir.absoluteFilePath(tcgInputFileName));
//    if (!tcgInputFile.open(QIODevice::WriteOnly
//                           | QIODevice::Text))
//    {
//      utils::AddPtrString(errorMessage)<<tr("could not open input file '%1'"
//                                            " for test generation app!")
//                                         .arg(tcgInputFile.fileName());
//      return false;
//    }

//    QTextStream out(&tcgInputFile);

//    if(mbtSpecModel->specModelType()==MbtSpecModel::SpenatModel)
//      out << mbtSpecModel->toJson();
//    else if(mbtSpecModel->specModelType()==MbtSpecModel::StateMachineModel)
//    {
//      model::spenat::SpenatDeclModel spenatModel;
//      model::statemachine::StateMachineModel const* stateMachineModel = qobject_cast<model::statemachine::StateMachineModel const*>(mbtSpecModel->specModel());
//      Q_ASSERT(stateMachineModel);
//      if(stateMachineModel == nullptr)
//      {
//        utils::AddPtrString(errorMessage)<<tr("could not cast mbtSpecModel '%1'")
//                                           .arg(mbtSpecModel->mbtSpecModelFilePath());
//        return false;
//      }

//      convert::statemachine::ComStateMachineToSpenat comCreateSpenat(&spenatModel, stateMachineModel);
//      result = comCreateSpenat.execute(errorMessage);
//      if(result == false)
//      {
//        return false;
//      }

//      model::spenat::ComCreateSpenatModel command(&spenatModel);
//      result = command.execute(errorMessage);
//      if(result)
//      {
//        model::spenat::ComCreateSpenatModel::SpenatModelPtr spenatModelPtr = command.spenatModel();
//        Q_ASSERT(spenatModelPtr);
//        model::base::JsonDocPtr tcgJsonDocPtr =spenatModelPtr->createJsonDocument();
//        Q_ASSERT(tcgJsonDocPtr);
//        QString output;
//        output = QLatin1String(tcgJsonDocPtr->toJson());
//        qDebug()<< output;
//        out << output;
//        //out << QLatin1String(tcgJsonDocPtr->toJson());
//      }
//    }
//    else
//    {
//      utils::AddPtrString(errorMessage)<<tr("file type of spec model file '%1' is not supported! "
//                                            "currently only spenat and statemachine files "
//                                            "are allowed as input files for test generation!")
//                                         .arg(mbtSpecModel->mbtSpecModelFilePath());
//      return false;
//    }

//    tcgInputFile.close();

//    mbtSpecModel->setTcgInputFilePath(tcgInputFile.fileName());

//    for(TcgCoverageProperty* tcgCoverageProp : mbtSpecModel->tcgCoverageProperties())
//    {
//      this->_d->_tcgCoverageProperties.append(tcgCoverageProp);

//      QString tcgOutputFileName = mbtSpecModel->name()
//                                  + QLatin1String("_")
//                                  + tcgCoverageProp->tcgCoverage()
//                                  + QLatin1String(".ats");
//      tcgCoverageProp->setTcgOutputFilePath(specModelDir.absoluteFilePath(tcgOutputFileName));
//    }
//  }


  this->_d->_specSimRuntimeState = Prepared;

  return true;
}

void SpecSimRuntime::start()
{
  //  qDebug()<<"SpecSimRuntime::start() executed!";

  if(this->_d->_specSimRuntimeState != Prepared)
  {
    QString errorMessage;
    bool result = this->prepare(&errorMessage);
    if(!result)
    {
      emit started(false,
                   QStringList()<<tr("model simulation runtime is not prepared! error: %1").arg(errorMessage));
      return;
    }
  }

//  for(TcgCoverageProperty const* tcgCoverageProp
//      : this->_d->_tcgCoverageProperties)
//  {
//    //create new tcg-process
//    QProcess* newTcgProcess = new QProcess(this);
//    newTcgProcess->closeWriteChannel();
//    newTcgProcess->setProcessChannelMode(QProcess::MergedChannels);
//    this->_d->_tcgProcesses.append(newTcgProcess);

//    //connect process-signals to respective slots
//    connect(newTcgProcess, &QProcess::started,
//            this, &MbtRuntime::tcgProcessStartedSlot,
//            Qt::QueuedConnection);
//    connect(newTcgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
//            this, &MbtRuntime::tcgProcessFinishedSlot,
//            Qt::QueuedConnection);
//    connect(newTcgProcess, &QProcess::errorOccurred,
//            this, &MbtRuntime::tcgProcessErrorSlot,
//            Qt::QueuedConnection);
//    connect(newTcgProcess, &QProcess::readyReadStandardOutput,
//            this, &MbtRuntime::tcgProcessStandardOutputSlot,
//            Qt::QueuedConnection);
//    connect(newTcgProcess, &QProcess::readyReadStandardError,
//            this, &MbtRuntime::tcgProcessStandardErrorSlot,
//            Qt::QueuedConnection);

//    MbtSpecModel* mbtSpecModel = tcgCoverageProp->mbtSpecModel();
//    Q_ASSERT(mbtSpecModel);

//    //start each process (with timeout?)
//    QStringList tcgAppArguments;
//    tcgAppArguments << QLatin1String("--input") << mbtSpecModel->tcgInputFilePath();
//    tcgAppArguments << QLatin1String("--coverage") << tcgCoverageProp->tcgCoverage();
//    tcgAppArguments << QLatin1String("--output") << tcgCoverageProp->tcgOutputFilePath();
//    tcgAppArguments << QLatin1String("--log") << (this->_d->_buildDirectory
//                                                  + QLatin1String("/") + mbtSpecModel->name()
//                                                  + QLatin1String("/")+ mbtSpecModel->name()
//                                                  + QLatin1String("_%N.log"));
//    tcgAppArguments << QLatin1String("--report") << (this->_d->_buildDirectory
//                                                     + QLatin1String("/") + mbtSpecModel->name()
//                                                     + QLatin1String("/")+ mbtSpecModel->name()
//                                                     + QLatin1String(".testreport"));

//    this->_d->_startedTcgProcesses.append(newTcgProcess);
//    this->_d->_runningTcgProcesses.append(newTcgProcess);

//    newTcgProcess->start(this->_d->_tcgApp, tcgAppArguments);
//  }

  return;
}

void SpecSimRuntime::runtimeStartedSlot(bool result, const QStringList &messages)
{
  //  qDebug()<<"SpecSimRuntime::runtimeStartedSlot() executed!";

  if(result)
  {
    emit started(true);
    this->_d->_specSimRuntimeState = Started;
  }
  else
  {
    emit started(false, messages);
    this->_d->_specSimRuntimeState = Error;
  }

  return;
}

void SpecSimRuntime::stop()
{
  if(this->_d->_specSimRuntimeState != Started)
  {
    emit stopped(false,
                 QStringList()<<tr("model simulation runtime is not started yet!"));
    return;
  }

  emit stopped(true, QStringList()<<tr("model simulation runtime was succesfully stopped!"));

  this->_d->_specSimRuntimeState = Stopped;

  return;
}

void SpecSimRuntime::runtimeFinishedSlot(bool result, const QStringList& messages)
{
  //  qDebug()<<"SpecSimRuntime::runtimeFinishedSlot() executed!";

  if(result)
  {
    emit finished(true);
    this->_d->_specSimRuntimeState = Finished;
  }
  else
  {
    emit finished(false, messages);
    this->_d->_specSimRuntimeState = Error;
  }

  return;
}

} // namespace specsim
