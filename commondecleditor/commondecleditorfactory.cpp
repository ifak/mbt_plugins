#include "commondecleditorfactory.hpp"
#include "commondecleditorconstants.hpp"
#include "commondecleditorplugin.hpp"
#include "commondecleditor.hpp"
#include "commondecleditorwidget.hpp"

#include <coreplugin/id.h>
#include <texteditor/texteditorsettings.h>

#include <QStringList>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace commondecleditor{
namespace Internal{

class CommonDeclEditorFactory::Private
{
  friend class CommonDeclEditorFactory;

  Private(CommonDeclEditorPlugin*	commondeclEditorPlugin)
    :	_commondeclEditorPlugin(commondeclEditorPlugin),
      _id(Constants::COMMONDECLEDITOR_ID)
  {
    this->_mimeTypes<<QLatin1String(Constants::COMMONDECL_MIME_TYPE);
  }

public:
  ~Private()
  {}

private:
  CommonDeclEditorPlugin*       _commondeclEditorPlugin;
  Core::Id                  _id;
  QStringList               _mimeTypes;
};

CommonDeclEditorFactory::CommonDeclEditorFactory(CommonDeclEditorPlugin *owner)
  :Core::IEditorFactory(owner),
    _d(new Private(owner))
{
  Q_ASSERT(this->_d->_commondeclEditorPlugin);
}

CommonDeclEditorFactory::~CommonDeclEditorFactory()
{
  delete this->_d;
}

QStringList CommonDeclEditorFactory::mimeTypes() const
{
  return this->_d->_mimeTypes;
}

Core::Id CommonDeclEditorFactory::id() const
{
  return this->_d->_id;
}

QString CommonDeclEditorFactory::displayName() const
{
  return QLatin1String(Constants::COMMONDECLEDITOR_NAME);
}

Core::IEditor* CommonDeclEditorFactory::createEditor(QWidget *parent)
{
  CommonDeclEditorWidget* editorWidget = new CommonDeclEditorWidget(parent);
  CommonDeclEditorPlugin::instance()->initializeEditor(editorWidget);

  return editorWidget->editor();
}

} //namespace Internal
} //end namespace commondecleditor
