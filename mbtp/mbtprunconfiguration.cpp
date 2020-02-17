#include "mbtprunconfiguration.hpp"

#include "mbtpconstants.hpp"

#include "mbtpproject.hpp"
#include "runtime/mbtpruntime.hpp"
#include "mbtpgraphiceditor.hpp"
#include "mbtpbuildconfiguration.hpp"
#include "mbtpreportbuilder.hpp"
#include "mbtptablemodel.hpp"

#include <projectexplorer/projectexplorer.h>
#include <coreplugin/editormanager/editormanager.h>
#include <projectexplorer/target.h>
#include <projectexplorer/task.h>

#include <QIcon>
#include <QDebug>
#include <QTime>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QAbstractTableModel>
#include <QQuickItem>
#include <QLabel>
#include <QPixmap>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

///MbtpRunConfiguration//////////
/// \brief MbtpRunConfiguration::MbtpRunConfiguration
/// \param parent
/// \param id

class MbtpRunConfiguration::Private
{
  friend class MbtpRunConfiguration;

  bool _enabled;

  Private()
    : _enabled(true)
  {}
};

MbtpRunConfiguration::MbtpRunConfiguration(ProjectExplorer::Target* parent)
  : ProjectExplorer::RunConfiguration(parent, Constants::MBTP_RUN),
    _d(new Private)
{
  Q_ASSERT(parent);

  this->setDisplayName(QLatin1String("Priorize Specification Model"));
}

MbtpRunConfiguration::~MbtpRunConfiguration()
{
  delete this->_d;
}

QWidget* MbtpRunConfiguration::createConfigurationWidget()
{
  qDebug()<<"ModSynRunConfiguration::createConfigWidget() executed()";

  return nullptr;
}

QString MbtpRunConfiguration::title() const
{
  return this->displayName();
}

void MbtpRunConfiguration::setEnabled(const bool enabled)
{
  if(this->_d->_enabled == enabled)
    return;

  this->_d->_enabled=enabled;
  emit enabledChanged();

  return;
}

bool MbtpRunConfiguration::isEnabled() const
{
  return this->_d->_enabled;
}

///MbtpConfigurationWidget/////////////////////
MbtpConfigurationWidget::MbtpConfigurationWidget(MbtpRunConfiguration* mbtpRunConfiguration,
                                                 QWidget *parent)
  : QWidget(parent), _mbtpRunConfiguration(mbtpRunConfiguration)
{}

///MbtpRunControl/////////////////////
/// \brief MbtpRunControl::MbtpRunControl
/// \param mbtpRunConfiguration
/// \param mode
/// \param runControlIcon

class MbtpRunControl::Private
{
  friend class MbtpRunControl;

  bool        _isRunning;
  QIcon       _runControlIcon;
  MbtpRuntime* _mbtpRuntime;
  MbtpReportBuilder* _mbtpReportBuilder;
  QFile             _prioReportFile;

  Private(const QIcon& runControlIcon)
    : _isRunning(false),
      _runControlIcon(runControlIcon)
  {}
};

MbtpRunControl::MbtpRunControl(MbtpRunConfiguration* mbtpRunConfiguration,
                               ProjectExplorer::RunMode mode,
                               const QIcon& runControlIcon)
  : ProjectExplorer::RunControl(mbtpRunConfiguration, mode),
    _d(new Private(runControlIcon))
{
  Q_ASSERT(mbtpRunConfiguration);

  MbtpBuildConfiguration* mbtpBC = qobject_cast<MbtpBuildConfiguration*>(mbtpRunConfiguration->target()->activeBuildConfiguration());
  Q_ASSERT(mbtpBC);

  this->_d->_mbtpRuntime = mbtpBC->mbtpRuntime();
  Q_ASSERT(this->_d->_mbtpRuntime);

  connect(this->_d->_mbtpRuntime, &MbtpRuntime::started,
          this, &MbtpRunControl::startedSlot,
          Qt::QueuedConnection);
  connect(this->_d->_mbtpRuntime, &MbtpRuntime::stopped,
          this, &MbtpRunControl::stoppedSlot,
          Qt::QueuedConnection);
  connect(this->_d->_mbtpRuntime, &MbtpRuntime::finished,
          this, &MbtpRunControl::finishedSlot,
          Qt::QueuedConnection);
  connect(this->_d->_mbtpRuntime, &MbtpRuntime::runtimeMessage,
          this, &MbtpRunControl::runtimeMessageSlot,
          Qt::QueuedConnection);
}

MbtpRunControl::~MbtpRunControl()
{}

void MbtpRunControl::start()
{
  this->_d->_isRunning = true;
  emit started();

  ProjectExplorer::RunConfiguration* runConfig=this->runConfiguration();
  Q_ASSERT(runConfig);
  Q_ASSERT(runConfig->target());

  MbtpRunConfiguration* mbtpRunConfig = qobject_cast<MbtpRunConfiguration*>(runConfig);
  Q_ASSERT(mbtpRunConfig);
  mbtpRunConfig->setEnabled(false);

  this->_d->_mbtpRuntime->start();

  return;
}

void MbtpRunControl::startedSlot(bool result, const QStringList& messages)
{
  qDebug()<<"MbtpRunControl::startedSlot() executed!";

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

    MbtpRunConfiguration* mbtpRunConfig = qobject_cast<MbtpRunConfiguration*>(runConfig);
    Q_ASSERT(mbtpRunConfig);
    mbtpRunConfig->setEnabled(true);
  }

  return;
}

MbtpRunControl::StopResult MbtpRunControl::stop()
{
  this->_d->_mbtpRuntime->stop();

  return AsynchronousStop;
}

void MbtpRunControl::stoppedSlot(bool result, const QStringList& messages)
{
  qDebug()<<"MbtpRunControl::stoppedSlot() executed!";

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

  MbtpRunConfiguration* mbtpRunConfig = qobject_cast<MbtpRunConfiguration*>(runConfig);
  Q_ASSERT(mbtpRunConfig);
  mbtpRunConfig->setEnabled(true);

  return;
}

void MbtpRunControl::finishedSlot(bool result, const QStringList& messages)
{
  qDebug()<<"MbtpRunControl::finishedSlot() executed!";

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

  MbtpRunConfiguration* mbtpRunConfig = qobject_cast<MbtpRunConfiguration*>(this->runConfiguration());
  Q_ASSERT(mbtpRunConfig);
  mbtpRunConfig->setEnabled(true);

  this->showResult();

  emit finished();

  return;
}

void MbtpRunControl::runtimeMessageSlot(bool success, const QString& message)
{
  qDebug()<<"MbtpRunControl::runtimeMessageSlot() executed!";

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

bool MbtpRunControl::isRunning() const
{
  return this->_d->_isRunning;
}

QIcon MbtpRunControl::icon() const
{
  return this->_d->_runControlIcon;
}

///MbtpRunControlFactory/////////////////////
/// \brief MbtpRunControlFactory::MbtpRunControlFactory
/// \param parent

class MbtpRunControlFactory::Private
{
  friend class MbtpRunControlFactory;

  const Core::Id _runId;

  Private(const Core::Id& runId)
    : _runId(runId)
  {}
};

MbtpRunControlFactory::MbtpRunControlFactory(QObject* parent)
  : ProjectExplorer::IRunControlFactory(parent),
    _d(new Private(Constants::MBTP_RUN))
{}

MbtpRunControlFactory::~MbtpRunControlFactory()
{
  delete this->_d;
}

bool MbtpRunControlFactory::canRun(ProjectExplorer::RunConfiguration* runConfiguration,
                                   ProjectExplorer::RunMode mode) const
{
  Q_ASSERT(runConfiguration);

  if(runConfiguration->id()==this->_d->_runId
     && mode == ProjectExplorer::NormalRunMode)
    return true;

  return false;
}

MbtpRunControl* MbtpRunControlFactory::create(ProjectExplorer::RunConfiguration* runConfiguration,
                                              ProjectExplorer::RunMode mode,
                                              QString* errorMessage)
{
  Q_ASSERT(runConfiguration);

  if(!this->canRun(runConfiguration, mode))
  {
    if(errorMessage)
      *errorMessage+=tr("run-configuration '%1' cannot be handled "
                        "by this test system!").arg(runConfiguration->displayName());
    return nullptr;
  }

  MbtpRunConfiguration* mbtpRunConfig=qobject_cast<MbtpRunConfiguration*>(runConfiguration);
  if(!mbtpRunConfig)
  {
    if(errorMessage)
      *errorMessage+=tr("run-configuration '%1' is not an "
                        "mbtp run configuration!").arg(runConfiguration->displayName());
    return nullptr;
  }

  return new MbtpRunControl(mbtpRunConfig, mode);
}

void MbtpRunControl::showResult()
{
  qDebug()<<"MbtpRunControl::showResult() executed!";

  ProjectExplorer::Project*  startupProject=ProjectExplorer::ProjectExplorerPlugin::instance()->startupProject();
  MbtpProject* mbtpProject=qobject_cast<MbtpProject*>(startupProject);
  if(mbtpProject)
  {
    QString tempPrioReportFile = QDir::tempPath();
    if (!tempPrioReportFile.endsWith(QLatin1Char('/')))
      tempPrioReportFile += QLatin1Char('/');
    tempPrioReportFile += mbtpProject->projectName()+ Constants::MBTPGRAPHIC_MIME_TYPE;

    if(this->_d->_prioReportFile.exists())
    {
      this->_d->_prioReportFile.close();
      this->_d->_prioReportFile.rename(tempPrioReportFile);
    }
    else
      this->_d->_prioReportFile.setFileName(tempPrioReportFile);

    QTC_ASSERT(this->_d->_prioReportFile.open(QIODevice::ReadWrite), return;);
    this->_d->_prioReportFile.close();
    qDebug()<<"prio report file: "<<this->_d->_prioReportFile.fileName();

    Core::EditorManager* editorManager = Core::EditorManager::instance();
    editorManager->removeAllSplits();
    Core::IEditor* editor = editorManager->openEditor(this->_d->_prioReportFile.fileName(),Constants::MBTP_GRAPHIC);

    if(MbtpGraphicEditor* mbtpGraphicEditor = qobject_cast<MbtpGraphicEditor*>(editor))
    {
      QQuickWidget* prioGraphicWidget=qobject_cast<QQuickWidget*>(mbtpGraphicEditor->widget());
      if(prioGraphicWidget)
      {
        prioGraphicWidget->engine()->clearComponentCache();

        this->_d->_mbtpReportBuilder = new MbtpReportBuilder();
        this->_d->_mbtpReportBuilder->buildReport(this->_d->_mbtpRuntime);
        QObject::connect(prioGraphicWidget, &QQuickWidget::destroyed,
                         this->_d->_mbtpReportBuilder, &MbtpReportBuilder::deleteLater);

        prioGraphicWidget->engine()->rootContext()->setContextProperty(QStringLiteral("mbtpModel"),
                                                                       this->_d->_mbtpReportBuilder->mbtpRuntimeModel());
//        prioGraphicWidget->engine()->rootContext()->setContextProperty("handler", this->_d->_mbtpReportBuilder);

        MbtpReportImageProvider* ip = new MbtpReportImageProvider();
        ip->setImage(this->_d->_mbtpRuntime->project()->resultImage());

        prioGraphicWidget->engine()->addImageProvider("resultImage", ip);

        prioGraphicWidget->setSource(QUrl(QStringLiteral("qrc:/resultView.qml")));

        prioGraphicWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        prioGraphicWidget->show();
        QObject* rootItem = qobject_cast<QObject*>(prioGraphicWidget->rootObject());
        QObject::connect(this, &MbtpRunControl::destroyed,
                         rootItem, &QQuickWidget::deleteLater);

      }
    }
  }
  return;
}

} // namespace mbtp

