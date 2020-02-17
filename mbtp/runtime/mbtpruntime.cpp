#include "mbtpruntime.hpp"

#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QProcess>
#include <QEventLoop>

#include <coreplugin/icore.h>

#include "mbtpproject.hpp"

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class MbtpRuntime::Private
{
  friend class MbtpRuntime;
  MbtpProject* _mbtpProject;
};

MbtpRuntime::MbtpRuntime(MbtpProject* mbtpProject,
                                   QObject* parent)
  :  QObject(parent),
    _d(new Private)
{
  this->_d->_mbtpProject = mbtpProject;
}

MbtpProject* MbtpRuntime::project()
{
  return this->_d->_mbtpProject;
}

MbtpRuntime::~MbtpRuntime()
{
  delete this->_d;
}

bool MbtpRuntime::prepare(QString *errorMessage)
{
  Q_UNUSED(errorMessage);
  return true;
}

void MbtpRuntime::reset()
{
  return;
}

bool MbtpRuntime::start()
{
  qDebug()<<"MBTP Runtime started";

  QSettings* settings = Core::ICore::settings();
  settings->beginGroup("Mbtp");
  QString mbtp_path = qvariant_cast<QString>(settings->value(QLatin1String("MbtpPath")));
  settings->endGroup();

  QString project_path = this->_d->_mbtpProject->projectDirectory() + "/";

  QString attributs_file = "";
  QString spenatjson_file = "";
  QString testcasetrace_file = "";

  for(QString file : this->_d->_mbtpProject->files(ProjectExplorer::Project::AllFiles))
  {
    if(file.right(11) == ".attributes"){
      attributs_file = file;
    }else if(file.right(11) == ".spenatjson"){
      spenatjson_file = file;
    }else if(file.right(14) == ".testcasetrace"){
      testcasetrace_file = file;
    }
  }

  if(attributs_file == "" || spenatjson_file == "" || testcasetrace_file == ""){
    return false;
  }

//  qDebug()<<"Mbtp-Files:";
//  qDebug()<<"attributes:"<<attributs_file;
//  qDebug()<<"spenatjson:"<<spenatjson_file;
//  qDebug()<<"testcasetrace:"<<testcasetrace_file;

  QStringList arg_list;

  arg_list.append("-s");
  arg_list.append(spenatjson_file);
  arg_list.append("-t");
  arg_list.append(testcasetrace_file);
  arg_list.append("-p");
  arg_list.append(attributs_file);
  arg_list.append("-m");
  arg_list.append("transition");
  arg_list.append("-l");
  arg_list.append("complete");
  arg_list.append("-d");
  arg_list.append("euclidean");
  arg_list.append("-i");
  arg_list.append("true");
  arg_list.append(project_path);

  this->_timer = new QTimer();
  this->_timer->setSingleShot(true);
  this->_timer->setInterval(30000);

  this->_process = new QProcess();
  this->_process->setProgram(mbtp_path);
  this->_process->setArguments(arg_list);
  QObject::connect(this->_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                   this, &MbtpRuntime::emitFinished);
  QObject::connect(this->_timer, &QTimer::timeout,
                   this->_process, &QProcess::kill);
  QObject::connect(this->_timer, &QTimer::timeout,
                   this, &MbtpRuntime::emitError);
  QObject::connect(this->_timer, &QTimer::timeout,
                   this->_process, &QProcess::deleteLater);
  QObject::connect(this->_timer, &QTimer::timeout,
                   this->_timer, &QTimer::deleteLater);
  this->_timer->start();
  this->_process->start();

//  bool sta = process.waitForStarted();
//  if(!sta){
//    qDebug()<<"Failed to start";
//    process.kill();
//    emit this->finished(false);
//    return false;
//  }
//  bool fin = process.waitForFinished();
//  if(!fin){
//    qDebug()<<"Failed to finish";
//    process.kill();
//    emit this->finished(false);
//    return false;
//  }

////  int exco = QProcess::execute(mbtp_path, arg_list);



//  this->emitFinished(0);
  return true;
}

void MbtpRuntime::emitError()
{
  qDebug()<<"timeout error";
  emit this->finished(false);
}

void MbtpRuntime::emitFinished(int exitCode)
{
  if(exitCode != 0){
    emit this->finished(false);
  }

  QString project_path = this->_d->_mbtpProject->projectDirectory() + "/";

  QFile file (project_path + "prioritization_result.txt");
  file.open(QIODevice::ReadOnly);

  QImage image (project_path + "dendogram.png");

  this->_d->_mbtpProject->setResult(file.readAll(), image);
  file.close();

  emit this->finished(true);
}

bool MbtpRuntime::stop()
{
 return true;
}

} // namespace mbtp
