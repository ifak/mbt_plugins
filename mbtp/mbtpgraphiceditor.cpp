#include "mbtpgraphiceditor.hpp"

#include "mbtpconstants.hpp"
#include "mbtpgraphicdocument.hpp"

#include <QFile>
#include <QString>
#include <QByteArray>
#include <QFileInfo>
#include <QQuickWidget>
#include <QQmlEngine>

#include <mobata/utils/functors.hpp>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class MbtpGraphicEditor::Private
{
  friend class MbtpGraphicEditor;

  Private()
    : _MbtpGraphicDocument(nullptr)
  {}
  QString                    _displayName;
  MbtpGraphicDocument* _MbtpGraphicDocument;
};

MbtpGraphicEditor::MbtpGraphicEditor(QQuickWidget* MbtpGraphicWidget)
  : Core::IEditor(MbtpGraphicWidget), _d(new Private)
{
  Q_ASSERT(MbtpGraphicWidget);

  this->_d->_MbtpGraphicDocument=new MbtpGraphicDocument(this);
  this->setWidget(MbtpGraphicWidget);
}

MbtpGraphicEditor::~MbtpGraphicEditor()
{
  delete this->_d->_MbtpGraphicDocument;
  delete this->_d;
}

bool MbtpGraphicEditor::createNew(const QString &contents)
{
  Q_UNUSED(contents);

  return true;
}

bool MbtpGraphicEditor::open(QString* errorString,
                                   const QString& fileName,
                                   const QString& realFileName)
{
  bool result=this->_d->_MbtpGraphicDocument->open(errorString,
                                                         fileName,
                                                         realFileName);

  if(!result)
    return false;

  QFileInfo fileInfo(fileName);
  if(!fileInfo.exists())
  {
    *errorString = QLatin1String("File not found!") + fileName;
    return false;
  }

  this->_d->_displayName = fileInfo.fileName();

  return true;
}

Core::IDocument*  MbtpGraphicEditor::document()
{
  return this->_d->_MbtpGraphicDocument;;
}

Core::Id MbtpGraphicEditor::id() const
{
  return Core::Id(Constants::MBTP_GRAPHIC);
}

QString MbtpGraphicEditor::displayName() const
{
  return this->_d->_displayName;
}

void MbtpGraphicEditor::setDisplayName(const QString &title)
{
  this->_d->_displayName=title;
}

bool MbtpGraphicEditor::isTemporary() const
{
  return true;
}

QWidget* MbtpGraphicEditor::toolBar()
{
  return nullptr;
}

} // namespace mbtp
