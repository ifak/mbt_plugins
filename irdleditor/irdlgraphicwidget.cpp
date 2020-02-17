#include "irdlgraphicwidget.hpp"

#include "irdlgraphiceditor.hpp"
#include "irdleditorpreviewgraph.hpp"
#include "irdloutlinemodel.hpp"

#include <coreplugin/icore.h>

#include <QToolBar>
#include <QMenu>

#include <mobata/model/requirement/requirementitem.hpp>

#include <mobata/memory_leak_start.hpp>

using namespace model::msc;

namespace irdleditor {

class IrdlGraphicWidget::Private
{
  friend class IrdlGraphicWidget;

  IrdlGraphicEditor* _irdlGraphicEditor;
  QToolBar*          _toolBar = nullptr;
};

IrdlGraphicWidget::IrdlGraphicWidget(QWidget *parent)
  : graphlayout::MscQuickWidget(nullptr,
                                parent),
    _d(new Private)
{
  this->_d->_irdlGraphicEditor = new IrdlGraphicEditor(this);
  this->createToolBar();
}

IrdlGraphicWidget::~IrdlGraphicWidget()
{
  delete this->_d;
}

void IrdlGraphicWidget::updateIrdlGraphic()
{
  qDebug()<<"IrdlGraphicWidget::updateIrdlGraphic";

  IrdlOutlineModel const* irdlOutlineModel = this->_d->_irdlGraphicEditor->irdlOutlineModel();
  if(!irdlOutlineModel)
    return;

  if(irdlOutlineModel->requirements().isEmpty())
  {
    qDebug()<<"IrdlGraphicWidget::updateIrdlGraphic: no irdl";
    return;
  }

  this->setMscSequence(irdlOutlineModel->requirements().first());
  irdlGraphLoadSettings(this);
  QString error;
  this->layout(&error);

  return;
}

void IrdlGraphicWidget::createToolBar()
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
          this, &IrdlGraphicWidget::zoomIn);

  QAction* zoomOutAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomout_dark.png")),
                                       tr("zoom out"),
                                       this);
  zoomOutAction->setStatusTip(tr("zoom out"));
  zoomOutAction->setToolTip(tr("zoom out"));
  zoomOutAction->setCheckable(false);
  zoomOutAction->setAutoRepeat(true);
  zoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
  connect(zoomOutAction, &QAction::triggered,
          this, &IrdlGraphicWidget::zoomOut);

  QAction* zoomFitInViewAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoominview_dark.png")),
                                             tr("zoom fit in view"),
                                             this);
  zoomFitInViewAction->setStatusTip(tr("zoom fit in view"));
  zoomFitInViewAction->setToolTip(tr("zoom fit in view"));
  zoomFitInViewAction->setCheckable(false);
  connect(zoomFitInViewAction, &QAction::triggered,
          this, &IrdlGraphicWidget::zoomFitAll);

  QAction* zoomResetAction = new QAction(QIcon(QLatin1String(":/mobata/images/zoomreset_dark.png")),
                                         tr("reset zoom"),
                                         this);
  zoomResetAction->setStatusTip(tr("reset zoom"));
  zoomResetAction->setToolTip(tr("reset zoom"));
  zoomResetAction->setCheckable(false);
  connect(zoomResetAction, &QAction::triggered,
          this, &IrdlGraphicWidget::zoomFitAll);

  QAction* saveImageAction = new QAction(QIcon(QLatin1String(":/mobata/images/save_picture.png")),
                                         tr("save graphpicture"),
                                         this);
  saveImageAction->setStatusTip(tr("save graphpicture"));
  saveImageAction->setToolTip(tr("save graphpicture"));
  saveImageAction->setCheckable(false);
  connect(saveImageAction, &QAction::triggered,
          this, &IrdlGraphicWidget::saveAsPicture);

  this->_d->_toolBar=new QToolBar;
  this->_d->_toolBar->addAction(zoomInAction);
  this->_d->_toolBar->addAction(zoomOutAction);
  this->_d->_toolBar->addAction(zoomFitInViewAction);
  this->_d->_toolBar->addAction(zoomResetAction);
  this->_d->_toolBar->addAction(saveImageAction);


  return;
}

QToolBar* IrdlGraphicWidget::toolBar()
{
  return this->_d->_toolBar;
}

IrdlGraphicEditor* IrdlGraphicWidget::irdlGraphicEditor() const
{
  return this->_d->_irdlGraphicEditor;
}

} // namespace irdleditor
