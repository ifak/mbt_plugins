#include "spenatgraphicdocument.hpp"

#include "spenateditorconstants.hpp"

#include <mobatawidgets/spenat/declscene.hpp>

#include <mobata/memory_leak_start.hpp>

namespace spenateditor {

class SpenatGraphicDocument::Private
{
  friend class SpenatGraphicDocument;

  QString _fileName;

  Private()
  {}
};

SpenatGraphicDocument::SpenatGraphicDocument(QObject* parent)
  : Core::IDocument(parent),
    _d(new Private)
{}

SpenatGraphicDocument::~SpenatGraphicDocument()
{
  delete this->_d;
}

bool SpenatGraphicDocument::open(QString* errorString,
                                 const QString& fileName,
                                 const QString& realFileName)
{
  Q_UNUSED(errorString);
  Q_UNUSED(realFileName);

  this->_d->_fileName=fileName;

  return true;
}

bool SpenatGraphicDocument::save(QString *errorString,
                                 const QString &fileName,
                                 bool autoSave)
{
  Q_UNUSED(errorString);
  Q_UNUSED(autoSave);

  this->_d->_fileName=fileName;

  return true;
}

QString SpenatGraphicDocument::fileName() const
{
  return this->_d->_fileName;
}

bool SpenatGraphicDocument::isFileReadOnly() const
{
  return true;
}

QString SpenatGraphicDocument::defaultPath() const
{
  return QStringLiteral("");
}

QString SpenatGraphicDocument::suggestedFileName() const
{
  return QStringLiteral("");
}

QString SpenatGraphicDocument::mimeType() const
{
  return QLatin1String(Constants::SPENATGRAPHIC_MIME_TYPE);
}

bool SpenatGraphicDocument::shouldAutoSave() const
{
  return false;
}

bool SpenatGraphicDocument::isModified() const
{
  return false;
}

bool SpenatGraphicDocument::isSaveAsAllowed() const
{
  return false;
}

bool SpenatGraphicDocument::reload(QString *errorString,
                                   ReloadFlag flag,
                                   ChangeType type)
{
  Q_UNUSED(errorString);
  Q_UNUSED(flag);
  Q_UNUSED(type);

  return true;
}

void SpenatGraphicDocument::rename(const QString &newName)
{
  Q_UNUSED(newName);

  return;
}

} // namespace spenateditor
