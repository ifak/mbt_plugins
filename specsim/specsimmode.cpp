#include "specsimmode.hpp"

#include "specsimconstants.hpp"
#include "specsimruntimeview.hpp"

#include <utils/styledbar.h>

#include <QToolButton>
#include <QDockWidget>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QMenu>

namespace specsim{

///SpecSimMode/////////////////
SpecSimMode::SpecSimMode()
{
  this->setContext(Core::Context(Constants::SPECSIM_MODE_CONTEXT));
  this->setDisplayName(tr("Spec Simulation"));
  this->setIcon(QIcon(QLatin1String(":/specsim/images/Simulator.png")));
  this->setPriority(Constants::SPECSIM_MODE_PRIORITY);
  this->setId(Constants::SPECSIM_MODE_ID);
  this->setContextHelpId(QLatin1String("simulation of specification models"));
}

///SpecSimModeWidget/////////////////
SpecSimModeWidget::SpecSimModeWidget(SpecSimMode* specSimMode,
                                     SpecSimMainModeWidget* specSimMainModeWidget)
  : ProjectExplorer::BaseModeWidget(specSimMode, specSimMainModeWidget)
{
  this->setObjectName(QLatin1String("SpecSimModeWidget"));
}

///SpecSimMainModeWidgetPrivate/////////////////

const char DOCKWIDGET_SPECSIMRUNTIME[] = "SpecSimMode.SpecSimRuntime";

class SpecSimMainModeWidgetPrivate
    : public QObject
{
  Q_OBJECT

  friend class SpecSimMainModeWidget;

  SpecSimRuntimeView* _specSimRuntimeView;

  QWidget*      _specSimToolBar;
  QHBoxLayout*  _specSimToolBarLayout;
  QToolButton*  _viewButton;

  SpecSimMainModeWidgetPrivate()
    : _specSimRuntimeView(new SpecSimRuntimeView),
      _specSimToolBar(new QWidget),
      _specSimToolBarLayout(new QHBoxLayout(_specSimToolBar)),
      _viewButton(nullptr)
  {
    this->_specSimToolBarLayout->setMargin(0);
    this->_specSimToolBarLayout->setSpacing(0);
  }

public:
  void resetSpecSimModeLayout();
};

void SpecSimMainModeWidgetPrivate::resetSpecSimModeLayout()
{
  //FIXME: still to be implemented!

  return;
}

///SpecSimMainModeWidget///////////////////////
SpecSimMainModeWidget::SpecSimMainModeWidget(SpecSimMode* specSimMode)
  : ProjectExplorer::BaseMainModeWidget(specSimMode,
                                        Core::Context(Constants::SPECSIM_MODE_CONTEXT),
                                        ProjectIdSet()<<Core::Id(Constants::SPECSIM_PROJECT)),
    _d(new SpecSimMainModeWidgetPrivate)
{
  using namespace ProjectExplorer;

  connect(this, &SpecSimMainModeWidget::resetLayout,
          this->_d, &SpecSimMainModeWidgetPrivate::resetSpecSimModeLayout);

  this->_d->_viewButton = new QToolButton();
  // FIXME: Use real thing after string freeze.
  QString hackyName = QCoreApplication::translate("Core::Internal::MainWindow", "&Views");
  hackyName.replace(QLatin1Char('&'), QString());
  this->_d->_viewButton->setText(hackyName);

  Utils::StyledBar* specSimToolBar = new Utils::StyledBar;
  specSimToolBar->setProperty("topBorder", true);
  QHBoxLayout* specSimToolBarLayout = new QHBoxLayout(specSimToolBar);
  specSimToolBarLayout->setMargin(0);
  specSimToolBarLayout->setSpacing(0);
  specSimToolBarLayout->addWidget(this->_d->_specSimToolBar);
  specSimToolBarLayout->addWidget(new Utils::StyledSeparator);
  specSimToolBarLayout->addWidget(this->_d->_viewButton);

  connect(this->_d->_viewButton, &QToolButton::clicked,
          this, &SpecSimMainModeWidget::showViewsMenu);

  QDockWidget *toolBarDock = new QDockWidget(SpecSimMainModeWidgetPrivate::tr("Spec Simulation Toolbar"));
  toolBarDock->setObjectName(QLatin1String("Spec Simulation Toolbar"));
  toolBarDock->setWidget(specSimToolBar);
  toolBarDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  toolBarDock->setAllowedAreas(Qt::BottomDockWidgetArea);
  toolBarDock->setTitleBarWidget(new QWidget(toolBarDock));
  toolBarDock->setProperty("managed_dockwidget", QLatin1String("true"));
  addDockWidget(Qt::BottomDockWidgetArea, toolBarDock);
  setToolBarDockWidget(toolBarDock);

  Q_ASSERT(this->_d->_specSimRuntimeView);
  this->_d->_specSimRuntimeView->setObjectName(QLatin1String(DOCKWIDGET_SPECSIMRUNTIME));
  QDockWidget* specSimRuntimeDock = this->createDockWidget(this->_d->_specSimRuntimeView, Qt::BottomDockWidgetArea);
  Q_ASSERT(specSimRuntimeDock);
  specSimRuntimeDock->setWindowTitle(QLatin1String("Model Simulation Settings"));

  this->splitDockWidget(toolBarDock, specSimRuntimeDock, Qt::Vertical);
}

SpecSimMainModeWidget::~SpecSimMainModeWidget()
{
  delete this->_d;
}

void SpecSimMainModeWidget::showViewsMenu()
{
  QMenu *menu = this->createPopupMenu();
  menu->exec(this->_d->_viewButton->mapToGlobal(QPoint()));
  delete menu;

  return;
}

}//namespace specsim

#include "specsimmode.moc"
