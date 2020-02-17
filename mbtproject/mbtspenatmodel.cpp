#include "mbtspenatmodel.hpp"

#include <dslparser/spenat/combuildspenatmodel.hpp>

#include <mobata/utils/functors.hpp>
#include <mobata/model/spenat/comcreatespenatmodel.hpp>
#include <mobata/model/spenat/spenatmodel.hpp>

#include <QIcon>
#include <QJsonObject>
#include <QJsonDocument>

using namespace mbtproject;

class MbtSpenatModel::Private
{
  friend class MbtSpenatModel;

  model::spenat::SpenatDeclModel* _spenatDeclModel;
  QString                         _tcgSpenatModelJson;

  Private()
    : _spenatDeclModel(0)
  {}
};

MbtSpenatModel::MbtSpenatModel(const QString& spenatFilePath,
                               QObject* parent)
  : MbtSpecModel(spenatFilePath,
                 QIcon(QStringLiteral(":/mobata/images/spenat/transition.png")),
                 parent),
    _d(new Private)
{
  this->_d->_spenatDeclModel = new model::spenat::SpenatDeclModel(this);
}

MbtSpenatModel::~MbtSpenatModel()
{
  delete this->_d;
}

model::spenat::SpenatDeclModel const* MbtSpenatModel::specModel() const
{
  return this->_d->_spenatDeclModel;
}

bool MbtSpenatModel::rebuildSpecModel(QString* errorMessage)
{
  Q_ASSERT(this->_d->_spenatDeclModel);

  this->clearErrors();
  this->_d->_spenatDeclModel->reset();
  this->_d->_tcgSpenatModelJson.clear();

  QFile spenatFile(this->mbtSpecModelFilePath());
  if(!spenatFile.exists())
  {
    utils::AddPtrString(errorMessage)<<tr("spenat model file '%1' does not exist!")
                                       .arg(this->mbtSpecModelFilePath());
    return false;
  }

  if(!spenatFile.open(QIODevice::Text | QIODevice::ReadOnly))
  {
    utils::AddPtrString(errorMessage)<<tr("spenat model file '%1' could not be opened!")
                                       .arg(this->mbtSpecModelFilePath());
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
      this->_d->_tcgSpenatModelJson = QLatin1String(tcgJsonDocPtr->toJson());
    }
  }

  return result;
}

QString MbtSpenatModel::toJson() const
{
  return this->_d->_tcgSpenatModelJson;
}
