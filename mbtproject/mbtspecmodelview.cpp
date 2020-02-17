#include "mbtspecmodelview.hpp"

#include "mbtproject.hpp"
#include "mbtprojectmodel.hpp"
#include "mbtspecmodel.hpp"

#include <mobata/model/spenat/placeitem.hpp>
#include <mobata/model/spenat/transitionitem.hpp>

#include <mobata/model/ts/testsuiteitem.hpp>
#include <mobata/model/ts/testcaseitem.hpp>

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/actionmanager/actionmanager.h>

#include <projectexplorer/projectexplorer.h>

#include <spenateditor/spenatdocument.hpp>
#include <spenateditor/spenatgraphiceditor.hpp>
#include <spenateditor/spenatgraphicwidget.hpp>
#include <spenateditor/spenatoutlinemodel.hpp>

#include <QApplication>
#include <QPainter>
#include <QHeaderView>
#include <QDebug>

using namespace mbtproject;

////
// MbtSpecModelView
////

class MbtSpecModelView::Private
{
  friend class MbtSpecModelView;

  MbtProject* _currentMbtProject;

  Private()
    : _currentMbtProject(nullptr)
  {}
};

MbtSpecModelView::MbtSpecModelView()
  : _d(new Private)
{
  using namespace ProjectExplorer;

  this->setWindowTitle(tr("Specification Models"));
  this->setWindowIcon(QIcon(QLatin1String(":/mobata/images/spenat/spenat.png")));
  this->setAlternatingRowColors(true);
  this->setUniformRowHeights(true);
  this->header()->hide();

  this->setSelectionMode(QAbstractItemView::SingleSelection);
  //  this->setDragEnabled(true);
  //  this->setAcceptDrops(true);
  //  this->setDropIndicatorShown(true);

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
          this, &MbtSpecModelView::currentProjectChanged);

  ProjectExplorer::Project* currentProject = pePlugin->currentProject();
  MbtProject* mbtProject = qobject_cast<MbtProject*>(currentProject);
  if(mbtProject)
    this->setModel(const_cast<MbtProjectModel*>(mbtProject->mbtProjectModel()));

  Core::EditorManager* em = Core::EditorManager::instance();
  connect(em, &Core::EditorManager::currentEditorChanged,
          this, &MbtSpecModelView::updateCurrentItem);
  connect(this, &MbtSpecModelView::customContextMenuRequested,
          this, &MbtSpecModelView::contextMenuRequested);
}

MbtSpecModelView::~MbtSpecModelView()
{
  delete this->_d;
}

void MbtSpecModelView::currentProjectChanged(ProjectExplorer::Project* currentProject)
{
  if(!currentProject)
    return;

  this->_d->_currentMbtProject = qobject_cast<MbtProject*>(currentProject);
  if(!this->_d->_currentMbtProject)
    return;

  //find and set model
  this->setModel(const_cast<MbtProjectModel*>(this->_d->_currentMbtProject->mbtProjectModel()));

  return;
}

void MbtSpecModelView::updateCurrentItem(Core::IEditor* editor)
{
  if (!editor)
  {
    this->clearSelection();
    return;
  }

  MbtProjectModel const* mbtProjectModel = this->mbtProjectModel();
  if(!mbtProjectModel)
  {
    this->clearSelection();
    return;
  }

  QModelIndex specModelIndex = mbtProjectModel->mbtSpecModelIndexOf(editor->document()->fileName());
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

bool MbtSpecModelView::eventFilter(QObject *obj,
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

///helper method////
QList<QUuid> collectCoverageItemUuids(const QList<model::msc::MscCoverageItem>& mscCoverageItems,
                                      model::spenat::SpenatDeclModel const* declModel)
{
  Q_ASSERT(declModel);

  using namespace model::msc;
  using namespace model::spenat;

  QList<QUuid> coverageItems;

  for(const MscCoverageItem& currMscCoverageItem : mscCoverageItems)
  {
    if(currMscCoverageItem.coverageType() == PlaceCoverageItem)
    {
      PlaceItem const* placeItem = declModel->place(currMscCoverageItem.name());
      if(placeItem)
      {
        coverageItems.append(placeItem->uuid());
        continue;
      }
    }
    else if(currMscCoverageItem.coverageType() == TransitionCoverageItem)
    {
      TransitionItem const* transitionItem = declModel->transition(currMscCoverageItem.name());
      if(transitionItem)
      {
        coverageItems.append(transitionItem->uuid());
        continue;
      }
    }
  }

  return coverageItems;
}

QList<QString> collectCoverageItemNames(const QList<model::msc::MscCoverageItem>& mscCoverageItems,
                                        model::spenat::SpenatDeclModel const* declModel)
{
  Q_ASSERT(declModel);

  using namespace model::msc;
  using namespace model::spenat;

  QList<QString> coverageItems;

  for(const MscCoverageItem& currMscCoverageItem : mscCoverageItems)
  {
    if(currMscCoverageItem.coverageType() == PlaceCoverageItem)
    {
      PlaceItem const* placeItem = declModel->place(currMscCoverageItem.name());
      if(placeItem)
      {
        coverageItems.append(placeItem->name());
        continue;
      }
    }
    else if(currMscCoverageItem.coverageType() == TransitionCoverageItem)
    {
      TransitionItem const* transitionItem = declModel->transition(currMscCoverageItem.name());
      if(transitionItem)
      {
        coverageItems.append(transitionItem->name());
        continue;
      }
    }
  }

  return coverageItems;
}

void MbtSpecModelView::activateEditor(const QModelIndex& index)
{
//  qDebug()<<"MbtSpecModelView::activateEditor() executed!";

  this->selectionModel()->select(index,
                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  MbtProjectModel const* mbtProjectModel = this->mbtProjectModel();
  if(!mbtProjectModel)
    return;

  MbtSpecModel* selectedMbtSpecModel = mbtProjectModel->mbtSpecModelForIndex(index);
  if(!selectedMbtSpecModel)
    return;

  Core::EditorManager::openEditor(selectedMbtSpecModel->mbtSpecModelFilePath());

  return;
}

MbtProjectModel const* MbtSpecModelView::mbtProjectModel() const
{
  return qobject_cast<MbtProjectModel*>(this->model());
}

MbtSpecModel const* mbtSpecModelForTestCase(model::ts::TestCaseItem* testCaseItem,
                                            MbtProjectModel const* mbtProjectModel)
{
  Q_ASSERT(testCaseItem);

  Q_ASSERT(testCaseItem->parent());//test cases
  Q_ASSERT(testCaseItem->parent()->parent());// test suite

  QStandardItem* tcgCoverageItem = testCaseItem->parent()->parent()->parent();
  Q_ASSERT(tcgCoverageItem);//tcg coverage property
  Q_ASSERT(tcgCoverageItem->parent());//tcg coverages parent

  QStandardItem* mbtSpecModelItem = tcgCoverageItem->parent()->parent();
  Q_ASSERT(mbtSpecModelItem);

  MbtSpecModel const* mbtSpecModel = mbtProjectModel->mbtSpecModelForIndex(mbtSpecModelItem->index());
  Q_ASSERT(mbtSpecModel);

  return mbtSpecModel;
}

void MbtSpecModelView::currentChanged(const QModelIndex& current,
                                      const QModelIndex& previous)
{
  using namespace spenateditor;

  MbtProjectModel const* mbtProjectModel = this->mbtProjectModel();
  if(!mbtProjectModel)
    return;

  QStandardItem* previousItem = mbtProjectModel->itemFromIndex(previous);
  if(model::ts::TestCaseItem* prevTestCaseItem = dynamic_cast<model::ts::TestCaseItem*>(previousItem))
  {
    //unhighlight previous test case coverages
    MbtSpecModel const* mbtSpecModel = mbtSpecModelForTestCase(prevTestCaseItem, mbtProjectModel);
    Q_ASSERT(mbtSpecModel);

    for(Core::IEditor* currOpenEditor
        : Core::EditorManager::instance()->openedEditors())
    {
      SpenatGraphicEditor* spenatGraphicEditor = qobject_cast<SpenatGraphicEditor*>(currOpenEditor);
      if(!spenatGraphicEditor)
        continue;

      if(!spenatGraphicEditor->spenatDocument())
        continue;

      if(spenatGraphicEditor->spenatDocument()->fileName() != mbtSpecModel->mbtSpecModelFilePath())
        continue;

      SpenatGraphicWidget* spenatGraphicWidget = spenatGraphicEditor->spenatGraphicWidget();
      Q_ASSERT(spenatGraphicWidget);
      spenatGraphicWidget->unhighlightAllGraphicItems();
    }
  }

  QStandardItem* currentItem = mbtProjectModel->itemFromIndex(current);
  if(!currentItem)
    return;

  if(currentItem->parent()==nullptr)//mbt spec model item
    this->activateEditor(current);
  else if(model::ts::TestCaseItem* currTestCaseItem = dynamic_cast<model::ts::TestCaseItem*>(currentItem))
  {
    if(!this->_d->_currentMbtProject)
      return;

    Q_ASSERT(currTestCaseItem->parent());//test cases
    Q_ASSERT(currTestCaseItem->parent()->parent());// test suite

    QStandardItem* tcgCoverageItem = currTestCaseItem->parent()->parent()->parent();
    Q_ASSERT(tcgCoverageItem);//tcg coverage property

    MbtSpecModel const* mbtSpecModel = mbtSpecModelForTestCase(currTestCaseItem, mbtProjectModel);
    Q_ASSERT(mbtSpecModel);

    //show test case graphic
    TcgCoverageProperty const* tcgCoverageProperty = mbtSpecModel->tcgCoverageProperty(tcgCoverageItem->text());
    Q_ASSERT(tcgCoverageProperty);
    this->_d->_currentMbtProject->showTestCaseGraphic(currTestCaseItem, tcgCoverageProperty);

    //highlight test case coverages
    for(Core::IEditor* currOpenEditor
        : Core::EditorManager::instance()->openedEditors())
    {
      SpenatGraphicEditor* spenatGraphicEditor = qobject_cast<SpenatGraphicEditor*>(currOpenEditor);
      if(!spenatGraphicEditor)
        continue;
      if(!spenatGraphicEditor->spenatDocument())
        continue;

      if(spenatGraphicEditor->spenatDocument()->fileName() != mbtSpecModel->mbtSpecModelFilePath())
        continue;

      QList<QUuid> testCaseCoverageUuids = collectCoverageItemUuids(currTestCaseItem->coverageItems(),
                                                                    spenatGraphicEditor->spenatDeclModel().data());
//      qDebug()<<"test case coverage items count: "<<testCaseCoverageUuids.size();

      if(testCaseCoverageUuids.size())
      {
        SpenatGraphicWidget* spenatGraphicWidget = spenatGraphicEditor->spenatGraphicWidget();
        Q_ASSERT(spenatGraphicWidget);
        spenatGraphicWidget->highlightGraphicItemsByUuids(testCaseCoverageUuids, true);
      }
    }
  }

  return;
}

void MbtSpecModelView::contextMenuRequested(QPoint pos)
{
  if(!this->_d->_currentMbtProject)
    return;

  MbtProjectModel const* mbtProjectModel=this->mbtProjectModel();
  if(!mbtProjectModel)
    return;

  QMenu contextMenu;

  QStandardItem* selectedItem = mbtProjectModel->itemFromIndex(this->indexAt(pos));
  if(model::ts::TestSuiteItem* testSuiteItem = dynamic_cast<model::ts::TestSuiteItem*>(selectedItem))
  {
    Q_ASSERT(testSuiteItem->parent());//tcg coverage item
    Q_ASSERT(testSuiteItem->parent()->parent());// tcg coverage title

    QStandardItem* mbtSpecModelItem = testSuiteItem->parent()->parent()->parent();
    Q_ASSERT(mbtSpecModelItem);
    MbtSpecModel const* mbtSpecModel = mbtProjectModel->mbtSpecModelForIndex(mbtSpecModelItem->index());
    Q_ASSERT(mbtSpecModel);

    Q_ASSERT(this->_d->_currentMbtProject->exportTestSuiteMenu());
    this->_d->_currentMbtProject->exportTestSuiteMenu()->setExportTestSuiteItem(testSuiteItem);
    this->_d->_currentMbtProject->exportTestSuiteMenu()->setMbtSpecModel(mbtSpecModel);
    contextMenu.addMenu(this->_d->_currentMbtProject->exportTestSuiteMenu());
  }

  contextMenu.addSeparator();
  contextMenu.addAction(tr("Expand All"), this, &MbtSpecModelView::expandAll);
  contextMenu.addAction(tr("Collapse All"), this, &MbtSpecModelView::collapseAll);

  contextMenu.exec(this->mapToGlobal(pos));

  return;
}

///
/// MbtSpecModelViewFactory
///

Core::NavigationView MbtSpecModelViewFactory::createWidget()
{
  Core::NavigationView n;
  n.widget = new MbtSpecModelView();
  return n;
}

QString MbtSpecModelViewFactory::displayName() const
{
  return MbtSpecModelView::tr("Specification Models");
}

int MbtSpecModelViewFactory::priority() const
{
  return 200;
}

Core::Id MbtSpecModelViewFactory::id() const
{
  return Core::Id("MbtSpecModelView");
}

QKeySequence MbtSpecModelViewFactory::activationSequence() const
{
  return QKeySequence(Core::UseMacShortcuts ? tr("Meta+M") : tr("Alt+M"));
}

MbtSpecModelViewFactory::MbtSpecModelViewFactory()
{}

MbtSpecModelViewFactory::~MbtSpecModelViewFactory()
{}
