#include "combuildsimsystemoutlinemodel.hpp"

#include "simsystemoutlinemodel.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace specsim {

ComBuildSimSystemOutlineModel::ComBuildSimSystemOutlineModel(const QString& simsystemDocText,
                                                       SimSystemOutlineModel* simSystemOutlineModel,
                                                       const QString &praefix,
                                                       QObject* parent)
  : modsim::ComBuildSimSystemModel(simsystemDocText, simSystemOutlineModel->simSystemItem(),praefix, false, parent),
    _outlineModel(simSystemOutlineModel)
{}

ComBuildSimSystemOutlineModel::~ComBuildSimSystemOutlineModel()
{}

bool ComBuildSimSystemOutlineModel::execute(QString* errorString)
{
  qDebug()<<"ComBuildSimSystemOutlineModel::execute()!";

  bool result = ComBuildSimSystemModel::execute(errorString);
  if(!result)
    return false;

  SimSystemOutlineModel* simSystemOutlineModel = _outlineModel;
  Q_ASSERT(simSystemOutlineModel);

  simSystemOutlineModel->setModelTextLocations(this->modelTextLocations());
  simSystemOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

  return true;
}

} // namespace simsystemeditor
