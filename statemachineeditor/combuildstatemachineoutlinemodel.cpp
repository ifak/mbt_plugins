#include "combuildstatemachineoutlinemodel.hpp"

#include "statemachineoutlinemodel.hpp"

#include <QDebug>

#include <mobata/memory_leak_start.hpp>

namespace statemachineeditor {

ComBuildStateMachineOutlineModel::ComBuildStateMachineOutlineModel(const QString& statemachineDocText,
                                                       StateMachineOutlineModel* stateMachineOutlineModel,
                                                       const QString &praefix,
                                                       QObject* parent)
  : dslparser::statemachine::ComBuildStateMachineModel(statemachineDocText, stateMachineOutlineModel,praefix, false, parent)
{}

ComBuildStateMachineOutlineModel::~ComBuildStateMachineOutlineModel()
{}

bool ComBuildStateMachineOutlineModel::execute(QString* errorString)
{
  qDebug()<<"ComBuildStateMachineOutlineModel::execute()!";

  bool result = ComBuildStateMachineModel::execute(errorString);
  if(!result)
    return false;

  StateMachineOutlineModel* stateMachineOutlineModel = static_cast<StateMachineOutlineModel*>(this->statemachineDeclModel());
  Q_ASSERT(stateMachineOutlineModel);

  stateMachineOutlineModel->setModelTextLocations(this->modelTextLocations());
  stateMachineOutlineModel->setKeywordTextLocations(this->keywordTextLocations());

  return true;
}

} // namespace statemachineeditor
