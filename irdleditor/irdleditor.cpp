#include "irdleditor.hpp"
#include "irdleditorwidget.hpp"
#include "irdleditorconstants.hpp"
#include "irdleditorplugin.hpp"

#include <coreplugin/coreconstants.h>
#include <texteditor/texteditorsettings.h>

#include <mobata/utils/functors.hpp>

#include <QDir>
#include <QTemporaryFile>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace irdleditor;
using namespace irdleditor::Internal;
using namespace model::irdl;

class IrdlEditor::Private
{
  friend class IrdlEditor;
};

IrdlEditor::IrdlEditor(IrdlEditorWidget* irdlEditorWidget)
  : TextEditor::BaseTextEditor(irdlEditorWidget),
    _d(new Private)
{
  this->m_context.add(Constants::IRDLEDITOR_CONTEXT);
  this->m_context.add(Constants::LANG_IRDL);
  this->m_context.add(TextEditor::Constants::C_TEXTEDITOR);

  //graphic action///
  QAction* irdlGraphAction=new QAction(QIcon(QLatin1String(":/mobata/images/msc/sequence.png")),
                                         tr("update/create irdl graphic..."),
                                         this);
  irdlGraphAction->setStatusTip(tr("irdl graphic"));
  connect(irdlGraphAction,
          &QAction::triggered,
          irdlEditorWidget,
          &IrdlEditorWidget::showIrdlGraphic);
  this->toolBar()->addAction(irdlGraphAction);
}

IrdlEditor::~IrdlEditor()
{
  delete this->_d;
}

Core::Id IrdlEditor::irdlEditorId()
{
  return Core::Id(Constants::IRDLEDITOR_ID);
}

bool IrdlEditor::duplicateSupported() const
{
  return false;
}

Core::IEditor* IrdlEditor::duplicate(QWidget *parent)
{
  IrdlEditorWidget* irdlEditorWidget = new IrdlEditorWidget(parent);
  irdlEditorWidget->duplicateFrom(this->irdlEditorWidget());

  IrdlEditorPlugin::instance()->initializeEditor(irdlEditorWidget);;

  return irdlEditorWidget->editor();
}

bool IrdlEditor::isTemporary() const
{
  return false;
}

Core::Id IrdlEditor::id() const
{
  return IrdlEditor::irdlEditorId();
}

IrdlDocument* IrdlEditor::document()
{
  return qobject_cast<IrdlDocument*>(TextEditor::BaseTextEditor::document());
}

IrdlEditorWidget* IrdlEditor::irdlEditorWidget()
{
  IrdlEditorWidget* irdlEditorWidget = qobject_cast<IrdlEditorWidget*>(this->widget());
  Q_ASSERT(irdlEditorWidget);

  return irdlEditorWidget;
}

RequirementsModel *IrdlEditor::irdlDeclModel()
{
  return this->document()?this->document()->irdlDeclModel():0;
}

IrdlOutlineModel* IrdlEditor::irdlOutlineModel()
{
  return this->document()?this->document()->irdlOutlineModel():0;
}
