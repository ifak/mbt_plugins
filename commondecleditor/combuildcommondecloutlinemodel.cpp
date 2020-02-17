#include "combuildcommondecloutlinemodel.hpp"

#include "commondecloutlinemodel.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace commondecleditor {

ComBuildCommonDeclOutlineModel::ComBuildCommonDeclOutlineModel(const QString& commondeclDocText,
                                                       CommonDeclOutlineModel* commondeclOutlineModel,
                                                       QString ,
                                                       QObject* parent)
  : dslparser::common::ComBuildBaseModel(commondeclDocText, commondeclOutlineModel, false, parent)
{}

ComBuildCommonDeclOutlineModel::~ComBuildCommonDeclOutlineModel()
{}

bool ComBuildCommonDeclOutlineModel::execute(QString* errorString)
{
  qDebug()<<"ComBuildCommonDeclOutlineModel::execute()!";

  bool result = ComBuildBaseModel::execute(errorString);
  if(!result)
    return false;

  CommonDeclOutlineModel* commondeclOutlineModel = static_cast<CommonDeclOutlineModel*>(this->baseDeclModel());
  Q_ASSERT(commondeclOutlineModel);

  commondeclOutlineModel->setModelTextLocations(this->modelTextLocations());
  commondeclOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

  return true;
}

} // namespace commondecleditor
