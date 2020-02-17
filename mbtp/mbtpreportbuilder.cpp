#include "mbtpreportbuilder.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QtDebug>

#include <QTime>
#include "mbtptablemodel.hpp"
#include "mbtpproject.hpp"

namespace mbtp {

MbtpReportBuilder::MbtpReportBuilder(QObject* parent)
  : QObject(parent)
{
  _mbtpRuntimeModel = new MbtpTableModel(4);
}

MbtpReportBuilder::~MbtpReportBuilder()
{
  delete _mbtpRuntimeModel;
}

void MbtpReportBuilder::reset()
{
  if(this->_mbtpRuntimeModel)
    delete this->_mbtpRuntimeModel;

  this->_mbtpRuntimeModel=nullptr;

  return;
}

void MbtpReportBuilder::buildReport(MbtpRuntime* mbtpRuntime,
                                    QString* errorString)
{
  Q_UNUSED(errorString);
  Q_ASSERT(mbtpRuntime);

  this->reset();

  this->_mbtpRuntimeModel = new MbtpTableModel(4);

  QString json = mbtpRuntime->project()->resultString();

  QJsonDocument doc;
  doc = QJsonDocument::fromJson(json.toUtf8());
  QJsonObject jsonObj = doc.object();
  QJsonArray cases = jsonObj["testcases"].toArray();
  for (QJsonValue value : cases) {
    QJsonObject obj = value.toObject();
    QVector<QVariant> vector;
    vector.append(obj.value("pos").toInt());
    vector.append(obj.value("id").toString());
    vector.append(obj.value("prio").toDouble());
    vector.append(obj.value("cluster").toString());
    this->_mbtpRuntimeModel->pushBack(vector);
  }
}

MbtpTableModel* MbtpReportBuilder::mbtpRuntimeModel(){
  return _mbtpRuntimeModel;
}

int MbtpReportBuilder::getExcludetCount(){
  return _excludetCount;
}

int MbtpReportBuilder::getFailedCount(){
  return _failedCount;
}

int MbtpReportBuilder::getNotExecutedCount(){
  return _notExecuedCount;
}

int MbtpReportBuilder::getSucceededCount(){
  return _succeededCount;
}


MbtpReportImageProvider::MbtpReportImageProvider()
  : QQuickImageProvider (QQuickImageProvider::Image)
{

}

QImage MbtpReportImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
  Q_UNUSED(id);
  Q_UNUSED(size);
  Q_UNUSED(requestedSize);
  return this->_image;
}

 void MbtpReportImageProvider::setImage(QImage image)
 {
   this->_image = image;
 }



}
