#include "specsimruncontrol.hpp"

#include "specsimrunconfiguration.hpp"
#include "specsimconstants.hpp"
#include "specsimproject.hpp"
#include "specsimbuildconfiguration.hpp"
//#include "specsimruntime.hpp"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/messagemanager.h>

#include <projectexplorer/target.h>
#include <projectexplorer/task.h>
#include <projectexplorer/projectexplorer.h>

#include <QIcon>
#include <QDate>
#include <QDebug>

namespace specsim {

///SpecSimRunControl/////////////////////

class SpecSimRunControl::Private
{
  friend class SpecSimRunControl;

  bool            _isRunning;
  QIcon           _runControlIcon;
  SpecSimRuntime* _specSimRuntime;

  Private(const QIcon& runControlIcon)
    : _isRunning(false),
      _runControlIcon(runControlIcon)
  {}
};

SpecSimRunControl::SpecSimRunControl(SpecSimRunConfiguration* specSimRunConfiguration,
                                     ProjectExplorer::RunMode mode,
                                     const QIcon& runControlIcon)
  : ProjectExplorer::RunControl(specSimRunConfiguration, mode),
    _d(new Private(runControlIcon))
{
  Q_ASSERT(specSimRunConfiguration);

  SpecSimBuildConfiguration* specSimBC = qobject_cast<SpecSimBuildConfiguration*>(specSimRunConfiguration
                                                                                  ->target()
                                                                                  ->activeBuildConfiguration());
  Q_ASSERT(specSimBC);

//  this->_d->_specSimRuntime = specSimBC->specSimRuntime();
//  Q_ASSERT(this->_d->_specSimRuntime);

//  connect(this->_d->_specSimRuntime, &SpecSimRuntime::started,
//          this, &SpecSimRunControl::startedSlot,
//          Qt::QueuedConnection);
//  connect(this->_d->_specSimRuntime, &SpecSimRuntime::stopped,
//          this, &SpecSimRunControl::stoppedSlot,
//          Qt::QueuedConnection);
//  connect(this->_d->_specSimRuntime, &SpecSimRuntime::finished,
//          this, &SpecSimRunControl::finishedSlot,
//          Qt::QueuedConnection);
//  connect(this->_d->_specSimRuntime, &SpecSimRuntime::runtimeMessage,
//          this, &SpecSimRunControl::runtimeMessageSlot,
//          Qt::QueuedConnection);
}

SpecSimRunControl::~SpecSimRunControl()
{
  delete this->_d;
}

void SpecSimRunControl::start()
{
  this->_d->_isRunning = true;
  emit started();

  ProjectExplorer::RunConfiguration* runConfig=this->runConfiguration();
  Q_ASSERT(runConfig);
  Q_ASSERT(runConfig->target());

  ProjectExplorer::BuildConfiguration* buildConfig = runConfig->target()->activeBuildConfiguration();
  Q_ASSERT(buildConfig);
  SpecSimBuildConfiguration* specSimBuildConfig = qobject_cast<SpecSimBuildConfiguration*>(buildConfig);
  Q_ASSERT(specSimBuildConfig);
  specSimBuildConfig->setEnabled(false);

  SpecSimRunConfiguration* specSimRunConfig = qobject_cast<SpecSimRunConfiguration*>(runConfig);
  Q_ASSERT(specSimRunConfig);
  specSimRunConfig->setEnabled(false);

//  this->_d->_mbtRuntime->start();

  return;
}

void SpecSimRunControl::startedSlot(bool result, const QStringList& messages)
{
//  qDebug()<<"SpecSimRunControl::startedSlot() executed!";

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
    SpecSimBuildConfiguration* specSimBuildConfig = qobject_cast<SpecSimBuildConfiguration*>(buildConfig);
    Q_ASSERT(specSimBuildConfig);
    specSimBuildConfig->setEnabled(true);

    SpecSimRunConfiguration* specSimRunConfig = qobject_cast<SpecSimRunConfiguration*>(runConfig);
    Q_ASSERT(specSimRunConfig);
    specSimRunConfig->setEnabled(true);
  }

  return;
}

SpecSimRunControl::StopResult SpecSimRunControl::stop()
{
//  this->_d->_mbtRuntime->stop();

  return AsynchronousStop;
}

void SpecSimRunControl::stoppedSlot(bool result, const QStringList& messages)
{
//  qDebug()<<"SpecSimRunControl::stoppedSlot() executed!";

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
  SpecSimBuildConfiguration* specSimBuildConfig = qobject_cast<SpecSimBuildConfiguration*>(buildConfig);
  Q_ASSERT(specSimBuildConfig);
  specSimBuildConfig->setEnabled(true);

  SpecSimRunConfiguration* specSimRunConfig = qobject_cast<SpecSimRunConfiguration*>(runConfig);
  Q_ASSERT(specSimRunConfig);
  specSimRunConfig->setEnabled(true);

  return;
}

void SpecSimRunControl::finishedSlot(bool result, const QStringList& messages)
{
//  qDebug()<<"SpecSimRunControl::finishedSlot() executed!";

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
  SpecSimBuildConfiguration* specSimBuildConfig = qobject_cast<SpecSimBuildConfiguration*>(buildConfig);
  Q_ASSERT(specSimBuildConfig);
  specSimBuildConfig->setEnabled(true);

  SpecSimRunConfiguration* specSimRunConfig = qobject_cast<SpecSimRunConfiguration*>(this->runConfiguration());
  Q_ASSERT(specSimRunConfig);
  specSimRunConfig->setEnabled(true);

  emit finished();

  return;
}

void SpecSimRunControl::runtimeMessageSlot(bool success, const QString& message)
{
//  qDebug()<<"SpecSimRunControl::runtimeMessageSlot() executed!";

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

bool SpecSimRunControl::isRunning() const
{
  return this->_d->_isRunning;
}

QIcon SpecSimRunControl::icon() const
{
  return this->_d->_runControlIcon;
}

///SpecSimRunControlFactory/////////////////////
class SpecSimRunControlFactory::Private
{
  friend class SpecSimRunControlFactory;

  const Core::Id _runId;

  Private(const Core::Id& runId)
    : _runId(runId)
  {}
};

SpecSimRunControlFactory::SpecSimRunControlFactory(QObject* parent)
  : ProjectExplorer::IRunControlFactory(parent),
    _d(new Private(Constants::SPECSIM_RUN))
{}

SpecSimRunControlFactory::~SpecSimRunControlFactory()
{
  delete this->_d;
}

bool SpecSimRunControlFactory::canRun(ProjectExplorer::RunConfiguration* runConfiguration,
                                  ProjectExplorer::RunMode mode) const
{
  Q_ASSERT(runConfiguration);

  if(runConfiguration->id()==this->_d->_runId
     && mode == ProjectExplorer::NormalRunMode)
    return true;

  return false;
}

SpecSimRunControl* SpecSimRunControlFactory::create(ProjectExplorer::RunConfiguration* runConfiguration,
                                            ProjectExplorer::RunMode mode,
                                            QString* errorMessage)
{
  Q_ASSERT(runConfiguration);

  if(!this->canRun(runConfiguration, mode))
  {
    if(errorMessage)
      *errorMessage+=tr("run-configuration '%1' cannot be handled "
                        "by this model simulation runtime!").arg(runConfiguration->displayName());
    return 0;
  }

  SpecSimRunConfiguration* specSimRunConfig=qobject_cast<SpecSimRunConfiguration*>(runConfiguration);
  if(!specSimRunConfig)
  {
    if(errorMessage)
      *errorMessage+=tr("run-configuration '%1' is not a "
                        "model simulation run configuration!").arg(runConfiguration->displayName());
    return 0;
  }

  SpecSimBuildConfiguration* specSimBC = qobject_cast<SpecSimBuildConfiguration*>(specSimRunConfig
                                                                      ->target()
                                                                      ->activeBuildConfiguration());
  if(!specSimBC)
  {
    if(errorMessage)
      *errorMessage+=tr("model simulation run configuration '%1' has no model simulation build configuration!")
                     .arg(runConfiguration->displayName());
    return 0;
  }

//  if(!specSimBC->specSimRuntime())
//  {
//    if(errorMessage)
//      *errorMessage += tr("there is no model simulation runtime available, please build your project first!");

//    return 0;
//  }

  return new SpecSimRunControl(specSimRunConfig, mode);
}

} // namespace specsim
