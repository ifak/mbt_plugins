#include "commondecleditor.hpp"
#include "commondecleditorwidget.hpp"
#include "commondecleditorconstants.hpp"
#include "commondecleditorplugin.hpp"

#include <coreplugin/coreconstants.h>
#include <texteditor/texteditorsettings.h>

#include <mobata/utils/functors.hpp>

#include <QDir>
#include <QTemporaryFile>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace commondecleditor;
using namespace commondecleditor::Internal;
using namespace model::base;

class CommonDeclEditor::Private
{
  friend class CommonDeclEditor;
};

CommonDeclEditor::CommonDeclEditor(CommonDeclEditorWidget* commondeclEditorWidget)
  : TextEditor::BaseTextEditor(commondeclEditorWidget),
    _d(new Private)
{
  this->m_context.add(Constants::COMMONDECLEDITOR_CONTEXT);
  this->m_context.add(Constants::LANG_COMMONDECL);
  this->m_context.add(TextEditor::Constants::C_TEXTEDITOR);
}

CommonDeclEditor::~CommonDeclEditor()
{
  delete this->_d;
}

Core::Id CommonDeclEditor::commondeclEditorId()
{
  return Core::Id(Constants::COMMONDECLEDITOR_ID);
}

bool CommonDeclEditor::duplicateSupported() const
{
  return true;
}

Core::IEditor* CommonDeclEditor::duplicate(QWidget *parent)
{
  CommonDeclEditorWidget* commondeclEditorWidget = new CommonDeclEditorWidget(parent);
  commondeclEditorWidget->duplicateFrom(this->commondeclEditorWidget());

  CommonDeclEditorPlugin::instance()->initializeEditor(commondeclEditorWidget);;

  return commondeclEditorWidget->editor();
}

bool CommonDeclEditor::isTemporary() const
{
  return false;
}

Core::Id CommonDeclEditor::id() const
{
  return CommonDeclEditor::commondeclEditorId();
}

CommonDeclDocument* CommonDeclEditor::document()
{
  return qobject_cast<CommonDeclDocument*>(TextEditor::BaseTextEditor::document());
}

CommonDeclEditorWidget* CommonDeclEditor::commondeclEditorWidget()
{
  CommonDeclEditorWidget* commondeclEditorWidget = qobject_cast<CommonDeclEditorWidget*>(this->widget());
  Q_ASSERT(commondeclEditorWidget);

  return commondeclEditorWidget;
}

BaseModel *CommonDeclEditor::commondeclDeclModel()
{
  return this->document()?this->document()->commondeclDeclModel():0;
}

CommonDeclOutlineModel* CommonDeclEditor::commondeclOutlineModel()
{
  return this->document()?this->document()->commondeclOutlineModel():0;
}
