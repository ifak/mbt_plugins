#include "spenatgraphicwidget.hpp"

#include "spenatgraphiceditor.hpp"
#include "spenatoutlinemodel.hpp"
#include "spenateditorpreviewgraph.hpp"

#include <graphlayout/layoutglobal.hpp>
#include <graphlayout/layoutgraph.hpp>
#include <graphlayout/comcreatedeclgraph.hpp>

#include <coreplugin/icore.h>

#include <QToolBar>
#include <QMenu>
#include <QFileDialog>

#include <mobata/memory_leak_start.hpp>

namespace spenateditor {

class SpenatGraphicWidget::Private
{
  friend class SpenatGraphicWidget;

  SpenatGraphicEditor*        _spenatGraphicEditor;
  graphlayout::LayoutGraph    _layoutGraph;
  QToolBar*                   _toolBar = nullptr;
};

SpenatGraphicWidget::SpenatGraphicWidget(QWidget *parent)
  : graphlayout::GraphQuickWidget(nullptr,
                                  graphlayout::Dot,
                                  parent),
    _d(new Private)
{
  this->setLayoutGraph(&this->_d->_layoutGraph);
  this->_d->_spenatGraphicEditor = new SpenatGraphicEditor(this);

  this->createToolBar();
}

SpenatGraphicWidget::~SpenatGraphicWidget()
{
  delete this->_d->_toolBar;
  delete this->_d;
}

void SpenatGraphicWidget::createToolBar()
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
          this, &SpenatGraphicWidget::zoomIn);

  QAction* zoomOutAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomout_dark.png")),
                                       tr("zoom out"),
                                       this);
  zoomOutAction->setStatusTip(tr("zoom out"));
  zoomOutAction->setToolTip(tr("zoom out"));
  zoomOutAction->setCheckable(false);
  zoomOutAction->setAutoRepeat(true);
  zoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
  connect(zoomOutAction, &QAction::triggered,
          this, &SpenatGraphicWidget::zoomOut);

  QAction* zoomFitInViewAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoominview_dark.png")),
                                             tr("zoom fit in view"),
                                             this);
  zoomFitInViewAction->setStatusTip(tr("zoom fit in view"));
  zoomFitInViewAction->setToolTip(tr("zoom fit in view"));
  zoomFitInViewAction->setCheckable(false);
  connect(zoomFitInViewAction, &QAction::triggered,
          this, &SpenatGraphicWidget::zoomFull);

  QAction* zoomResetAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomreset_dark.png")),
                                         tr("reset zoom"),
                                         this);
  zoomResetAction->setStatusTip(tr("reset zoom"));
  zoomResetAction->setToolTip(tr("reset zoom"));
  zoomResetAction->setCheckable(false);
  connect(zoomResetAction, &QAction::triggered,
          this, &SpenatGraphicWidget::zoomReset);

  QAction* zoomFitWidthAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomwidth_dark.png")),
                                         tr("zoom width"),
                                         this);
  zoomFitWidthAction->setStatusTip(tr("zoom width"));
  zoomFitWidthAction->setToolTip(tr("zoom width"));
  zoomFitWidthAction->setCheckable(false);
  connect(zoomFitWidthAction, &QAction::triggered,
          this, &SpenatGraphicWidget::zoomWidth);

  QAction* zoomFitHeightAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomheight_dark.png")),
                                         tr("zoom height"),
                                         this);
  zoomFitHeightAction->setStatusTip(tr("zoom height"));
  zoomFitHeightAction->setToolTip(tr("zoom height"));
  zoomFitHeightAction->setCheckable(false);
  connect(zoomFitHeightAction, &QAction::triggered,
          this, &SpenatGraphicWidget::zoomHeight);

  QAction* showLabelAction = new QAction(QIcon(QLatin1String(":/mobata/images/save.png")),
                                         tr("show label"),
                                         this);
  showLabelAction->setStatusTip(tr("show label"));
  showLabelAction->setToolTip(tr("show label"));
  showLabelAction->setCheckable(true);
  connect(showLabelAction, &QAction::toggled,
          this, &SpenatGraphicWidget::toggleLabel);

  QAction* zoomToActiveItemAction = new QAction(QIcon(QLatin1String(":/mobata/images/select.png")),
                                         tr("zoom to active item"),
                                         this);
  zoomToActiveItemAction->setStatusTip(tr("zoom to active item"));
  zoomToActiveItemAction->setToolTip(tr("zoom to active item"));
  zoomToActiveItemAction->setCheckable(true);
  connect(zoomToActiveItemAction, &QAction::toggled,
          this, &SpenatGraphicWidget::zoomToActiveEnabled);

  QAction* saveImageAction = new QAction(QIcon(QLatin1String(":/mobata/images/save_picture.png")),
                                         tr("save graph as picture"),
                                         this);
  saveImageAction->setStatusTip(tr("save graph as picture"));
  saveImageAction->setToolTip(tr("save graph as picture"));
  saveImageAction->setCheckable(false);
  connect(saveImageAction, &QAction::triggered,
          this, &SpenatGraphicWidget::saveGraphAsPicture);

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

QToolBar* SpenatGraphicWidget::toolBar()
{
  return this->_d->_toolBar;
}

void SpenatGraphicWidget::updateSpenatGraphic()
{
//  qDebug()<<"SpenatGraphicWidget::updateSpenatGraphic";

  const SpenatDeclModelPtr spenatDeclModel = this->_d->_spenatGraphicEditor->spenatDeclModel();
  if(!spenatDeclModel)
    return;

  graphlayout::ComCreateDeclGraph comCreateDeclGraph(&this->_d->_layoutGraph,
                                                     spenatDeclModel.data());

  spenatGraphLoadSettings(&comCreateDeclGraph,
                          this);

  QString error;
  bool result = comCreateDeclGraph.execute(&error);
  if(!result)
  {
    qWarning()<<"error during spenat layout graph creation: "<<error;
    return;
  }

  result = this->layout(&error);
  if(!result)
  {
    qWarning()<<"error during layouting: "<<error<<"\n"<<this->lastLog();
    return;
  }

  return;
}

SpenatGraphicEditor* SpenatGraphicWidget::spenatGraphicEditor() const
{
  return this->_d->_spenatGraphicEditor;
}

} // namespace spenateditor
