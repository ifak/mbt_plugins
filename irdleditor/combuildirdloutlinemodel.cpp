#include "combuildirdloutlinemodel.hpp"

#include "irdloutlinemodel.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace irdleditor {

ComBuildIrdlOutlineModel::ComBuildIrdlOutlineModel(const QString& irdlDocText,
                                                       IrdlOutlineModel* irdlOutlineModel,
                                                       QString prefix,
                                                       QObject* parent)
  : dslparser::irdl::ComBuildIrdlModel(irdlDocText, irdlOutlineModel,prefix, false, parent)
{}

ComBuildIrdlOutlineModel::~ComBuildIrdlOutlineModel()
{}

bool ComBuildIrdlOutlineModel::execute(QString* errorString, bool deleteOldModel)
{
  qDebug()<<"ComBuildIrdlOutlineModel::execute()!";

  bool result = ComBuildIrdlModel::execute(errorString, deleteOldModel);
  if(!result)
    return false;

  IrdlOutlineModel* irdlOutlineModel = static_cast<IrdlOutlineModel*>(this->irdlDeclModel());
  Q_ASSERT(irdlOutlineModel);

  irdlOutlineModel->setModelTextLocations(this->modelTextLocations());
  irdlOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

  return true;
}

ComBuildIrdlDeclOutlineModel::ComBuildIrdlDeclOutlineModel(const QString& irdlDocText,
                                                           IrdlOutlineModel* irdlOutlineModel,
                                                           QString prefix,
                                                           QObject* parent)
    : dslparser::irdl::ComBuildIrdlDeclModel(irdlDocText, irdlOutlineModel,prefix, false, parent)
{}

ComBuildIrdlDeclOutlineModel::~ComBuildIrdlDeclOutlineModel()
{

}

bool ComBuildIrdlDeclOutlineModel::execute(QString *errorString, bool deleteOldModel)
{
    Q_UNUSED(deleteOldModel);
    qDebug()<<"ComBuildIrdlOutlineModel::execute()!";

    bool result = dslparser::irdl::ComBuildIrdlDeclModel::execute(errorString);
    if(!result)
      return false;

    IrdlOutlineModel* irdlOutlineModel = static_cast<IrdlOutlineModel*>(this->IrdlCommonDeclModel());
    Q_ASSERT(irdlOutlineModel);

    irdlOutlineModel->setModelTextLocations(this->modelTextLocations());
    irdlOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

    return true;
}

} // namespace irdleditor
