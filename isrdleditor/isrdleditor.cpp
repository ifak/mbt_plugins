#include "isrdleditor.hpp"
#include "isrdleditorwidget.hpp"
#include "isrdleditorconstants.hpp"
#include "isrdleditorplugin.hpp"

#include <coreplugin/coreconstants.h>
#include <texteditor/texteditorsettings.h>

#include <mobata/utils/functors.hpp>

#include <QDir>
#include <QTemporaryFile>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace isrdleditor;
using namespace isrdleditor::Internal;
using namespace model::isrdl;

class IsrdlEditor::Private
{
  friend class IsrdlEditor;
};

IsrdlEditor::IsrdlEditor(IsrdlEditorWidget* isrdlEditorWidget)
  : TextEditor::BaseTextEditor(isrdlEditorWidget),
    _d(new Private)
{
  this->m_context.add(Constants::ISRDLEDITOR_CONTEXT);
  this->m_context.add(Constants::LANG_ISRDL);
  this->m_context.add(TextEditor::Constants::C_TEXTEDITOR);

  //graphic action///
  QAction* isrdlGraphAction=new QAction(QIcon(QLatin1String(":/mobata/images/msc/sequence.png")),
                                         tr("update/create isrdl graphic..."),
                                         this);
  isrdlGraphAction->setStatusTip(tr("isrdl graphic"));
  connect(isrdlGraphAction,
          &QAction::triggered,
          isrdlEditorWidget,
          &IsrdlEditorWidget::showIsrdlGraphic);
  this->toolBar()->addAction(isrdlGraphAction);
}

IsrdlEditor::~IsrdlEditor()
{
  delete this->_d;
}

Core::Id IsrdlEditor::isrdlEditorId()
{
  return Core::Id(Constants::ISRDLEDITOR_ID);
}

bool IsrdlEditor::duplicateSupported() const
{
  return false;
}

Core::IEditor* IsrdlEditor::duplicate(QWidget *parent)
{
  IsrdlEditorWidget* isrdlEditorWidget = new IsrdlEditorWidget(parent);
  isrdlEditorWidget->duplicateFrom(this->isrdlEditorWidget());

  IsrdlEditorPlugin::instance()->initializeEditor(isrdlEditorWidget);;

  return isrdlEditorWidget->editor();
}

bool IsrdlEditor::isTemporary() const
{
  return false;
}

Core::Id IsrdlEditor::id() const
{
  return IsrdlEditor::isrdlEditorId();
}

IsrdlDocument* IsrdlEditor::document()
{
  return qobject_cast<IsrdlDocument*>(TextEditor::BaseTextEditor::document());
}

IsrdlEditorWidget* IsrdlEditor::isrdlEditorWidget()
{
  IsrdlEditorWidget* isrdlEditorWidget = qobject_cast<IsrdlEditorWidget*>(this->widget());
  Q_ASSERT(isrdlEditorWidget);

  return isrdlEditorWidget;
}

IsrdlModel *IsrdlEditor::isrdlDeclModel()
{
  return this->document()?this->document()->isrdlDeclModel():0;
}

IsrdlOutlineModel* IsrdlEditor::isrdlOutlineModel()
{
  return this->document()?this->document()->isrdlOutlineModel():0;
}
