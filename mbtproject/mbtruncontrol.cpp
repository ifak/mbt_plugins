#include "mbtruncontrol.hpp"

#include "mbtrunconfiguration.hpp"
#include "mbtprojectconstants.hpp"
#include "mbtproject.hpp"
#include "mbtbuildconfiguration.hpp"
#include "mbtruntime.hpp"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/messagemanager.h>

#include <projectexplorer/target.h>
#include <projectexplorer/task.h>
#include <projectexplorer/projectexplorer.h>

#include <QIcon>
#include <QDate>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

///MbtRunControl/////////////////////

class MbtRunControl::Private
{
  friend class MbtRunControl;

  bool        _isRunning;
  QIcon       _runControlIcon;
  MbtRuntime* _mbtRuntime;

  Private(const QIcon& runControlIcon)
    : _isRunning(false),
      _runControlIcon(runControlIcon)
  {}
};

MbtRunControl::MbtRunControl(MbtRunConfiguration* mbtRunConfiguration,
                             ProjectExplorer::RunMode mode,
                             const QIcon& runControlIcon)
  : ProjectExplorer::RunControl(mbtRunConfiguration, mode),
    _d(new Private(runControlIcon))
{
  Q_ASSERT(mbtRunConfiguration);

  MbtBuildConfiguration* mbtBC = qobject_cast<MbtBuildConfiguration*>(mbtRunConfiguration
                                                                      ->target()
                                                                      ->activeBuildConfiguration());
  Q_ASSERT(mbtBC);

  this->_d->_mbtRuntime = mbtBC->mbtRuntime();
  Q_ASSERT(this->_d->_mbtRuntime);

  connect(this->_d->_mbtRuntime, &MbtRuntime::started,
          this, &MbtRunControl::startedSlot,
          Qt::QueuedConnection);
  connect(this->_d->_mbtRuntime, &MbtRuntime::stopped,
          this, &MbtRunControl::stoppedSlot,
          Qt::QueuedConnection);
  connect(this->_d->_mbtRuntime, &MbtRuntime::finished,
          this, &MbtRunControl::finishedSlot,
          Qt::QueuedConnection);
  connect(this->_d->_mbtRuntime, &MbtRuntime::runtimeMessage,
          this, &MbtRunControl::runtimeMessageSlot,
          Qt::QueuedConnection);
}

MbtRunControl::~MbtRunControl()
{
  delete this->_d;
}

void MbtRunControl::start()
{
  this->_d->_isRunning = true;
  emit started();

  ProjectExplorer::RunConfiguration* runConfig=this->runConfiguration();
  Q_ASSERT(runConfig);
  Q_ASSERT(runConfig->target());

  ProjectExplorer::BuildConfiguration* buildConfig = runConfig->target()->activeBuildConfiguration();
  Q_ASSERT(buildConfig);
  MbtBuildConfiguration* mbtBuildConfig = qobject_cast<MbtBuildConfiguration*>(buildConfig);
  Q_ASSERT(mbtBuildConfig);
  mbtBuildConfig->setEnabled(false);

  MbtRunConfiguration* mbtRunConfig = qobject_cast<MbtRunConfiguration*>(runConfig);
  Q_ASSERT(mbtRunConfig);
  mbtRunConfig->setEnabled(false);

  this->_d->_mbtRuntime->start();

  return;
}

void MbtRunControl::startedSlot(bool result, const QStringList& messages)
{
//  qDebug()<<"MbtRunControl::startedSlot() executed!";

  Utils::OutputFormat messageFormat = Utils::NormalMessageFormat;
  if(!result)
    messageFormat = Utils::ErrorMessageFormat;

  QString currTimeString = QTime::currentTime().toString(QLatin1String("hh:mm:ss.zzz"));
  for(const QString& message : messages)
    this->appendMessage(currTimeString
                        + QStringLiteral(": ")
                        + message
                        +QStringLiteral("\n"),
                        messageFormat);

  if(!result)
  {
    this->_d->_isRunning=false;
    emit finished();

    ProjectExplorer::RunConfiguration* runConfig=this->runConfiguration();
    Q_ASSERT(runConfig);
    Q_ASSERT(runConfig->target());

    ProjectExplorer::BuildConfiguration* buildConfig = runConfig->target()->activeBuildConfiguration();
    Q_ASSERT(buildConfig);
    MbtBuildConfiguration* mbtBuildConfig = qobject_cast<MbtBuildConfiguration*>(buildConfig);
    Q_ASSERT(mbtBuildConfig);
    mbtBuildConfig->setEnabled(true);

    MbtRunConfiguration* mbtRunConfig = qobject_cast<MbtRunConfiguration*>(runConfig);
    Q_ASSERT(mbtRunConfig);
    mbtRunConfig->setEnabled(true);
  }

  return;
}

MbtRunControl::StopResult MbtRunControl::stop()
{
  this->_d->_mbtRuntime->stop();

  return AsynchronousStop;
}

void MbtRunControl::stoppedSlot(bool result, const QStringList& messages)
{
//  qDebug()<<"MbtRunControl::stoppedSlot() executed!";

  Utils::OutputFormat messageFormat = Utils::NormalMessageFormat;
  if(!result)
    messageFormat = Utils::ErrorMessageFormat;

  QString currTimeString = QTime::currentTime().toString(QLatin1String("hh:mm:ss.zzz"));
  for(const QString& message : messages)
    this->appendMessage(currTimeString
                        + QStringLiteral(": ")
                        + message
                        +QStringLiteral("\n"),
                        messageFormat);

  this->_d->_isRunning=false;
  emit finished();

  ProjectExplorer::RunConfiguration* runConfig=this->runConfiguration();
  Q_ASSERT(runConfig);
  Q_ASSERT(runConfig->target());

  ProjectExplorer::BuildConfiguration* buildConfig = runConfig->target()->activeBuildConfiguration();
  Q_ASSERT(buildConfig);
  MbtBuildConfiguration* mbtBuildConfig = qobject_cast<MbtBuildConfiguration*>(buildConfig);
  Q_ASSERT(mbtBuildConfig);
  mbtBuildConfig->setEnabled(true);

  MbtRunConfiguration* mbtRunConfig = qobject_cast<MbtRunConfiguration*>(runConfig);
  Q_ASSERT(mbtRunConfig);
  mbtRunConfig->setEnabled(true);

  return;
}

void MbtRunControl::finishedSlot(bool result, const QStringList& messages)
{
//  qDebug()<<"MbtRunControl::finishedSlot() executed!";

  Utils::OutputFormat messageFormat = Utils::NormalMessageFormat;
  if(!result)
    messageFormat = Utils::ErrorMessageFormat;

  QString currTimeString = QTime::currentTime().toString(QLatin1String("hh:mm:ss.zzz"));
  for(const QString& message : messages)
    this->appendMessage(currTimeString
                        + QStringLiteral(": ")
                        + message
                        +QStringLiteral("\n"),
                        messageFormat);

  this->_d->_isRunning=false;
  emit finished();

  ProjectExplorer::RunConfiguration* runConfig=this->runConfiguration();
  Q_ASSERT(runConfig);
  Q_ASSERT(runConfig->target());

  ProjectExplorer::BuildConfiguration* buildConfig = runConfig->target()->activeBuildConfiguration();
  Q_ASSERT(buildConfig);
  MbtBuildConfiguration* mbtBuildConfig = qobject_cast<MbtBuildConfiguration*>(buildConfig);
  Q_ASSERT(mbtBuildConfig);
  mbtBuildConfig->setEnabled(true);

  MbtRunConfiguration* mbtRunConfig = qobject_cast<MbtRunConfiguration*>(this->runConfiguration());
  Q_ASSERT(mbtRunConfig);
  mbtRunConfig->setEnabled(true);

  emit finished();

  return;
}

void MbtRunControl::runtimeMessageSlot(bool success, const QString& message)
{
//  qDebug()<<"MbtRunControl::runtimeMessageSlot() executed!";

  Utils::OutputFormat messageFormat = Utils::NormalMessageFormat;
  if(!success)
    messageFormat = Utils::ErrorMessageFormat;

  QString currTimeString = QTime::currentTime().toString(QLatin1String("hh:mm:ss.zzz"));
  this->appendMessage(currTimeString
                      + QStringLiteral(": ")
                      + message
                      +QStringLiteral("\n"),
                      messageFormat);

  return;
}

bool MbtRunControl::isRunning() const
{
  return this->_d->_isRunning;
}

QIcon MbtRunControl::icon() const
{
  return this->_d->_runControlIcon;
}

///MbtRunControlFactory/////////////////////
class MbtRunControlFactory::Private
{
  friend class MbtRunControlFactory;

  const Core::Id _runId;

  Private(const Core::Id& runId)
    : _runId(runId)
  {}
};

MbtRunControlFactory::MbtRunControlFactory(QObject* parent)
  : ProjectExplorer::IRunControlFactory(parent),
    _d(new Private(Constants::MBT_RUN))
{}

MbtRunControlFactory::~MbtRunControlFactory()
{
  delete this->_d;
}

bool MbtRunControlFactory::canRun(ProjectExplorer::RunConfiguration* runConfiguration,
                                  ProjectExplorer::RunMode mode) const
{
  Q_ASSERT(runConfiguration);

  if(runConfiguration->id()==this->_d->_runId
     && mode == ProjectExplorer::NormalRunMode)
    return true;

  return false;
}

MbtRunControl* MbtRunControlFactory::create(ProjectExplorer::RunConfiguration* runConfiguration,
                                            ProjectExplorer::RunMode mode,
                                            QString* errorMessage)
{
  Q_ASSERT(runConfiguration);

  if(!this->canRun(runConfiguration, mode))
  {
    if(errorMessage)
      *errorMessage+=tr("run-configuration '%1' cannot be handled "
                        "by this test system!").arg(runConfiguration->displayName());
    return 0;
  }

  MbtRunConfiguration* mbtRunConfig=qobject_cast<MbtRunConfiguration*>(runConfiguration);
  if(!mbtRunConfig)
  {
    if(errorMessage)
      *errorMessage+=tr("run-configuration '%1' is not an "
                        "mbt run configuration!").arg(runConfiguration->displayName());
    return 0;
  }

  MbtBuildConfiguration* mbtBC = qobject_cast<MbtBuildConfiguration*>(mbtRunConfig
                                                                      ->target()
                                                                      ->activeBuildConfiguration());
  if(!mbtBC)
  {
    if(errorMessage)
      *errorMessage+=tr("mbt run configuration '%1' has no mbt build configuration!")
                     .arg(runConfiguration->displayName());
    return 0;
  }

  //  if(!mbtBC->mbtRuntime())
  //  {
  //    if(errorMessage)
  //      *errorMessage += tr("there is no dis-test-runtime available. please build your project first!");

  //    return 0;
  //  }

  return new MbtRunControl(mbtRunConfig, mode);
}

} // namespace mbtproject
