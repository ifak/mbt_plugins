#include "statemachinegraphiceditor.hpp"

#include "statemachinegraphicdocument.hpp"
#include "statemachinegraphicwidget.hpp"
#include "statemachineeditorconstants.hpp"
#include "statemachineeditor.hpp"

#include <coreplugin/editormanager/editormanager.h>

#include <mobata/utils/functors.hpp>
#include <mobata/model/statemachine/statemachinemodel.hpp>

#include <QFileInfo>
#include <QToolBar>

#include <mobata/memory_leak_start.hpp>

namespace statemachineeditor {

class StateMachineGraphicEditor::Private
{
  friend class StateMachineGraphicEditor;

  Private()
    : _stateMachineModel(0),
      _stateMachineGraphicDocument(0)
  {}

  StateMachineModelPtr            _stateMachineModel = nullptr;
  StateMachineDocument const*     _stateMachineDocument = nullptr;
  StateMachineGraphicDocument*    _stateMachineGraphicDocument = nullptr;
  QSet<StateMachineEditor const*> _connectedStateMachineEditors;
};

StateMachineGraphicEditor::StateMachineGraphicEditor(StateMachineGraphicWidget* stateMachineGraphicWidget)
  : dsleditor::DslGraphicEditor(Constants::statemachineGRAPHIC_ID),
    _d(new Private)
{
  Q_ASSERT(stateMachineGraphicWidget);

  this->_d->_stateMachineGraphicDocument=new StateMachineGraphicDocument(this);
  this->setWidget(stateMachineGraphicWidget);
}

StateMachineGraphicEditor::~StateMachineGraphicEditor()
{
  if(this->m_widget)
    delete this->m_widget;
  if(this->_d)
    delete this->_d;
}

bool StateMachineGraphicEditor::open(QString* errorString,
                                     const QString& fileName,
                                     const QString& realFileName)
{
  Q_UNUSED(errorString);
  Q_UNUSED(realFileName);

  bool result=this->_d->_stateMachineGraphicDocument->open(errorString,
                                                           fileName,
                                                           realFileName);

  if(!result)
    return false;

  QFileInfo fileInfo(this->_d->_stateMachineGraphicDocument->fileName());
  if(!fileInfo.exists())
    return false;

  this->setDisplayName(fileInfo.fileName());

  return true;
}

Core::IDocument*  StateMachineGraphicEditor::document()
{
  return this->_d->_stateMachineGraphicDocument;
}

QWidget* StateMachineGraphicEditor::toolBar()
{
  StateMachineGraphicWidget* stateMachineGraphicWidget = this->stateMachineGraphicWidget();
  Q_ASSERT(stateMachineGraphicWidget);

  return stateMachineGraphicWidget->toolBar();
}

void StateMachineGraphicEditor::setStateMachineModel(const StateMachineModelPtr& stateMachineModel)
{
  this->_d->_stateMachineModel=stateMachineModel;

  return;
}

const StateMachineModelPtr& StateMachineGraphicEditor::stateMachineModel() const
{
  return this->_d->_stateMachineModel;
}


void StateMachineGraphicEditor::setStateMachineDocument(StateMachineDocument const* stateMachineDocument)
{
  this->_d->_stateMachineDocument = stateMachineDocument;

  return;
}

StateMachineDocument const* StateMachineGraphicEditor::stateMachineDocument() const
{
  return this->_d->_stateMachineDocument;
}

StateMachineGraphicWidget* StateMachineGraphicEditor::stateMachineGraphicWidget()
{
  StateMachineGraphicWidget* stateMachineGraphicWidget = qobject_cast<StateMachineGraphicWidget*>(this->m_widget);

  return stateMachineGraphicWidget;
}

QString StateMachineGraphicEditor::fileName() const
{
  return this->_d->_stateMachineGraphicDocument->fileName();
}

} // namespace statemachineeditor
