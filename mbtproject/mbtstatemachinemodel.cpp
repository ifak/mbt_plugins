#include "mbtstatemachinemodel.hpp"

#include <dslparser/statemachine/combuildstatemachinemodel.hpp>

#include <QIcon>
#include <QJsonDocument>

using namespace mbtproject;

class MbtStateMachineModel::Private
{
  friend class MbtStateMachineModel;

  model::statemachine::StateMachineModel* _stateMachineModel;
  QString _tcgStatemachineModelJson;

  Private()
    : _stateMachineModel(0)
  {}
};

MbtStateMachineModel::MbtStateMachineModel(const QString& stateMachineFilePath,
                                           QObject* parent)
  : MbtSpecModel(stateMachineFilePath,
                 QIcon(QStringLiteral(":/mobata/images/base/model.png")),
                 parent),
    _d(new Private)
{
  this->_d->_stateMachineModel = new model::statemachine::StateMachineModel(this);
}

MbtStateMachineModel::~MbtStateMachineModel()
{
  delete this->_d;
}

model::statemachine::StateMachineModel const* MbtStateMachineModel::specModel() const
{
  return this->_d->_stateMachineModel;
}

bool MbtStateMachineModel::rebuildSpecModel(QString* errorMessage)
{
  Q_ASSERT(this->_d->_stateMachineModel);

  this->clearErrors();
  this->_d->_stateMachineModel->reset();
  this->_d->_tcgStatemachineModelJson.clear();

  QFile stateMachineFile(this->mbtSpecModelFilePath());
  if(!stateMachineFile.exists())
  {
    utils::AddPtrString(errorMessage)<<tr("state machine model file '%1' does not exist!")
                                       .arg(this->mbtSpecModelFilePath());
    return false;
  }

  if(!stateMachineFile.open(QIODevice::Text | QIODevice::ReadOnly))
  {
    utils::AddPtrString(errorMessage)<<tr("state machine model file '%1' could not be opened!")
                                       .arg(this->mbtSpecModelFilePath());
    return false;
  }

  QString stateMachineDocText = QString::fromLatin1(stateMachineFile.readAll());
  stateMachineFile.close();

  dslparser::statemachine::ComBuildStateMachineModel buildCommand(stateMachineDocText,
                                                                  this->_d->_stateMachineModel);
  bool result = buildCommand.execute(errorMessage);
  for(const dslparser::DslError& dslError : buildCommand.errors())
    this->addDslError(dslError);

  if(result)
  {
    model::base::JsonObjectPtr stateMachineJsonObject=this->_d->_stateMachineModel->createJson();
    Q_ASSERT(stateMachineJsonObject.data());
    QJsonDocument stateMachineJsonDocument(*stateMachineJsonObject.data());

    this->_d->_tcgStatemachineModelJson =  QLatin1String(stateMachineJsonDocument.toJson());
  }
  return result;

}

QString MbtStateMachineModel::toJson() const
{
  return this->_d->_tcgStatemachineModelJson;
}
