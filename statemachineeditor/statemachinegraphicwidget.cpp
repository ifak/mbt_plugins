#include "statemachinegraphicwidget.hpp"

#include "statemachinegraphiceditor.hpp"
#include "statemachineoutlinemodel.hpp"
#include "statemachineeditorpreviewgraph.hpp"

#include <graphlayout/layoutglobal.hpp>
#include <graphlayout/layoutgraph.hpp>
#include <graphlayout/comcreatestatemachinegraph.hpp>
#include <graphlayout/io/comloadjsonlayout.hpp>

#include <mobata/utils/functors.hpp>
#include <mobata/model/statemachine/statemachinemodel.hpp>

#include <coreplugin/icore.h>

#include <QToolBar>
#include <QMenu>
#include <QFileDialog>

#include <mobata/memory_leak_start.hpp>

namespace statemachineeditor {

class StateMachineGraphicWidget::Private
{
  friend class StateMachineGraphicWidget;

  StateMachineGraphicEditor*  _stateMachineGraphicEditor;
  graphlayout::LayoutGraph    _layoutGraph;
  QToolBar*                   _toolBar;
};

StateMachineGraphicWidget::StateMachineGraphicWidget(QWidget *parent)
  : graphlayout::GraphQuickWidget(nullptr,
                                  graphlayout::Dot,
                                  parent),
    _d(new Private)
{
  this->setLayoutGraph(&this->_d->_layoutGraph);
  this->_d->_stateMachineGraphicEditor = new StateMachineGraphicEditor(this);

  this->createToolBar();
}

StateMachineGraphicWidget::~StateMachineGraphicWidget()
{
  delete this->_d->_toolBar;
  delete this->_d;
}

void StateMachineGraphicWidget::createToolBar()
{
  QAction* zoomInAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomin_dark.png")),
                                      tr("zoom in"),
                                      this);
  zoomInAction->setStatusTip(tr("zoom in"));
  zoomInAction->setToolTip(tr("zoom in"));
  zoomInAction->setCheckable(false);
  zoomInAction->setAutoRepeat(true);
  zoomInAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
  connect(zoomInAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::zoomIn);

  QAction* zoomOutAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomout_dark.png")),
                                       tr("zoom out"),
                                       this);
  zoomOutAction->setStatusTip(tr("zoom out"));
  zoomOutAction->setToolTip(tr("zoom out"));
  zoomOutAction->setCheckable(false);
  zoomOutAction->setAutoRepeat(true);
  zoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
  connect(zoomOutAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::zoomOut);

  QAction* zoomFitInViewAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoominview_dark.png")),
                                             tr("zoom fit in view"),
                                             this);
  zoomFitInViewAction->setStatusTip(tr("zoom fit in view"));
  zoomFitInViewAction->setToolTip(tr("zoom fit in view"));
  zoomFitInViewAction->setCheckable(false);
  connect(zoomFitInViewAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::zoomFull);

  QAction* zoomResetAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomreset_dark.png")),
                                         tr("reset zoom"),
                                         this);
  zoomResetAction->setStatusTip(tr("reset zoom"));
  zoomResetAction->setToolTip(tr("reset zoom"));
  zoomResetAction->setCheckable(false);
  connect(zoomResetAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::zoomReset);

  QAction* zoomFitWidthAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomwidth_dark.png")),
                                         tr("zoom width"),
                                         this);
  zoomFitWidthAction->setStatusTip(tr("zoom width"));
  zoomFitWidthAction->setToolTip(tr("zoom width"));
  zoomFitWidthAction->setCheckable(false);
  connect(zoomFitWidthAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::zoomWidth);

  QAction* zoomFitHeightAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomheight_dark.png")),
                                         tr("zoom height"),
                                         this);
  zoomFitHeightAction->setStatusTip(tr("zoom height"));
  zoomFitHeightAction->setToolTip(tr("zoom height"));
  zoomFitHeightAction->setCheckable(false);
  connect(zoomFitHeightAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::zoomHeight);

  QAction* showLabelAction = new QAction(QIcon(QLatin1String(":/mobata/images/save.png")),
                                         tr("show label"),
                                         this);
  showLabelAction->setStatusTip(tr("show label"));
  showLabelAction->setToolTip(tr("show label"));
  showLabelAction->setCheckable(true);
  connect(showLabelAction, &QAction::toggled,
          this, &StateMachineGraphicWidget::toggleLabel);

  QAction* zoomToActiveItemAction = new QAction(QIcon(QLatin1String(":/mobata/images/select.png")),
                                         tr("zoom to active item"),
                                         this);
  zoomToActiveItemAction->setStatusTip(tr("zoom to active item"));
  zoomToActiveItemAction->setToolTip(tr("zoom to active item"));
  zoomToActiveItemAction->setCheckable(true);
  connect(zoomToActiveItemAction, &QAction::toggled,
          this, &StateMachineGraphicWidget::zoomToActiveEnabled);

  QAction* saveImageAction = new QAction(QIcon(QLatin1String(":/mobata/images/save_picture.png")),
                                         tr("save graphpicture"),
                                         this);
  saveImageAction->setStatusTip(tr("save graphpicture"));
  saveImageAction->setToolTip(tr("save graphpicture"));
  saveImageAction->setCheckable(false);
  connect(saveImageAction, &QAction::triggered,
          this, &StateMachineGraphicWidget::saveGraphAsPicture);

  this->_d->_toolBar=new QToolBar;
  this->_d->_toolBar->addAction(zoomInAction);
  this->_d->_toolBar->addAction(zoomOutAction);
  this->_d->_toolBar->addAction(zoomFitInViewAction);
  this->_d->_toolBar->addAction(zoomResetAction);
  this->_d->_toolBar->addAction(zoomFitWidthAction);
  this->_d->_toolBar->addAction(zoomFitHeightAction);
  this->_d->_toolBar->addAction(showLabelAction);
  this->_d->_toolBar->addAction(zoomToActiveItemAction);
  this->_d->_toolBar->addAction(saveImageAction);

  return;
}

QToolBar* StateMachineGraphicWidget::toolBar()
{
  return this->_d->_toolBar;
}

void StateMachineGraphicWidget::updateStateMachineGraphic(const QString& layoutFilepath)
{
//  qDebug()<<"StateMachineGraphicWidget::updateStateMachineGraphic";

  const StateMachineModelPtr stateMachineModel = this->_d->_stateMachineGraphicEditor
                                                 ->stateMachineModel();
  if(!stateMachineModel)
    return;

  graphlayout::ComCreateStateMachineGraph comCreateStateMachineGraph(&this->_d->_layoutGraph,
                                                                     stateMachineModel.data());

  stateMachineGraphLoadSettings(&comCreateStateMachineGraph,
                                this);

  QString error;
  bool result = comCreateStateMachineGraph.execute(&error);
  if(!result)
  {
    qWarning()<<"error during state machine layout graph creation: "<<error;
    return;
  }

  //Overwrite LayoutGraph with properties saved in LayoutFile (if Layoutfile exists)
  if(!layoutFilepath.isEmpty()){
    graphlayout::ComLoadJsonLayout comLoadJsonLayout(layoutFilepath, &this->_d->_layoutGraph);
    if(!comLoadJsonLayout.execute(&error))
        qDebug() << error; //better would be to print this error to mbtcreators build warnings
    else
      this->setGraphvizAlgorithm(graphlayout::NoGraphviz);
  }

  result = this->layout(&error);
  if(!result)
  {
    qWarning()<<"error during layouting: "<<error<<"\n"<<this->lastLog();
    return;
  }

  return;
}

void StateMachineGraphicWidget::updateStateMachineGraphic()
{
  return this->updateStateMachineGraphic(QLatin1String(""));
}

StateMachineGraphicEditor* StateMachineGraphicWidget::stateMachineGraphicEditor() const
{
  return this->_d->_stateMachineGraphicEditor;
}

} // namespace statemachineeditor
