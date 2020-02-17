#include "combuildspenatoutlinemodel.hpp"

#include "spenatoutlinemodel.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace spenateditor {

ComBuildSpenatOutlineModel::ComBuildSpenatOutlineModel(const QString& spenatDocText,
                                                       SpenatOutlineModel* spenatOutlineModel,
                                                       QObject* parent)
  : dslparser::spenat::ComBuildSpenatModel(spenatDocText, spenatOutlineModel, false, parent)
{}

ComBuildSpenatOutlineModel::~ComBuildSpenatOutlineModel()
{}

bool ComBuildSpenatOutlineModel::execute(QString* errorString)
{
//  qDebug()<<"ComBuildSpenatOutlineModel::execute()!";

  bool result = ComBuildSpenatModel::execute(errorString);
  if(!result)
    return false;

  SpenatOutlineModel* spenatOutlineModel = static_cast<SpenatOutlineModel*>(this->spenatDeclModel());
  Q_ASSERT(spenatOutlineModel);

  spenatOutlineModel->setModelTextLocations(this->modelTextLocations());
  spenatOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

  return true;
}

} // namespace spenateditor
