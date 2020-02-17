#include "combuildisrdloutlinemodel.hpp"

#include "isrdloutlinemodel.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace isrdleditor {

ComBuildIsrdlOutlineModel::ComBuildIsrdlOutlineModel(const QString& isrdlDocText,
                                                       IsrdlOutlineModel* isrdlOutlineModel,
                                                       QString prefix,
                                                       QObject* parent)
  : dslparser::isrdl::ComBuildIsrdlModel(isrdlDocText, isrdlOutlineModel, false, parent)
{
  Q_UNUSED(prefix);
}

ComBuildIsrdlOutlineModel::~ComBuildIsrdlOutlineModel()
{}

bool ComBuildIsrdlOutlineModel::execute(QString* errorString)
{
  qDebug()<<"ComBuildIsrdlOutlineModel::execute()!";

  bool result = ComBuildIsrdlModel::execute(errorString);
  if(!result)
    return false;

  IsrdlOutlineModel* isrdlOutlineModel = static_cast<IsrdlOutlineModel*>(this->isrdlModel());
  Q_ASSERT(isrdlOutlineModel);

  isrdlOutlineModel->setModelTextLocations(this->modelTextLocations());
  isrdlOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

  return true;
}

} // namespace isrdleditor
