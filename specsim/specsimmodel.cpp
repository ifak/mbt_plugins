#include "specsimmodel.hpp"

#include "specsimproject.hpp"

#include <mobata/utils/functors.hpp>
#include <mobata/model/spenat/spenatmodel.hpp>
#include <mobata/model/spenat/comcreatespenatmodel.hpp>

#include <dslparser/spenat/combuildspenatmodel.hpp>
#include <dslparser/statemachine/combuildstatemachinemodel.hpp>

#include <QFileInfo>
#include <QIcon>
#include <QJsonDocument>

namespace specsim {

class SpecSimModel::Private
{
  friend class SpecSimModel;

  QString                           _specSimModelFilePath;
  QIcon                             _specSimModelIcon;
  bool                              _enabled;
  QList<ProjectExplorer::BaseError> _errors;

  Private(const QString& specSimModelFilePath,
          const QIcon& specSimModelIcon)
    : _specSimModelFilePath(specSimModelFilePath),
      _specSimModelIcon(specSimModelIcon),
      _enabled(false)
  {}
};

SpecSimModel::SpecSimModel(const QString& specSimModelFilePath,
                           const QIcon& specSimModelIcon,
                           QObject *parent)
  : QObject(parent),
    _d(new Private(specSimModelFilePath, specSimModelIcon))
{}

SpecSimModel::~SpecSimModel()
{
  delete this->_d;
}

const QList<ProjectExplorer::BaseError>&  SpecSimModel::errors() const
{
  return this->_d->_errors;
}

QString SpecSimModel::name() const
{
  return QFileInfo(this->_d->_specSimModelFilePath).baseName();
}

const QString& SpecSimModel::specSimModelFilePath() const
{
  return this->_d->_specSimModelFilePath;
}

const QIcon& SpecSimModel::icon() const
{
  return this->_d->_specSimModelIcon;
}

void SpecSimModel::setEnabled(bool enabled)
{
  this->_d->_enabled=enabled;

  return;
}

bool SpecSimModel::isEnabled() const
{
  return this->_d->_enabled;
}

void SpecSimModel::clearErrors()
{
  this->_d->_errors.clear();

  return;
}

void SpecSimModel::addDslError(const dslparser::DslError& dslError)
{
  SpecSimProject::addDslError(dslError, this->_d->_specSimModelFilePath, &this->_d->_errors);

  return;
}

class SpecSimSpenatModel::Private
{
  friend class SpecSimSpenatModel;

  model::spenat::SpenatDeclModel* _spenatDeclModel;
  QString                         _spenatModelJson;

  Private()
    : _spenatDeclModel(0)
  {}
};


///SpecSimSpenatModel////////////////////////////
SpecSimSpenatModel::SpecSimSpenatModel(const QString& spenatFilePath,
                               QObject* parent)
  : SpecSimModel(spenatFilePath,
                 QIcon(QStringLiteral(":/mobata/images/spenat/transition.png")),
                 parent),
    _d(new Private)
{
  this->_d->_spenatDeclModel = new model::spenat::SpenatDeclModel(this);
}

SpecSimSpenatModel::~SpecSimSpenatModel()
{
  delete this->_d;
}

model::spenat::SpenatDeclModel const* SpecSimSpenatModel::specModel() const
{
  return this->_d->_spenatDeclModel;
}

bool SpecSimSpenatModel::rebuildSpecModel(QString* errorMessage)
{
  Q_ASSERT(this->_d->_spenatDeclModel);

  this->clearErrors();
  this->_d->_spenatDeclModel->reset();
  this->_d->_spenatModelJson.clear();

  QFile spenatFile(this->specSimModelFilePath());
  if(!spenatFile.exists())
  {
    utils::AddPtrString(errorMessage)<<tr("spenat model file '%1' does not exist!")
                                       .arg(this->specSimModelFilePath());
    return false;
  }

  if(!spenatFile.open(QIODevice::Text | QIODevice::ReadOnly))
  {
    utils::AddPtrString(errorMessage)<<tr("spenat model file '%1' could not be opened!")
                                       .arg(this->specSimModelFilePath());
    return false;
  }

  QString spenatDocText = QString::fromLatin1(spenatFile.readAll());
  spenatFile.close();

  dslparser::spenat::ComBuildSpenatModel buildCommand(spenatDocText, this->_d->_spenatDeclModel);
  bool result = buildCommand.execute(errorMessage);
  for(const dslparser::DslError& dslError : buildCommand.errors())
    this->addDslError(dslError);

  if(result)
  {
    model::spenat::ComCreateSpenatModel command(this->_d->_spenatDeclModel);
    result = command.execute(errorMessage);
    if(result)
    {
      model::spenat::ComCreateSpenatModel::SpenatModelPtr spenatModelPtr = command.spenatModel();
      Q_ASSERT(spenatModelPtr);
      model::base::JsonDocPtr tcgJsonDocPtr =spenatModelPtr->createJsonDocument();
      Q_ASSERT(tcgJsonDocPtr);
      this->_d->_spenatModelJson = QLatin1String(tcgJsonDocPtr->toJson());
    }
  }

  return result;
}

QString SpecSimSpenatModel::toJson() const
{
  return this->_d->_spenatModelJson;
}

///SpecSimStateMachineModel////////////////////////////////
class SpecSimStateMachineModel::Private
{
  friend class SpecSimStateMachineModel;

  model::statemachine::StateMachineModel* _stateMachineModel;
  QString                                 _stateMachineModelJson;

  Private()
    : _stateMachineModel(nullptr)
  {}
};

SpecSimStateMachineModel::SpecSimStateMachineModel(const QString& stateMachineFilePath,
                                           QObject* parent)
  : SpecSimModel(stateMachineFilePath,
                 QIcon(QStringLiteral(":/mobata/images/base/model.png")),
                 parent),
    _d(new Private)
{
  this->_d->_stateMachineModel = new model::statemachine::StateMachineModel(this);
}

SpecSimStateMachineModel::~SpecSimStateMachineModel()
{
  delete this->_d;
}

model::statemachine::StateMachineModel const* SpecSimStateMachineModel::specModel() const
{
  return this->_d->_stateMachineModel;
}

bool SpecSimStateMachineModel::rebuildSpecModel(QString* errorMessage)
{
  Q_ASSERT(this->_d->_stateMachineModel);

  this->clearErrors();
  this->_d->_stateMachineModel->reset();
  this->_d->_stateMachineModelJson.clear();

  QFile stateMachineFile(this->specSimModelFilePath());
  if(!stateMachineFile.exists())
  {
    utils::AddPtrString(errorMessage)<<tr("state machine model file '%1' does not exist!")
                                       .arg(this->specSimModelFilePath());
    return false;
  }

  if(!stateMachineFile.open(QIODevice::Text | QIODevice::ReadOnly))
  {
    utils::AddPtrString(errorMessage)<<tr("state machine model file '%1' could not be opened!")
                                       .arg(this->specSimModelFilePath());
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

    this->_d->_stateMachineModelJson =  QLatin1String(stateMachineJsonDocument.toJson());
  }
  return result;

}

QString SpecSimStateMachineModel::toJson() const
{
  return this->_d->_stateMachineModelJson;
}

} // namespace specsim
