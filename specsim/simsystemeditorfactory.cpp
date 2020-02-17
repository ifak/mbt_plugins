#include "simsystemeditorfactory.hpp"
#include "simsystemeditor.hpp"
#include "simsystemeditorwidget.hpp"
#include "specsimconstants.hpp"
#include "specsimplugin.hpp"

#include <coreplugin/id.h>
#include <texteditor/texteditorsettings.h>

#include <QStringList>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace specsim{

class SimSystemEditorFactory::Private
{
  friend class SimSystemEditorFactory;

  Private(SpecSimPlugin*	SpecSimPlugin)
    :	_simSystemEditorPlugin(SpecSimPlugin),
      _id(Constants::SIMSYSTEMEDITOR_ID)
  {
    this->_mimeTypes<<QLatin1String(Constants::SIMSYSTEM_MIME_TYPE);
  }

public:
  ~Private()
  {}

private:
  SpecSimPlugin*       _simSystemEditorPlugin;
  Core::Id                  _id;
  QStringList               _mimeTypes;
};

SimSystemEditorFactory::SimSystemEditorFactory(SpecSimPlugin *owner)
  :Core::IEditorFactory(owner),
    _d(new Private(owner))
{
  Q_ASSERT(this->_d->_simSystemEditorPlugin);
}

SimSystemEditorFactory::~SimSystemEditorFactory()
{
  delete this->_d;
}

QStringList SimSystemEditorFactory::mimeTypes() const
{
  return this->_d->_mimeTypes;
}

Core::Id SimSystemEditorFactory::id() const
{
  return this->_d->_id;
}

QString SimSystemEditorFactory::displayName() const
{
  return QLatin1String(Constants::SIMSYSTEMEDITOR_NAME);
}

Core::IEditor* SimSystemEditorFactory::createEditor(QWidget *parent)
{
  SimSystemEditorWidget* editorWidget = new SimSystemEditorWidget(parent);
  SpecSimPlugin::instance()->initializeEditor(editorWidget);

  return editorWidget->editor();
}

} //end namespace simsystemeditor
