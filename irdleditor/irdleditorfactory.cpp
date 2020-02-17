#include "irdleditorfactory.hpp"
#include "irdleditorconstants.hpp"
#include "irdleditorplugin.hpp"
#include "irdleditor.hpp"
#include "irdleditorwidget.hpp"

#include <coreplugin/id.h>
#include <texteditor/texteditorsettings.h>

#include <QStringList>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace irdleditor{
namespace Internal{

class IrdlEditorFactory::Private
{
  friend class IrdlEditorFactory;

  Private(IrdlEditorPlugin*	irdlEditorPlugin)
    :	_irdlEditorPlugin(irdlEditorPlugin),
      _id(Constants::IRDLEDITOR_ID)
  {
    this->_mimeTypes<<QLatin1String(Constants::REQUIREMENT_MIME_TYPE)<<QLatin1String(Constants::REQUIREMENTDECL_MIME_TYPE);
  }

public:
  ~Private()
  {}

private:
  IrdlEditorPlugin*       _irdlEditorPlugin;
  Core::Id                  _id;
  QStringList               _mimeTypes;
};

IrdlEditorFactory::IrdlEditorFactory(IrdlEditorPlugin *owner)
  :Core::IEditorFactory(owner),
    _d(new Private(owner))
{
  Q_ASSERT(this->_d->_irdlEditorPlugin);
}

IrdlEditorFactory::~IrdlEditorFactory()
{
  delete this->_d;
}

QStringList IrdlEditorFactory::mimeTypes() const
{
  return this->_d->_mimeTypes;
}

Core::Id IrdlEditorFactory::id() const
{
  return this->_d->_id;
}

QString IrdlEditorFactory::displayName() const
{
  return QLatin1String(Constants::IRDLEDITOR_NAME);
}

Core::IEditor* IrdlEditorFactory::createEditor(QWidget *parent)
{
  IrdlEditorWidget* editorWidget = new IrdlEditorWidget(parent);
  IrdlEditorPlugin::instance()->initializeEditor(editorWidget);

  return editorWidget->editor();
}

} //namespace Internal
} //end namespace irdleditor
