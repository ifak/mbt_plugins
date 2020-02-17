#include "mbtpproject.hpp"

#include "mbtpconstants.hpp"
#include "mbtprunconfiguration.hpp"

#include <utils/qtcassert.h>

#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/target.h>

#include <mobata/memory_leak_start.hpp>

#include <QDebug>

namespace mbtp {

class MbtpProject::Private
{
  friend class MbtpProject;

  ProjectExplorer::FolderNode*      _spenatjsonFolder;
  ProjectExplorer::FolderNode*      _testcasetraceFolder;
  ProjectExplorer::FolderNode*      _attributesFolder;

  QString _resultJson;
  QImage _resultImage;

  Private()
    : _spenatjsonFolder(nullptr),
      _testcasetraceFolder(nullptr),
      _attributesFolder(nullptr)
  {}
};


MbtpProject::MbtpProject(MbtpProjectManager *projectManager,
                             const QString &fileName)
  :  ProjectExplorer::BaseProject(projectManager,
                                  fileName,
                                  QLatin1String(Constants::MBTP_PROJECT_MIME_TYPE),
                                  QLatin1String(Constants::MBTP_PROJECT)),
     _d(new Private)
 {

// SpenatJson Folder
  this->_d->_spenatjsonFolder=this->addFolderNode(QLatin1String("spenatjson"),
                                            QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                            true);
  Q_ASSERT(this->_d->_spenatjsonFolder);
  this->_d->_spenatjsonFolder->setProperty(Constants::MBTP_PROJECTNODE_TYPE,
                                     QLatin1String(Constants::MBTP_PROJECTNODE_MODEL));
  this->addArrayFilesKey(QLatin1String("SpenatJson"), this->_d->_spenatjsonFolder);

  // Trace Folder
  this->_d->_testcasetraceFolder=this->addFolderNode(QLatin1String("trace"),
                                            QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                            true);
  Q_ASSERT(this->_d->_testcasetraceFolder);
  this->_d->_testcasetraceFolder->setProperty(Constants::MBTP_PROJECTNODE_TYPE,
                                     QLatin1String(Constants::MBTP_PROJECTNODE_TRACE));
  this->addArrayFilesKey(QLatin1String("TestcaseTrace"), this->_d->_testcasetraceFolder);

  // Attributes Folder
  this->_d->_attributesFolder=this->addFolderNode(QLatin1String("attributes"),
                                            QIcon(QLatin1String(":/mobata/images/base/manytype.png")),
                                            true);
  Q_ASSERT(this->_d->_attributesFolder);
  this->_d->_attributesFolder->setProperty(Constants::MBTP_PROJECTNODE_TYPE,
                                     QLatin1String(Constants::MBTP_PROJECTNODE_ATTRIBUTES));
  this->addArrayFilesKey(QLatin1String("Attributes"), this->_d->_attributesFolder);


}

MbtpProject::~MbtpProject()
{
  delete this->_d;
}

QImage MbtpProject::resultImage()
{
  return this->_d->_resultImage;
}

QString MbtpProject::resultString()
{
  return this->_d->_resultJson;
}

void MbtpProject::setResult(QString result, QImage image)
{
  this->_d->_resultJson = result;
  this->_d->_resultImage = image;
}

bool MbtpProject::supportsKit(ProjectExplorer::Kit *kit,
                                QString *errorMessage) const
{
  if(!kit)
    return false;

  if(kit->id()==Core::Id(Constants::MBTP_KIT)){
    return true;
  }

  QTC_ASSERT(errorMessage, return false);

  *errorMessage+=tr("kit '%1' is not supported!").arg(kit->displayName());
  qDebug()<<tr("kit '%1' is not supported!").arg(kit->displayName());

  return false;
}

bool MbtpProject::needsConfiguration() const
{
  return false;
}

bool MbtpProject::supportsNoTargetPanel() const
{
  return false;
}

QVariantMap *MbtpProject::settings()
{
  return &this->_settingsMap;
}

bool MbtpProject::fromMap(const QVariantMap& map)
{
  Q_UNUSED(map)
  qDebug()<<"YY"<<"fromMap";

  ProjectExplorer::KitManager* kitManager=ProjectExplorer::KitManager::instance();
  Q_ASSERT(kitManager);

  ProjectExplorer::Kit* mbtpKit=nullptr;
  for(ProjectExplorer::Kit* kit : kitManager->kits())
  {
    if(kit->id()==Core::Id(Constants::MBTP_KIT))
    {
      mbtpKit=kit;
      break;
    }
  }

  ProjectExplorer::Target* activeTarget=this->activeTarget();
  if (!activeTarget)
  {
    if(!mbtpKit)
      return false;

    ProjectExplorer::Target* mbtpTarget=this->createTarget(mbtpKit);
    Q_ASSERT(mbtpTarget);
    mbtpTarget->setDisplayName(QLatin1String("MBTP target"));
    this->addTarget(mbtpTarget);
    this->setActiveTarget(mbtpTarget);
  }

  // Sanity check: We need both a buildconfiguration and a runconfiguration!
  QList<ProjectExplorer::Target *> targetList = this->targets();
  for (ProjectExplorer::Target *t : targetList)
  {
    if (!t->activeBuildConfiguration())
    {
      this->removeTarget(t);
      continue;
    }

    if (!t->activeRunConfiguration())
    {
      MbtpRunConfiguration *rc = new MbtpRunConfiguration(t);
      t->addRunConfiguration(rc);
      t->setActiveRunConfiguration(rc);
    }
  }


  return true;
}

QVariantMap MbtpProject::toMap() const
{
    QVariantMap map = BaseProject::toMap();

    return map;
}

MbtpProjectManager::MbtpProjectManager()
  : ProjectExplorer::BaseProjectManager(QLatin1String(Constants::MBTP_PROJECT_MIME_TYPE))
{}

MbtpProjectManager::~MbtpProjectManager()
{}

MbtpProject*
MbtpProjectManager::createProject(const QString &fileName)
{
  return new MbtpProject(this, fileName);
}

} // namespace mbtp
