#include "mbtpgraphicdocument.hpp"

#include "mbtpconstants.hpp"

#include <utils/qtcassert.h>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class MbtpGraphicDocument::Private
{
  friend class MbtpGraphicDocument;

  QString             _fileName;

  Private()
  {}
};

MbtpGraphicDocument::MbtpGraphicDocument(QObject* parent)
  : Core::IDocument(parent),
    _d(new Private())
{
}

MbtpGraphicDocument::~MbtpGraphicDocument()
{
  delete this->_d;
}

bool MbtpGraphicDocument::open(QString* errorString,
                              const QString& fileName,
                              const QString& realFileName)
{
  Q_UNUSED(realFileName);
  QTC_ASSERT(errorString, return false);

  this->_d->_fileName=fileName;

  return true;
}

bool MbtpGraphicDocument::save(QString *errorString,
                              const QString &fileName,
                              bool autoSave)
{
  Q_UNUSED(errorString);
  Q_UNUSED(autoSave);

  this->_d->_fileName=fileName;

  return true;
}

QString MbtpGraphicDocument::fileName() const
{
  return this->_d->_fileName;
}

bool MbtpGraphicDocument::isFileReadOnly() const
{
  return true;
}

QString MbtpGraphicDocument::defaultPath() const
{
  return QStringLiteral("");
}

QString MbtpGraphicDocument::suggestedFileName() const
{
  return QStringLiteral("");
}

QString MbtpGraphicDocument::mimeType() const
{
  return QLatin1String(Constants::MBTPGRAPHIC_MIME_TYPE);
}

bool MbtpGraphicDocument::shouldAutoSave() const
{
  return false;
}

bool MbtpGraphicDocument::isModified() const
{
  return false;
}

bool MbtpGraphicDocument::isSaveAsAllowed() const
{
  return false;
}

bool MbtpGraphicDocument::reload(QString *errorString,
                                ReloadFlag flag,
                                ChangeType type)
{
  Q_UNUSED(errorString);
  Q_UNUSED(flag);
  Q_UNUSED(type);

  return true;
}

void MbtpGraphicDocument::rename(const QString &newName)
{
  Q_UNUSED(newName);

  return;
}

} // namespace mbtp

