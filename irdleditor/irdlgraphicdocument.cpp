#include "irdlgraphicdocument.hpp"

#include "irdleditorconstants.hpp"

//#include <mobatawidgets/irdl/declscene.hpp>

#include <mobata/memory_leak_start.hpp>

namespace irdleditor {

class IrdlGraphicDocument::Private
{
  friend class IrdlGraphicDocument;

  QString _fileName;

  Private()
  {}
};

IrdlGraphicDocument::IrdlGraphicDocument(QObject* parent)
  : Core::IDocument(parent),
    _d(new Private)
{}

IrdlGraphicDocument::~IrdlGraphicDocument()
{
  delete this->_d;
}

bool IrdlGraphicDocument::open(QString* errorString,
                                 const QString& fileName,
                                 const QString& realFileName)
{
  Q_UNUSED(errorString);
  Q_UNUSED(realFileName);

  this->_d->_fileName=fileName;

  return true;
}

bool IrdlGraphicDocument::save(QString *errorString,
                                 const QString &fileName,
                                 bool autoSave)
{
  Q_UNUSED(errorString);
  Q_UNUSED(autoSave);

  this->_d->_fileName=fileName;

  return true;
}

QString IrdlGraphicDocument::fileName() const
{
  return this->_d->_fileName;
}

bool IrdlGraphicDocument::isFileReadOnly() const
{
  return true;
}

QString IrdlGraphicDocument::defaultPath() const
{
  return QStringLiteral("");
}

QString IrdlGraphicDocument::suggestedFileName() const
{
  return QStringLiteral("");
}

QString IrdlGraphicDocument::mimeType() const
{
  return QLatin1String(Constants::IRDLGRAPHIC_MIME_TYPE);
}

bool IrdlGraphicDocument::shouldAutoSave() const
{
  return false;
}

bool IrdlGraphicDocument::isModified() const
{
  return false;
}

bool IrdlGraphicDocument::isSaveAsAllowed() const
{
  return false;
}

bool IrdlGraphicDocument::reload(QString *errorString,
                                   ReloadFlag flag,
                                   ChangeType type)
{
  Q_UNUSED(errorString);
  Q_UNUSED(flag);
  Q_UNUSED(type);

  return true;
}

void IrdlGraphicDocument::rename(const QString &newName)
{
  Q_UNUSED(newName);

  return;
}

} // namespace irdleditor
