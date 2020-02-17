#include "mbtpattributeseditor.hpp"

#include "mbtpconstants.hpp"
#include "mbtpattributesdocument.hpp"

#include <QFile>
#include <QString>
#include <QByteArray>
#include <QFileInfo>
#include <QQuickWidget>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <projectexplorer/projectexplorer.h>
#include <mbtpproject.hpp>

#include <mobata/utils/functors.hpp>

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class MbtpAttributesEditor::Private
{
  friend class MbtpAttributesEditor;

  Private()
    : _MbtpAttributesDocument(nullptr)
  {}
  QString                    _displayName;
  MbtpAttributesDocument* _MbtpAttributesDocument;
  QQuickWidget* _widget = nullptr;

  QStandardItemModel* _weightModel;
  QStandardItemModel* _requirementModel;
};

MbtpAttributesEditor::MbtpAttributesEditor(QQuickWidget* MbtpAttributesWidget)
  : Core::IEditor(MbtpAttributesWidget), _d(new Private)
{
  Q_ASSERT(MbtpAttributesWidget);

  this->_d->_MbtpAttributesDocument = new MbtpAttributesDocument(this);
  this->setWidget(MbtpAttributesWidget);
}

MbtpAttributesEditor::~MbtpAttributesEditor()
{
  delete this->_d->_MbtpAttributesDocument;
  delete this->_d;
}

bool MbtpAttributesEditor::createNew(const QString &contents)
{
  Q_UNUSED(contents);

  return true;
}

bool MbtpAttributesEditor::open(QString* errorString,
                                   const QString& fileName,
                                   const QString& realFileName)
{
  bool result=this->_d->_MbtpAttributesDocument->open(errorString,
                                                         fileName,
                                                         realFileName);

  if(!result)
    return false;

  QFileInfo fileInfo(fileName);
  if(!fileInfo.exists())
  {
    *errorString = QLatin1String("File not found!") + fileName;
    return false;
  }

  this->_d->_displayName = fileInfo.fileName();

  this->_d->_widget = qobject_cast<QQuickWidget*>(this->widget());

  this->_d->_requirementModel = new QStandardItemModel();
  QList<QStandardItem*> reqNameList;
  reqNameList.append(new QStandardItem("Requirement"));
  this->_d->_weightModel = new QStandardItemModel();
  QList<QStandardItem*> weiHeader;
  QStandardItem* weiHeadName = new QStandardItem("Attribute");
  QStandardItem* weiHeadWeight = new QStandardItem("Weight");
  weiHeader.append(weiHeadName);
  weiHeader.append(weiHeadWeight);

  this->_d->_weightModel->appendRow(weiHeader);


  QFile att(this->_d->_MbtpAttributesDocument->fileName());
  att.open(QIODevice::ReadOnly);

  QJsonDocument doc = QJsonDocument::fromJson(att.readAll());
  att.close();

  QStringList attNames;

  QJsonObject obj = doc.object();

  QJsonArray wei = obj.value("attributes").toArray();
  if(!wei.isEmpty()){
    for(QJsonValue weiVal : wei){
      QJsonObject weiObj = weiVal.toObject();
      if(!weiObj.isEmpty()){
        QList<QStandardItem*> weiList;
        weiList.append(new QStandardItem(weiObj.value("name").toString()));
        weiList.append(new QStandardItem(QString::number(weiObj.value("weight").toInt())));
        reqNameList.append(new QStandardItem(weiObj.value("name").toString()));
        attNames.append(weiObj.value("name").toString());
        this->_d->_weightModel->appendRow(weiList);
        weiList.clear();
      }
    }
  }

  this->_d->_requirementModel->appendRow(reqNameList);
  reqNameList.clear();

  QJsonArray req = obj.value("requirements").toArray();

  if(!req.isEmpty()){
    for(QJsonValue val : req){
      QList<QStandardItem*> stdObjList;
      QJsonObject subObj = val.toObject();
      if(!subObj.isEmpty()){
        QString name = subObj.value("name").toString();
        QStandardItem* nameStd = new QStandardItem(name);
        stdObjList.append(nameStd);

        QJsonArray attarr = subObj.value("attributes").toArray();
        for(QString attName : attNames){
          bool found = false;
          for(QJsonValue arrVal : attarr){
            QJsonObject attObj = arrVal.toObject();
            if(!attObj.isEmpty()){
              if(attObj.value("name").toString() == attName){
                found = true;
                stdObjList.append(new QStandardItem(QString::number(attObj.value("value").toInt())));
                break;
              }
            }
          }
          if(!found){
            stdObjList.append(new QStandardItem("0"));
          }
        }
      }
      this->_d->_requirementModel->appendRow(stdObjList);
      stdObjList.clear();
    }
  }

  this->_d->_widget->rootContext()->setContextProperty("reqModel", this->_d->_requirementModel);
  this->_d->_widget->rootContext()->setContextProperty("weiModel", this->_d->_weightModel);

  if(!this->_d->_widget)
    return false;

  this->_d->_widget->setSource(QUrl(QStringLiteral("qrc:/attributeseditor.qml")));
  this->_d->_widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
  this->_d->_widget->show();

  QObject* saveButton = this->_d->_widget->rootObject()->findChild<QObject*>("saveButton");
  if(saveButton){
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButton()));
  }else{
    qDebug()<<"saveButton not found";
  }

  QObject* closeButton = this->_d->_widget->rootObject()->findChild<QObject*>("closeButton");
  closeButton->setProperty("visible", false);
  if(closeButton){
    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(closeButton()));
  }else{
    qDebug()<<"closeButton not found";
  }

  QObject* addAttributeButton = this->_d->_widget->rootObject()->findChild<QObject*>("addAttributeButton");
  if(addAttributeButton){
    QObject::connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttributeButton()));
  }else{
    qDebug()<<"addAttributeButton not found";
  }

  QObject* removeAttributeButton = this->_d->_widget->rootObject()->findChild<QObject*>("removeAttributeButton");
  if(removeAttributeButton){
    QObject::connect(removeAttributeButton, SIGNAL(clicked()), this, SLOT(removeAttributeButton()));
  }else{
    qDebug()<<"removeAttributeButton not found";
  }

  QObject* renameAttributeButton = this->_d->_widget->rootObject()->findChild<QObject*>("renameAttributeButton");
  if(renameAttributeButton){
    QObject::connect(renameAttributeButton, SIGNAL(clicked()), this, SLOT(renameAttributeButton()));
  }else{
    qDebug()<<"renameAttributeButton not found";
  }

  QObject* importCC = this->_d->_widget->rootObject()->findChild<QObject*>("importCC");
  if(importCC){
    QObject::connect(importCC, SIGNAL(clicked()), this, SLOT(importCC()));
  }else{
    qDebug()<<"importCC Button not found";
  }

  readReq();

  return true;
}

Core::IDocument*  MbtpAttributesEditor::document()
{
  return this->_d->_MbtpAttributesDocument;;
}

Core::Id MbtpAttributesEditor::id() const
{
  return Core::Id(Constants::MBTP_ATTRIBUTES);
}

QString MbtpAttributesEditor::displayName() const
{
  return this->_d->_displayName;
}

void MbtpAttributesEditor::setDisplayName(const QString &title)
{
  this->_d->_displayName=title;
}

bool MbtpAttributesEditor::isTemporary() const
{
  return true;
}

QWidget* MbtpAttributesEditor::toolBar()
{
  return nullptr;
}

void MbtpAttributesEditor::saveButton()
{
  qDebug()<<"save clicked";

  QJsonDocument doc;
  QJsonObject obj;
  QJsonArray attributes;
  QJsonArray requirements;

  for(int i = 1; i < this->_d->_weightModel->rowCount(); i++){
      QJsonObject curAttribute;
      curAttribute.insert("name", this->_d->_weightModel->item(i,0)->data(Qt::DisplayRole).toString());
      curAttribute.insert("weight", this->_d->_weightModel->item(i,1)->data(Qt::DisplayRole).toInt());
      attributes.append(curAttribute);
  }

  for(int i = 1; i < this->_d->_requirementModel->rowCount(); i++){
    QJsonObject curReq;
    curReq.insert("name", this->_d->_requirementModel->item(i,0)->data(Qt::DisplayRole).toString());
    QJsonArray thisAttributes;
    for(int j = 1; j < this->_d->_requirementModel->columnCount(); j++){
      QJsonObject attObj;
      attObj.insert("name", this->_d->_requirementModel->item(0,j)->data(Qt::DisplayRole).toString());
      attObj.insert("value", this->_d->_requirementModel->item(i,j)->data(Qt::DisplayRole).toInt());
      thisAttributes.append(attObj);
    }
    curReq.insert("attributes",thisAttributes);
    requirements.append(curReq);
  }

  obj.insert("attributes",attributes);
  obj.insert("requirements",requirements);
  doc.setObject(obj);

  QFile att(this->_d->_MbtpAttributesDocument->fileName());
  att.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
  att.write(doc.toJson());
  att.close();
}

void MbtpAttributesEditor::closeButton()
{
  qDebug()<<"close clicked";
}

void MbtpAttributesEditor::addAttributeButton()
{
  qDebug()<<"add clicked";
  bool ok;
  QString text = QInputDialog::getText(this->_d->_widget, tr("New Attribute Name"),
                                       tr("Attribute name:"), QLineEdit::Normal,
                                       "", &ok);
  if(!ok || text.isEmpty()){
    return;
  }
  this->addAttribute(text);

}

void MbtpAttributesEditor::addAttribute(const QString& attributeName)
{
  QStringList myItems;
  for(int i = 1; i < this->_d->_weightModel->rowCount(); i++){
    myItems.append(this->_d->_weightModel->item(i,0)->data(Qt::DisplayRole).toString());
  }

  if(myItems.contains(attributeName)){
    QMessageBox::warning(this->_d->_widget,"Attribute could not added","The attribute could not added, because it already exists");
    return;
  }

  QList<QStandardItem*> addWeight;
  addWeight.append(new QStandardItem(attributeName));
  addWeight.append(new QStandardItem("0"));
  this->_d->_weightModel->appendRow(addWeight);

  QList<QStandardItem*> addAtt;
  addAtt.append(new QStandardItem(attributeName));
  for(int i = 1; i < this->_d->_requirementModel->rowCount(); i++){
    addAtt.append(new QStandardItem("0"));
  }
  this->_d->_requirementModel->appendColumn(addAtt);
}

void MbtpAttributesEditor::removeAttributeButton()
{
  qDebug()<<"remove clicked";

  QStringList myItems;
  for(int i = 1; i < this->_d->_weightModel->rowCount(); i++){
    myItems.append(this->_d->_weightModel->item(i,0)->data(Qt::DisplayRole).toString());
  }

  if(myItems.isEmpty()){
    QMessageBox::warning(this->_d->_widget,"Attribute could not removed","There are no Attributes");
    return;
  }

  bool ok;
  QString text = QInputDialog::getItem(this->_d->_widget, tr("Remove Attribute"),
                                       tr("Attribute name:"), myItems,
                                       0, false, &ok);

  if(!ok){
    return;
  }

  int index = myItems.indexOf(text);

  if(index < 0){
    return;
  }

  this->_d->_weightModel->removeRow(index+1);
  this->_d->_requirementModel->removeColumn(index+1);
}

void MbtpAttributesEditor::renameAttributeButton()
{
  qDebug()<<"rename clicked";

  QStringList myItems;
  for(int i = 1; i < this->_d->_weightModel->rowCount(); i++){
    myItems.append(this->_d->_weightModel->item(i,0)->data(Qt::DisplayRole).toString());
  }

  if(myItems.isEmpty()){
    QMessageBox::warning(this->_d->_widget,"Attribute could not renamed","There are no Attributes");
    return;
  }

  bool ok;
  QString text = QInputDialog::getItem(this->_d->_widget, tr("Rename Attribute"),
                                       tr("Attribute name:"), myItems,
                                       0, false, &ok);
  QString newText = QInputDialog::getText(this->_d->_widget, tr("Rename Attribute"), tr("New Name"));

  if(newText == ""){
    QMessageBox::warning(this->_d->_widget,"Attribute could not renamed","Empty name entered");
    return;
  }

  if(!ok){
    return;
  }

  int index = myItems.indexOf(text);

  if(index < 0){
    return;
  }

  if(myItems.contains(newText)){
    QMessageBox::warning(this->_d->_widget,"Attribute could not renamed","Attribute with new name already exists");
    return;
  }

  this->_d->_weightModel->item(index+1,0)->setData(newText, Qt::DisplayRole);
  this->_d->_requirementModel->item(0,index+1)->setData(newText, Qt::DisplayRole);
}

void MbtpAttributesEditor::importCC()
{
  QString filename = QFileDialog::getOpenFileName(this->widget(),"Cyclomatic Complexity File", "", "Json (*.json)");
  if(filename == ""){
    return;
  }
  QFile file(filename);
  file.open(QIODevice::ReadOnly);
  QString content = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
  QJsonObject obj = doc.object();
  int max = 0;
  for(QString key : obj.keys()){
    if(obj.value(key).toInt() > max){
      max = obj.value(key).toInt();
    }
  }

  this->addAttribute("CyclomaticComplexity");

  for(QString key : obj.keys()){
    int scaledValue = qRound(((static_cast<double>(obj.value(key).toInt()) / static_cast<double>(max))) * 100.0);
    qDebug()<<scaledValue;
    for(int i = 1; i < this->_d->_requirementModel->rowCount(); i++){
      if(this->_d->_requirementModel->item(i,0)->data(Qt::DisplayRole) == key){
        this->_d->_requirementModel->item(i,this->_d->_requirementModel->columnCount()-1)->setData(scaledValue, Qt::DisplayRole);
        break;
      }
    }
  }
}

void MbtpAttributesEditor::readReq()
{
  QStringList reqList;

  MbtpProject* pro = dynamic_cast<MbtpProject*>(ProjectExplorer::ProjectExplorerPlugin::instance()->currentProject());
  for (const QString& file : pro->files(ProjectExplorer::Project::AllFiles)){
    if(file.right(14) == ".testcasetrace"){
      QFile f(file);
      f.open(QIODevice::ReadOnly);
      QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
      f.close();
      QJsonObject obj = doc.object();
      QJsonArray tcArr = obj.value("testcases").toArray();
      for(QJsonValue val : tcArr){
        QJsonObject o = val.toObject();
        QJsonArray a = o.value("requirements").toArray();
        for(QJsonValue v : a){
          QString reqString = v.toString();
          if(reqString == ""){
            continue;
          }
          if(reqList.contains(reqString)){
            continue;
          }
          reqList.append(reqString);
        }
      }

    }
  }

  for(int i = 1; i < this->_d->_requirementModel->rowCount(); i++){
    QString reqName = this->_d->_requirementModel->item(i,0)->data(Qt::DisplayRole).toString();
    if(reqList.contains(reqName)){
      reqList.removeAll(reqName);
    }
  }
  for(const QString& str : reqList){
    QStandardItem* itm = new QStandardItem;
    itm->setData(str, Qt::DisplayRole);
    this->_d->_requirementModel->appendRow(itm);
  }
}

} // namespace mbtp
