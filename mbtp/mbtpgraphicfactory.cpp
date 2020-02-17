#include "mbtpgraphicfactory.hpp"

#include "mbtpconstants.hpp"
#include "mbtpgraphiceditor.hpp"

#include <QDebug>
#include <QtQuickWidgets/QQuickWidget>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

MbtpGraphicFactory::MbtpGraphicFactory(QObject* parent)
  : Core::IEditorFactory(parent)
{
    qDebug()<< "MbtpGraphicFactory::MbtpGraphicFactory";
}

MbtpGraphicFactory::~MbtpGraphicFactory()
{}

Core::IEditor* MbtpGraphicFactory::createEditor(QWidget* parent)
{
  qDebug()<< "MbtpGraphicFactory::createEditor";
  return new MbtpGraphicEditor(new QQuickWidget(parent));
}

QStringList MbtpGraphicFactory::mimeTypes() const
{
  return QStringList()<< QLatin1String(Constants::MBTPGRAPHIC_MIME_TYPE);
}

Core::Id MbtpGraphicFactory::id() const
{
  return Core::Id(Constants::MBTP_GRAPHIC);
}

QString MbtpGraphicFactory::displayName() const
{
  return QLatin1String(Constants::MBTP_GRAPHIC_NAME);
}

} // namespace mbtp
