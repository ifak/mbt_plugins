#include "mbtpattributesdocument.hpp"

#include "mbtpconstants.hpp"

#include <utils/qtcassert.h>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class MbtpAttributesDocument::Private
{
  friend class MbtpAttributesDocument;

  QString             _fileName;

  Private()
  {}
};

MbtpAttributesDocument::MbtpAttributesDocument(QObject* parent)
  : Core::IDocument(parent),
    _d(new Private())
{
}

MbtpAttributesDocument::~MbtpAttributesDocument()
{
  delete this->_d;
}

bool MbtpAttributesDocument::open(QString* errorString,
                              const QString& fileName,
                              const QString& realFileName)
{
  Q_UNUSED(realFileName);
  QTC_ASSERT(errorString, return false);

  this->_d->_fileName=fileName;

  return true;
}

bool MbtpAttributesDocument::save(QString *errorString,
                              const QString &fileName,
                              bool autoSave)
{
  Q_UNUSED(errorString);
  Q_UNUSED(autoSave);

  this->_d->_fileName=fileName;

  return true;
}

QString MbtpAttributesDocument::fileName() const
{
  return this->_d->_fileName;
}

bool MbtpAttributesDocument::isFileReadOnly() const
{
  return true;
}

QString MbtpAttributesDocument::defaultPath() const
{
  return QStringLiteral("");
}

QString MbtpAttributesDocument::suggestedFileName() const
{
  return QStringLiteral("");
}

QString MbtpAttributesDocument::mimeType() const
{
  return QLatin1String(Constants::MBTP_ATTRIBUTES_MIME_TYPE);
}

bool MbtpAttributesDocument::shouldAutoSave() const
{
  return false;
}

bool MbtpAttributesDocument::isModified() const
{
  return false;
}

bool MbtpAttributesDocument::isSaveAsAllowed() const
{
  return false;
}

bool MbtpAttributesDocument::reload(QString *errorString,
                                ReloadFlag flag,
                                ChangeType type)
{
  Q_UNUSED(errorString);
  Q_UNUSED(flag);
  Q_UNUSED(type);

  return true;
}

void MbtpAttributesDocument::rename(const QString &newName)
{
  Q_UNUSED(newName);

  return;
}

} // namespace mbtp

