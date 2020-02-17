#include "mbtmode.hpp"

#include "mbtprojectconstants.hpp"
#include "mbtspecmodelview.hpp"

#include <QHeaderView>
#include <QDockWidget>

namespace mbtproject {

///MbtMode/////////////////
MbtMode::MbtMode()
{
  this->setContext(Core::Context(Constants::MBT_PROJECT_MODE_CONTEXT));
  this->setDisplayName(tr("Test Generation"));
  this->setIcon(QIcon(QLatin1String(":/mbtproject/images/mbt_mode.png")));
  this->setPriority(Constants::MBT_PROJECT_MODE_PRIORITY);
  this->setId(Constants::MBT_PROJECT_MODE_ID);
  this->setContextHelpId(QLatin1String("additional settings for test generation"));
}

///MbtModeWidget/////////////////
MbtModeWidget::MbtModeWidget(MbtMode* mbtMode,
                             MbtMainModeWidget* mbtMainModeWidget)
  : ProjectExplorer::BaseModeWidget(mbtMode, mbtMainModeWidget)
{
  this->setObjectName(QLatin1String("MbtModeWidget"));
}

///MbtMainModeWidgetPrivate/////////////////
const char DOCKWIDGET_MBTCOVERAGE[] = "MbtMode.MbtCoverage";

class MbtMainModeWidgetPrivate
    : public QObject
{
  Q_OBJECT

  friend class MbtMainModeWidget;

  MbtSpecModelView*         _mbtSpecModelView;


  MbtMainModeWidgetPrivate()
    : _mbtSpecModelView(new MbtSpecModelView)
  { }

public:
  void resetMbtModeLayout();
};

void MbtMainModeWidgetPrivate::resetMbtModeLayout()
{
  //FIXME: still to be implemented!

  return;
}

///MbtMainModeWidget///////////////////////
MbtMainModeWidget::MbtMainModeWidget(MbtMode* mbtMode)
  : ProjectExplorer::BaseMainModeWidget(mbtMode,
                                        Core::Context(Constants::MBT_PROJECT_MODE_CONTEXT),
                                        ProjectIdSet()<<Core::Id(Constants::MBT_PROJECT)),
    _d(new MbtMainModeWidgetPrivate)
{
  using namespace ProjectExplorer;

  connect(this, &MbtMainModeWidget::resetLayout,
          this->_d, &MbtMainModeWidgetPrivate::resetMbtModeLayout);

  Q_ASSERT(this->_d->_mbtSpecModelView);
  this->_d->_mbtSpecModelView->header()->show();
  this->_d->_mbtSpecModelView->setObjectName(QLatin1String(DOCKWIDGET_MBTCOVERAGE));
  QDockWidget* dockWidget = this->createDockWidget(this->_d->_mbtSpecModelView, Qt::RightDockWidgetArea);
  Q_ASSERT(dockWidget);
  dockWidget->setWindowTitle(QLatin1String("MBT Coverage Specification"));
}

MbtMainModeWidget::~MbtMainModeWidget()
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

#include "mbtmode.moc"
