#include "mbtpmode.hpp"

#include "mbtpconstants.hpp"

#include <QHeaderView>
#include <QDockWidget>

namespace mbtp {

///MbtpMode/////////////////
MbtpMode::MbtpMode()
{
  this->setContext(Core::Context(Constants::MBTP_PROJECT_MODE_CONTEXT));
  this->setDisplayName(tr("Test Prio"));
  this->setIcon(QIcon(QLatin1String(":/mbtp/images/test_prio_run.png")));
  this->setPriority(Constants::MBTP_PROJECT_MODE_PRIORITY);
  this->setId(Constants::MBTP_PROJECT_MODE_ID);
  this->setContextHelpId(QLatin1String("settings for test prio"));
}

///MbtpModeWidget/////////////////
MbtpModeWidget::MbtpModeWidget(MbtpMode* mbtpMode,
                                   MbtpMainModeWidget* mbtpMainModeWidget)
  : ProjectExplorer::BaseModeWidget(mbtpMode, mbtpMainModeWidget)
{
  this->setObjectName(QLatin1String("MbtpModeWidget"));
}

///MbtpMainModeWidgetPrivate/////////////////
const char DOCKWIDGET_MBTPSETTINGS[] = "MbtpMode.MbtpSettings";

class MbtpMainModeWidgetPrivate
    : public QObject
{
  Q_OBJECT

  friend class MbtpMainModeWidget;

  MbtpMainModeWidgetPrivate()
  { }

public:
  void resetMbtpModeLayout();
};

void MbtpMainModeWidgetPrivate::resetMbtpModeLayout()
{
  //FIXME: still to be implemented!

  return;
}

///MbtpMainModeWidget///////////////////////
MbtpMainModeWidget::MbtpMainModeWidget(MbtpMode* modSynMode)
  : ProjectExplorer::BaseMainModeWidget(modSynMode,
                                        Core::Context(Constants::MBTP_PROJECT_MODE_CONTEXT),
                                        ProjectIdSet()<<Core::Id(Constants::MBTP_PROJECT)),
    _d(new MbtpMainModeWidgetPrivate)
{
  using namespace ProjectExplorer;

  connect(this, &MbtpMainModeWidget::resetLayout,
          this->_d, &MbtpMainModeWidgetPrivate::resetMbtpModeLayout);
}

MbtpMainModeWidget::~MbtpMainModeWidget()
{
  delete this->_d;
}

//void MbtMainModeWidget::readSettings()
//{
//  qDebug()<<"MbtMainModeWidget::readSettings() executed!";

//  QSettings *settings = Core::ICore::settings();
//  this->_d->_dockWidgetActiveState.clear();

//  settings->beginGroup(QLatin1String("MbtMode"));
//  for (const QString &key : settings->childKeys())
//    this->_d->_dockWidgetActiveState.insert(key, settings->value(key));
//  settings->endGroup();

//  // Reset initial settings when there are none yet.
//  if (this->_d->_dockWidgetActiveState.isEmpty())
//  {
//    this->_d->setInitDockWidgetsArrangement();
//    this->_d->_dockWidgetActiveState = this->saveSettings();
//  }

//  this->writeSettings();

//  return;
//}

//void MbtMainModeWidget::showViewsMenu()
//{
//  QMenu *menu = this->createPopupMenu();
//  menu->exec(this->_d->_viewButton->mapToGlobal(QPoint()));
//  delete menu;

//  return;
//}

} // namespace mbtproject

#include "mbtpmode.moc"
