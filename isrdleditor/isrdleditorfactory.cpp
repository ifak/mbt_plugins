#include "isrdleditorfactory.hpp"
#include "isrdleditorconstants.hpp"
#include "isrdleditorplugin.hpp"
#include "isrdleditor.hpp"
#include "isrdleditorwidget.hpp"

#include <coreplugin/id.h>
#include <texteditor/texteditorsettings.h>

#include <QStringList>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace isrdleditor{
namespace Internal{

class IsrdlEditorFactory::Private
{
  friend class IsrdlEditorFactory;

  Private(IsrdlEditorPlugin*	isrdlEditorPlugin)
    :	_isrdlEditorPlugin(isrdlEditorPlugin),
      _id(Constants::ISRDLEDITOR_ID)
  {
    this->_mimeTypes<<QLatin1String(Constants::REQUIREMENT_MIME_TYPE)<<QLatin1String(Constants::REQUIREMENTDECL_MIME_TYPE);
  }

public:
  ~Private()
  {}

private:
  IsrdlEditorPlugin*       _isrdlEditorPlugin;
  Core::Id                  _id;
  QStringList               _mimeTypes;
};

IsrdlEditorFactory::IsrdlEditorFactory(IsrdlEditorPlugin *owner)
  :Core::IEditorFactory(owner),
    _d(new Private(owner))
{
  Q_ASSERT(this->_d->_isrdlEditorPlugin);
}

IsrdlEditorFactory::~IsrdlEditorFactory()
{
  delete this->_d;
}

QStringList IsrdlEditorFactory::mimeTypes() const
{
  return this->_d->_mimeTypes;
}

Core::Id IsrdlEditorFactory::id() const
{
  return this->_d->_id;
}

QString IsrdlEditorFactory::displayName() const
{
  return QLatin1String(Constants::ISRDLEDITOR_NAME);
}

Core::IEditor* IsrdlEditorFactory::createEditor(QWidget *parent)
{
  IsrdlEditorWidget* editorWidget = new IsrdlEditorWidget(parent);
  IsrdlEditorPlugin::instance()->initializeEditor(editorWidget);

  return editorWidget->editor();
}

} //namespace Internal
} //end namespace isrdleditor
