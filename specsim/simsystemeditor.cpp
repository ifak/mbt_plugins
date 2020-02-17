#include "simsystemeditor.hpp"
#include "simsystemeditorwidget.hpp"
#include "specsimconstants.hpp"
#include "specsimplugin.hpp"

#include <coreplugin/coreconstants.h>
#include <texteditor/texteditorsettings.h>

#include <mobata/utils/functors.hpp>

#include <QDir>
#include <QTemporaryFile>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace specsim;
using namespace modsim;

class SimSystemEditor::Private
{
  friend class SimSystemEditor;
};

SimSystemEditor::SimSystemEditor(SimSystemEditorWidget* SimSystemEditorWidget)
  : TextEditor::BaseTextEditor(SimSystemEditorWidget),
    _d(new Private)
{
  this->m_context.add(Constants::SIMSYSTEMEDITOR_CONTEXT);
  this->m_context.add(Constants::LANG_SIMSYSTEM);
  this->m_context.add(TextEditor::Constants::C_TEXTEDITOR);

  //graphic action///
  QAction* simSystemGraphAction=new QAction(QIcon(QLatin1String(":/mobata/images/simsystem/simsystem_blue.png")),
                                         tr("update/create simsystem graphic..."),
                                         this);
  simSystemGraphAction->setStatusTip(tr("simsystem graphic"));
  connect(simSystemGraphAction,
          &QAction::triggered,
          SimSystemEditorWidget,
          &SimSystemEditorWidget::showSimSystemGraphic);
  this->toolBar()->addAction(simSystemGraphAction);
}

SimSystemEditor::~SimSystemEditor()
{
  delete this->_d;
}

Core::Id SimSystemEditor::simSystemEditorId()
{
  return Core::Id(Constants::SIMSYSTEMEDITOR_ID);
}

bool SimSystemEditor::duplicateSupported() const
{
  return true;
}

Core::IEditor* SimSystemEditor::duplicate(QWidget *parent)
{
  SimSystemEditorWidget* simsystemEditorWidget = new SimSystemEditorWidget(parent);
  simsystemEditorWidget->duplicateFrom(this->editorWidget());
  SpecSimPlugin::instance()->initializeEditor(simsystemEditorWidget);
  return simsystemEditorWidget->editor();
}

bool SimSystemEditor::isTemporary() const
{
  return false;
}

Core::Id SimSystemEditor::id() const
{
  return SimSystemEditor::simSystemEditorId();
}

SimSystemDocument* SimSystemEditor::document()
{
  return qobject_cast<SimSystemDocument*>(TextEditor::BaseTextEditor::document());
}

SimSystemModel* SimSystemEditor::simsystemDeclModel()
{
  return this->document()?this->document()->simsystemDeclModel():0;
}

SimSystemOutlineModelPtr SimSystemEditor::simsystemOutlineModel()
{
  return this->document()?this->document()->simsystemOutlineModel():0;
}
