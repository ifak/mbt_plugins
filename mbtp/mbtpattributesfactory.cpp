#include "mbtpattributesfactory.hpp"

#include "mbtpconstants.hpp"
#include "mbtpattributeseditor.hpp"

#include <QDebug>
#include <QtQuickWidgets/QQuickWidget>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

MbtpAttributesFactory::MbtpAttributesFactory(QObject* parent)
  : Core::IEditorFactory(parent)
{
    qDebug()<< "MbtpAttributesFactory::MbtpAttributesFactory";
}

MbtpAttributesFactory::~MbtpAttributesFactory()
{}

Core::IEditor* MbtpAttributesFactory::createEditor(QWidget* parent)
{
  qDebug()<< "MbtpAttributesFactory::createEditor";
  return new MbtpAttributesEditor(new QQuickWidget(parent));
}

QStringList MbtpAttributesFactory::mimeTypes() const
{
  return QStringList()<< QLatin1String(Constants::MBTP_ATTRIBUTES_MIME_TYPE);
}

Core::Id MbtpAttributesFactory::id() const
{
  return Core::Id(Constants::MBTP_ATTRIBUTES);
}

QString MbtpAttributesFactory::displayName() const
{
  return QLatin1String(Constants::MBTP_ATTRIBUTES_NAME);
}

} // namespace mbtp
