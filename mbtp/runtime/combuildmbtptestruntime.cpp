#include "combuildmbtptestruntime.hpp"

#include "mbtpproject.hpp"
#include "mbtpruntime.hpp"

#include <QHostAddress>



#include <mobata/memory_leak_start.hpp>

namespace mbtp {

class ComBuildMbtpTestRuntime::Private
{
  friend class ComBuildMbtpTestRuntime;

  MbtpProject*     _mbtpProject;
  MbtpTestRuntime* _mbtpTestRuntime;
  QDir              _buildDir;

  Private(MbtpProject* mbtpProject,
          MbtpTestRuntime* mbtpTestRuntime,
          const QDir& buildDir)
    : _mbtpProject(mbtpProject),
      _mbtpTestRuntime(mbtpTestRuntime),
      _buildDir(buildDir)
  {}

private:
  bool buildTestComponents(QString* errorString);
  bool buildTestCases(QString* errorString);
};


bool ComBuildMbtpTestRuntime::Private::buildTestComponents(QString* errorString)
{
  Q_UNUSED(errorString);
  return true;
}

bool ComBuildMbtpTestRuntime::Private::buildTestCases(QString* errorString)
{
  Q_UNUSED(errorString);
  return true;
}

/////////////ComBuildMbtpTestRuntime///////////////////////
ComBuildMbtpTestRuntime::ComBuildMbtpTestRuntime(MbtpProject* mbtpProject,
                                                 MbtpTestRuntime* mbtpTestRuntime,
                                                 const QDir& buildDir,
                                                 QObject* parent)
  : QObject(parent),
    _d(new Private(mbtpProject, mbtpTestRuntime, buildDir))
{
  Q_ASSERT(this->_d->_mbtpProject);
  Q_ASSERT(this->_d->_mbtpTestRuntime);
}

ComBuildMbtpTestRuntime::~ComBuildMbtpTestRuntime()
{
  delete this->_d;
}

bool ComBuildMbtpTestRuntime::execute(QString* errorString)
{
  Q_UNUSED(errorString);
  return true;
}

} // namespace mbtp
