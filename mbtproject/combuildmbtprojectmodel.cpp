#include "combuildmbtprojectmodel.hpp"

#include "mbtproject.hpp"
#include "mbtprojectmodel.hpp"
#include "mbtprojectconfiguration.hpp"
#include "mbtspecmodelsettings.hpp"
#include "mbtspenatmodel.hpp"
#include "mbtstatemachinemodel.hpp"

#include <mobata/utils/functors.hpp>

namespace mbtproject {

class ComBuildMbtProjectModel::Private
{
  friend class ComBuildMbtProjectModel;

  MbtProject const* _mbtProject;
  MbtProjectModel*  _mbtProjectModel;

  Private(MbtProject const* mbtProject)
    : _mbtProject(mbtProject),
      _mbtProjectModel(0)
  {}
};

ComBuildMbtProjectModel::ComBuildMbtProjectModel(MbtProject const* mbtProject,
                                                 QObject *parent)
  : QObject(parent),
    _d(new Private(mbtProject))
{
  Q_ASSERT(this->_d->_mbtProject);
  this->_d->_mbtProjectModel=const_cast<MbtProjectModel*>(this->_d->_mbtProject->mbtProjectModel());
  Q_ASSERT(this->_d->_mbtProjectModel);
}

ComBuildMbtProjectModel::~ComBuildMbtProjectModel()
{
  delete this->_d;
}

bool ComBuildMbtProjectModel::execute(QString* errorMessage)
{
  Q_ASSERT(this->_d->_mbtProject);
  Q_ASSERT(this->_d->_mbtProjectModel);

  this->_d->_mbtProjectModel->reset();

  for(const QString& specModelFile : this->_d->_mbtProject->specModelFiles())
  {
    QFileInfo specModelFileInfo(specModelFile);
    if(!specModelFileInfo.exists())
    {
      utils::AddPtrString(errorMessage)<<tr("specification model file '%1' does not exist!")
                                         .arg(specModelFile);
      return false;
    }

    if(specModelFileInfo.suffix() == QLatin1String("spenat"))
      this->_d->_mbtProjectModel->addMbtSpecModel(new MbtSpenatModel(specModelFile));
    else if(specModelFileInfo.suffix() == QLatin1String("statemachine"))
      this->_d->_mbtProjectModel->addMbtSpecModel(new MbtStateMachineModel(specModelFile));
    else
    {
      utils::AddPtrString(errorMessage)<<tr("type '%1' of specification model file '%2' is currently not supported!")
                                         .arg(specModelFileInfo.suffix()).arg(specModelFile);
      return false;
    }
  }

  return true;
}

} // namespace mbtproject
