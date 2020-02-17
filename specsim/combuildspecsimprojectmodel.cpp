#include "combuildspecsimprojectmodel.hpp"

#include "specsimproject.hpp"
#include "specsimprojectmodel.hpp"
#include "specsimprojectconfiguration.hpp"
#include "specsimmodelsettings.hpp"
#include "specsimmodel.hpp"

#include <mobata/utils/functors.hpp>

namespace specsim {

class ComBuildSpecSimProjectModel::Private
{
  friend class ComBuildSpecSimProjectModel;

  SpecSimProject const* _specSimProject;
  SpecSimProjectModel*  _specSimProjectModel;

  Private(SpecSimProject const* specSimProject)
    : _specSimProject(specSimProject),
      _specSimProjectModel(nullptr)
  {}
};

ComBuildSpecSimProjectModel::ComBuildSpecSimProjectModel(SpecSimProject const* specSimProject,
                                                         QObject *parent)
  : QObject(parent),
    _d(new Private(specSimProject))
{
  Q_ASSERT(this->_d->_specSimProject);
  this->_d->_specSimProjectModel=const_cast<SpecSimProjectModel*>(this->_d->_specSimProject->specSimProjectModel());
  Q_ASSERT(this->_d->_specSimProjectModel);
}

ComBuildSpecSimProjectModel::~ComBuildSpecSimProjectModel()
{
  delete this->_d;
}

bool ComBuildSpecSimProjectModel::execute(QString* errorMessage)
{
  Q_ASSERT(this->_d->_specSimProject);
  Q_ASSERT(this->_d->_specSimProjectModel);

  this->_d->_specSimProjectModel->reset();

  for(const QString& specSimModelFile : this->_d->_specSimProject->specSimModelFiles())
  {
    QFileInfo specModelFileInfo(specSimModelFile);
    if(!specModelFileInfo.exists())
    {
      utils::AddPtrString(errorMessage)<<tr("simulation model file '%1' does not exist!")
                                         .arg(specSimModelFile);
      return false;
    }

    if(specModelFileInfo.suffix() == QLatin1String("spenat"))
      this->_d->_specSimProjectModel->addSpecSimModel(new SpecSimSpenatModel(specSimModelFile));
    else if(specModelFileInfo.suffix() == QLatin1String("statemachine"))
      this->_d->_specSimProjectModel->addSpecSimModel(new SpecSimStateMachineModel(specSimModelFile));
    else
    {
      utils::AddPtrString(errorMessage)<<tr("type '%1' of specification model file '%2' is currently not supported!")
                                         .arg(specModelFileInfo.suffix()).arg(specSimModelFile);
      return false;
    }
  }

  return true;
}

} // namespace specsim
