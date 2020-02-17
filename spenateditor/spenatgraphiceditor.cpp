#include "spenatgraphiceditor.hpp"

#include "spenatgraphicdocument.hpp"
#include "spenatgraphicwidget.hpp"
#include "spenateditorconstants.hpp"
#include "spenateditor.hpp"

#include <mobata/utils/functors.hpp>

#include <QFileInfo>
#include <QToolBar>

#include <mobata/memory_leak_start.hpp>

namespace spenateditor{

class SpenatGraphicEditor::Private
{
  friend class SpenatGraphicEditor;

  Private()
  {}

  SpenatDeclModelPtr        _spenatDeclModel = nullptr;
  SpenatDocument const*     _spenatDocument = nullptr;
  SpenatGraphicDocument*    _spenatGraphicDocument = nullptr;
  QSet<SpenatEditor const*> _connectedSpenatEditors;
};

SpenatGraphicEditor::SpenatGraphicEditor(SpenatGraphicWidget* spenatGraphicWidget)
  : dsleditor::DslGraphicEditor(Constants::SPENATGRAPHIC_ID),
    _d(new Private)
{
  Q_ASSERT(spenatGraphicWidget);

  this->_d->_spenatGraphicDocument=new SpenatGraphicDocument(this);
  this->setWidget(spenatGraphicWidget);
}

SpenatGraphicEditor::~SpenatGraphicEditor()
{
  delete this->m_widget;
  delete this->_d;
}

bool SpenatGraphicEditor::open(QString* errorString,
                               const QString& fileName,
                               const QString& realFileName)
{
  Q_UNUSED(errorString);
  Q_UNUSED(realFileName);

  bool result=this->_d->_spenatGraphicDocument->open(errorString,
                                                     fileName,
                                                     realFileName);

  if(!result)
    return false;

  QFileInfo fileInfo(this->_d->_spenatGraphicDocument->fileName());
  if(!fileInfo.exists())
    return false;

  this->setDisplayName(fileInfo.fileName());

  return true;
}

Core::IDocument*  SpenatGraphicEditor::document()
{
  return this->_d->_spenatGraphicDocument;
}

QWidget* SpenatGraphicEditor::toolBar()
{
  SpenatGraphicWidget* spenatGraphicWidget = this->spenatGraphicWidget();
  Q_ASSERT(spenatGraphicWidget);

  return spenatGraphicWidget->toolBar();
}

void SpenatGraphicEditor::setSpenatDeclModel(const SpenatDeclModelPtr& spenatDeclModel)
{
  this->_d->_spenatDeclModel=spenatDeclModel;

  return;
}

const SpenatDeclModelPtr& SpenatGraphicEditor::spenatDeclModel() const
{
  return this->_d->_spenatDeclModel;
}

void SpenatGraphicEditor::setSpenatDocument(SpenatDocument const* spenatDocument)
{
  this->_d->_spenatDocument = spenatDocument;

  return;
}

SpenatDocument const* SpenatGraphicEditor::spenatDocument() const
{
  return this->_d->_spenatDocument;
}

SpenatGraphicWidget* SpenatGraphicEditor::spenatGraphicWidget()
{
  SpenatGraphicWidget* spenatGraphicWidget = qobject_cast<SpenatGraphicWidget*>(this->m_widget);

  return spenatGraphicWidget;
}

QString SpenatGraphicEditor::fileName() const
{
  return this->_d->_spenatGraphicDocument->fileName();
}

} // namespace spenateditor
