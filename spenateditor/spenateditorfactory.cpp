#include "spenateditorfactory.hpp"
#include "spenateditorconstants.hpp"
#include "spenateditorplugin.hpp"
#include "spenateditor.hpp"
#include "spenateditorwidget.hpp"

#include <coreplugin/id.h>
#include <texteditor/texteditorsettings.h>

#include <QStringList>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace spenateditor{
namespace Internal{

class SpenatEditorFactory::Private
{
  friend class SpenatEditorFactory;

  Private(SpenatEditorPlugin*	spenatEditorPlugin)
    :	_spenatEditorPlugin(spenatEditorPlugin),
      _id(Constants::SPENATEDITOR_ID)
  {
    this->_mimeTypes<<QLatin1String(Constants::SPENAT_MIME_TYPE);
  }

public:
  ~Private()
  {}

private:
  SpenatEditorPlugin*       _spenatEditorPlugin;
  Core::Id                  _id;
  QStringList               _mimeTypes;
};

SpenatEditorFactory::SpenatEditorFactory(SpenatEditorPlugin *owner)
  :Core::IEditorFactory(owner),
    _d(new Private(owner))
{
  Q_ASSERT(this->_d->_spenatEditorPlugin);
}

SpenatEditorFactory::~SpenatEditorFactory()
{
  delete this->_d;
}

QStringList SpenatEditorFactory::mimeTypes() const
{
  return this->_d->_mimeTypes;
}

Core::Id SpenatEditorFactory::id() const
{
  return this->_d->_id;
}

QString SpenatEditorFactory::displayName() const
{
  return QLatin1String(Constants::SPENATEDITOR_NAME);
}

Core::IEditor* SpenatEditorFactory::createEditor(QWidget *parent)
{
  SpenatEditorWidget* editorWidget = new SpenatEditorWidget(parent);
  SpenatEditorPlugin::instance()->initializeEditor(editorWidget);

  return editorWidget->editor();
}

} //namespace Internal
} //end namespace spenateditor
