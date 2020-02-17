#include "irdlgraphiceditor.hpp"

#include "irdlgraphicdocument.hpp"
#include "irdlgraphicwidget.hpp"
#include "irdleditorconstants.hpp"

#include <mobata/utils/functors.hpp>
//#include <mobatawidgets/irdl/declscene.hpp>

#include <QFileInfo>
#include <QToolBar>

#include <mobata/memory_leak_start.hpp>

namespace irdleditor {

class IrdlGraphicEditor::Private
{
  friend class IrdlGraphicEditor;

  Private()
  {}

  IrdlOutlineModel const* _irdlOutlineModel = nullptr;
  IrdlDocument const*     _irdlDocument = nullptr;
  IrdlGraphicDocument*    _irdlGraphicDocument = nullptr;
  QString                         _displayName;
};

IrdlGraphicEditor::IrdlGraphicEditor(IrdlGraphicWidget* irdlGraphicWidget)
  : Core::IEditor(irdlGraphicWidget), _d(new Private)
{
  Q_ASSERT(irdlGraphicWidget);

  this->_d->_irdlGraphicDocument=new IrdlGraphicDocument(this);
  this->setWidget(irdlGraphicWidget);
}

IrdlGraphicEditor::~IrdlGraphicEditor()
{
  delete this->_d;
}

bool IrdlGraphicEditor::createNew(const QString &contents)
{
  Q_UNUSED(contents);

  return true;
}

bool IrdlGraphicEditor::open(QString* errorString,
                               const QString& fileName,
                               const QString& realFileName)
{
  Q_UNUSED(errorString);
  Q_UNUSED(realFileName);

  bool result=this->_d->_irdlGraphicDocument->open(errorString,
                                                     fileName,
                                                     realFileName);

  if(!result)
    return false;

  QFileInfo fileInfo(this->_d->_irdlGraphicDocument->fileName());
  if(!fileInfo.exists())
    return false;

  this->_d->_displayName = fileInfo.fileName();

  return true;
}

Core::IDocument*  IrdlGraphicEditor::document()
{
  return this->_d->_irdlGraphicDocument;
}

Core::Id IrdlGraphicEditor::id() const
{
  return Core::Id(Constants::IRDLGRAPHIC_ID);
}

QString IrdlGraphicEditor::displayName() const
{
  return this->_d->_displayName;
}

void IrdlGraphicEditor::setDisplayName(const QString &title)
{
  this->_d->_displayName=title;
}

bool IrdlGraphicEditor::isTemporary() const
{
  return true;
}

QWidget* IrdlGraphicEditor::toolBar()
{
  IrdlGraphicWidget* irdlGraphicWidget = this->irdlGraphicWidget();
  Q_ASSERT(irdlGraphicWidget);

  return irdlGraphicWidget->toolBar();
}

void IrdlGraphicEditor::setIrdlOutlineModel(IrdlOutlineModel const* irdlOutlineModel)
{
  this->_d->_irdlOutlineModel=irdlOutlineModel;

  return;
}

IrdlOutlineModel const* IrdlGraphicEditor::irdlOutlineModel() const
{
  return this->_d->_irdlOutlineModel;
}

void IrdlGraphicEditor::setIrdlDocument(IrdlDocument const* irdlDocument)
{
  this->_d->_irdlDocument = irdlDocument;

  return;
}

IrdlDocument const* IrdlGraphicEditor::irdlDocument() const
{
  return this->_d->_irdlDocument;
}

IrdlGraphicWidget* IrdlGraphicEditor::irdlGraphicWidget()
{
  IrdlGraphicWidget* irdlGraphicWidget = qobject_cast<IrdlGraphicWidget*>(this->widget());
  Q_ASSERT(irdlGraphicWidget);

  return irdlGraphicWidget;
}

QString IrdlGraphicEditor::fileName() const
{
  return this->_d->_irdlGraphicDocument->fileName();
}

} // namespace irdleditor
