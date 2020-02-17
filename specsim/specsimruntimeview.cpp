#include "specsimruntimeview.hpp"

#include "specsimproject.hpp"
#include "specsimprojectmodel.hpp"
#include "specsimmodel.hpp"

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>
//#include <coreplugin/actionmanager/actionmanager.h>

#include <projectexplorer/projectexplorer.h>

//#include <spenateditor/spenatdocument.hpp>
//#include <spenateditor/spenatgraphiceditor.hpp>
//#include <spenateditor/spenatgraphicwidget.hpp>
//#include <spenateditor/spenatoutlinemodel.hpp>

#include <QTreeView>
#include <QHBoxLayout>
#include <QDebug>

using namespace specsim;

////
// SpecSimModelView
////

class SpecSimModelView
    : public QTreeView
{
  Q_OBJECT

public:
  SpecSimModelView();
  virtual ~SpecSimModelView();

public:
  SpecSimProjectModel const* specSimProjectModel() const;

private slots:
  void updateCurrentItem(Core::IEditor*);
  void contextMenuRequested(QPoint pos);
  void currentProjectChanged(ProjectExplorer::Project* currentProject);

private:
  bool  eventFilter(QObject *obj, QEvent *event);
  void  activateEditor(const QModelIndex &index);

protected:
  virtual void currentChanged(const QModelIndex& current,
                              const QModelIndex& previous);

private:
  Q_DISABLE_COPY(SpecSimModelView)
  class Private;
  Private* _d;
};

class SpecSimModelView::Private
{
  friend class SpecSimModelView;

  SpecSimProject* _currentSpecSimProject;

  Private()
    : _currentSpecSimProject(nullptr)
  {}
};

SpecSimModelView::SpecSimModelView()
  : _d(new Private)
{
  using namespace ProjectExplorer;

  this->setWindowTitle(tr("Simulation Models"));
  this->setWindowIcon(QIcon(QLatin1String(":/mobata/images/statemachine/statemachine.png")));
  this->setAlternatingRowColors(true);
  this->setUniformRowHeights(true);

  this->setSelectionMode(QAbstractItemView::SingleSelection);

  this->setTextElideMode(Qt::ElideMiddle);
  this->setFrameStyle(QFrame::NoFrame);
  this->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->setSelectionMode(QAbstractItemView::SingleSelection);
  this->setSelectionBehavior(QAbstractItemView::SelectRows);
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  this->installEventFilter(this);
  this->viewport()->installEventFilter(this);

  ProjectExplorerPlugin* pePlugin= ProjectExplorerPlugin::instance();
  Q_ASSERT(pePlugin);
  connect(pePlugin, &ProjectExplorerPlugin::currentProjectChanged,
          this, &SpecSimModelView::currentProjectChanged);

  ProjectExplorer::Project* currentProject = pePlugin->currentProject();
  SpecSimProject* specSimProject = qobject_cast<SpecSimProject*>(currentProject);
  if(specSimProject)
    this->setModel(const_cast<SpecSimProjectModel*>(specSimProject->specSimProjectModel()));

  Core::EditorManager* em = Core::EditorManager::instance();
  connect(em, &Core::EditorManager::currentEditorChanged,
          this, &SpecSimModelView::updateCurrentItem);
  connect(this, &SpecSimModelView::customContextMenuRequested,
          this, &SpecSimModelView::contextMenuRequested);
}

SpecSimModelView::~SpecSimModelView()
{
  delete this->_d;
}

void SpecSimModelView::currentProjectChanged(ProjectExplorer::Project* currentProject)
{
  if(!currentProject)
    return;

  this->_d->_currentSpecSimProject = qobject_cast<SpecSimProject*>(currentProject);
  if(!this->_d->_currentSpecSimProject)
    return;

  //find and set model
  this->setModel(const_cast<SpecSimProjectModel*>(this->_d->_currentSpecSimProject->specSimProjectModel()));

  return;
}

void SpecSimModelView::updateCurrentItem(Core::IEditor* editor)
{
  if (!editor)
  {
    this->clearSelection();
    return;
  }

  SpecSimProjectModel const* specSimProjectModel = this->specSimProjectModel();
  if(!specSimProjectModel)
  {
    this->clearSelection();
    return;
  }

  QModelIndex specModelIndex = specSimProjectModel->specSimModelIndexOf(editor->document()->fileName());
  if(!specModelIndex.isValid())
  {
    this->clearSelection();
    return;
  }

  this->setCurrentIndex(specModelIndex);
  this->selectionModel()->select(this->currentIndex(),
                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  this->scrollTo(this->currentIndex());

  return;
}

bool SpecSimModelView::eventFilter(QObject *obj,
                                   QEvent *event)
{
  if (obj == this
      && event->type() == QEvent::KeyPress
      && this->currentIndex().isValid())
  {
    QKeyEvent *ke = static_cast<QKeyEvent*>(event);
    if ((ke->key() == Qt::Key_Return
         || ke->key() == Qt::Key_Enter)
        && ke->modifiers() == 0)
    {
      this->activateEditor(currentIndex());
      return true;
    }
  }

  return false;
}

void SpecSimModelView::activateEditor(const QModelIndex& index)
{
  qDebug()<<"SpecSimModelView::activateEditor() executed!";

  this->selectionModel()->select(index,
                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  SpecSimProjectModel const* specSimProjectModel = this->specSimProjectModel();
  if(!specSimProjectModel)
    return;

  SpecSimModel* selectedSpecSimModel = specSimProjectModel->specSimModelForIndex(index);
  if(!selectedSpecSimModel)
    return;

  Core::EditorManager::openEditor(selectedSpecSimModel->specSimModelFilePath());

  return;
}

SpecSimProjectModel const* SpecSimModelView::specSimProjectModel() const
{
  return qobject_cast<SpecSimProjectModel*>(this->model());
}

void SpecSimModelView::currentChanged(const QModelIndex& current,
                                      const QModelIndex& previous)
{
  Q_UNUSED(previous);
  SpecSimProjectModel const* specSimProjectModel = this->specSimProjectModel();
  if(!specSimProjectModel)
    return;

//  QStandardItem* previousItem = specSimProjectModel->itemFromIndex(previous);
  QStandardItem* currentItem = specSimProjectModel->itemFromIndex(current);
  if(!currentItem)
    return;

  if(currentItem->parent()==nullptr)//spec sim model item
    this->activateEditor(current);

  return;
}

void SpecSimModelView::contextMenuRequested(QPoint pos)
{
  if(!this->_d->_currentSpecSimProject)
    return;

  SpecSimProjectModel const* specSimProjectModel=this->specSimProjectModel();
  if(!specSimProjectModel)
    return;

  QMenu contextMenu;

//  QStandardItem* selectedItem = specSimProjectModel->itemFromIndex(this->indexAt(pos));
//  if(model::ts::TestSuiteItem* testSuiteItem = dynamic_cast<model::ts::TestSuiteItem*>(selectedItem))
//  {
//    Q_ASSERT(testSuiteItem->parent());//tcg coverage item
//    Q_ASSERT(testSuiteItem->parent()->parent());// tcg coverage title

//    QStandardItem* mbtSpecModelItem = testSuiteItem->parent()->parent()->parent();
//    Q_ASSERT(mbtSpecModelItem);
//    MbtSpecModel const* mbtSpecModel = specSimProjectModel->mbtSpecModelForIndex(mbtSpecModelItem->index());
//    Q_ASSERT(mbtSpecModel);

//    Q_ASSERT(this->_d->_currentMbtProject->exportTestSuiteMenu());
//    this->_d->_currentMbtProject->exportTestSuiteMenu()->setExportTestSuiteItem(testSuiteItem);
//    this->_d->_currentMbtProject->exportTestSuiteMenu()->setMbtSpecModel(mbtSpecModel);
//    contextMenu.addMenu(this->_d->_currentMbtProject->exportTestSuiteMenu());
//  }

  contextMenu.addSeparator();
  contextMenu.addAction(tr("Expand All"), this, &SpecSimModelView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &SpecSimModelView::collapseAll);

  contextMenu.exec(this->mapToGlobal(pos));

  return;
}

////
// SpecSimRuntimeView
////

SpecSimRuntimeView::SpecSimRuntimeView(QWidget *parent)
  : QWidget(parent)
{
  QHBoxLayout* hBoxLayout = new QHBoxLayout;
  hBoxLayout->addWidget(new SpecSimModelView());

  this->setLayout(hBoxLayout);
}

SpecSimRuntimeView::~SpecSimRuntimeView()
{}

#include "specsimruntimeview.moc"
