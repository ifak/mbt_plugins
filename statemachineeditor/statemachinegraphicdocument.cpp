#include "statemachinegraphicdocument.hpp"

#include "statemachineeditorconstants.hpp"

//#include <mobatawidgets/statemachine/declscene.hpp>

#include <mobata/memory_leak_start.hpp>

namespace statemachineeditor {

class StateMachineGraphicDocument::Private
{
  friend class StateMachineGraphicDocument;

  view::statemachine::DeclScene*  _declScene;
  QString                   _fileName;

  Private()
    : _declScene(0)
  {}
};

StateMachineGraphicDocument::StateMachineGraphicDocument(QObject* parent)
  : Core::IDocument(parent),
    _d(new Private)
{
  //this->_d->_declScene=new view::statemachine::DeclScene(this);
}

StateMachineGraphicDocument::~StateMachineGraphicDocument()
{
  delete this->_d;
}

view::statemachine::DeclScene* StateMachineGraphicDocument::declScene()
{
  return this->_d->_declScene;
}

bool StateMachineGraphicDocument::open(QString* errorString,
                                 const QString& fileName,
                                 const QString& realFileName)
{
  Q_UNUSED(errorString);
  Q_UNUSED(realFileName);

  this->_d->_fileName=fileName;

  return true;
}

bool StateMachineGraphicDocument::save(QString *errorString,
                                 const QString &fileName,
                                 bool autoSave)
{
  Q_UNUSED(errorString);
  Q_UNUSED(autoSave);

  this->_d->_fileName=fileName;

  return true;
}

QString StateMachineGraphicDocument::fileName() const
{
  return this->_d->_fileName;
}

bool StateMachineGraphicDocument::isFileReadOnly() const
{
  return true;
}

QString StateMachineGraphicDocument::defaultPath() const
{
  return QStringLiteral("");
}

QString StateMachineGraphicDocument::suggestedFileName() const
{
  return QStringLiteral("");
}

QString StateMachineGraphicDocument::mimeType() const
{
  return QLatin1String(Constants::statemachineGRAPHIC_MIME_TYPE);
}

bool StateMachineGraphicDocument::shouldAutoSave() const
{
  return false;
}

bool StateMachineGraphicDocument::isModified() const
{
  return false;
}

bool StateMachineGraphicDocument::isSaveAsAllowed() const
{
  return false;
}

bool StateMachineGraphicDocument::reload(QString *errorString,
                                   ReloadFlag flag,
                                   ChangeType type)
{
  Q_UNUSED(errorString);
  Q_UNUSED(flag);
  Q_UNUSED(type);

  return true;
}

void StateMachineGraphicDocument::rename(const QString &newName)
{
  Q_UNUSED(newName);

  return;
}

} // namespace statemachineeditor
