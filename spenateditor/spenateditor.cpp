#include "spenateditor.hpp"
#include "spenateditorwidget.hpp"
#include "spenateditorconstants.hpp"
#include "spenateditorplugin.hpp"

#include <coreplugin/coreconstants.h>
#include <texteditor/texteditorsettings.h>

#include <mobata/utils/functors.hpp>

#include <QDir>
#include <QTemporaryFile>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace spenateditor;
using namespace spenateditor::Internal;
using namespace model::spenat;

class SpenatEditor::Private
{
  friend class SpenatEditor;
};

SpenatEditor::SpenatEditor(SpenatEditorWidget* spenatEditorWidget)
  : TextEditor::BaseTextEditor(spenatEditorWidget),
    _d(new Private)
{
  this->m_context.add(Constants::SPENATEDITOR_CONTEXT);
  this->m_context.add(Constants::LANG_SPENAT);
  this->m_context.add(TextEditor::Constants::C_TEXTEDITOR);

  //graphic action///
  QAction* spenatGraphAction=new QAction(QIcon(QLatin1String(":/mobata/images/spenat/spenat_blue.png")),
                                         tr("update/create spenat graphic..."),
                                         this);
  spenatGraphAction->setStatusTip(tr("spenat graphic"));
  connect(spenatGraphAction,
          &QAction::triggered,
          spenatEditorWidget,
          &SpenatEditorWidget::showSpenatGraphic);
  this->toolBar()->addAction(spenatGraphAction);
}

SpenatEditor::~SpenatEditor()
{
  delete this->_d;
}

Core::Id SpenatEditor::spenatEditorId()
{
  return Core::Id(Constants::SPENATEDITOR_ID);
}

bool SpenatEditor::duplicateSupported() const
{
  return true;
}

Core::IEditor* SpenatEditor::duplicate(QWidget *parent)
{
  SpenatEditorWidget* spenatEditorWidget = new SpenatEditorWidget(parent);
  spenatEditorWidget->duplicateFrom(this->spenatEditorWidget());

  SpenatEditorPlugin::instance()->initializeEditor(spenatEditorWidget);;

  return spenatEditorWidget->editor();
}

bool SpenatEditor::isTemporary() const
{
  return false;
}

Core::Id SpenatEditor::id() const
{
  return SpenatEditor::spenatEditorId();
}

SpenatDocument* SpenatEditor::document()
{
  return qobject_cast<SpenatDocument*>(TextEditor::BaseTextEditor::document());
}

SpenatEditorWidget* SpenatEditor::spenatEditorWidget()
{
  SpenatEditorWidget* spenatEditorWidget = qobject_cast<SpenatEditorWidget*>(this->widget());
  Q_ASSERT(spenatEditorWidget);

  return spenatEditorWidget;
}

SpenatDeclModel* SpenatEditor::spenatDeclModel()
{
  return this->document()?this->document()->spenatDeclModel():0;
}

SpenatOutlineModelPtr SpenatEditor::spenatOutlineModel()
{
  if(this->document())
    return this->document()->spenatOutlineModel();

  return SpenatOutlineModelPtr();
}
