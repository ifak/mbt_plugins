#include "comexporttestsuite.hpp"

namespace mbtproject {

class ComExportTestSuite::Private
{
  friend class ComExportTestSuite;

  QString _title;
  QIcon   _icon;

  Private(const QString& title,
          const QIcon& icon)
    : _title(title),
      _icon(icon)
  {}
};

ComExportTestSuite::ComExportTestSuite(const QString& title,
                                       const QIcon& icon,
                                       QObject* parent)
  : QObject(parent), _d(new Private(title, icon))
{}

ComExportTestSuite::~ComExportTestSuite()
{
  delete this->_d;
}

const QString& ComExportTestSuite::title() const
{
  return this->_d->_title;
}

void ComExportTestSuite::setTitle(const QString &title)
{
  this->_d->_title=title;

  return;
}

const QIcon& ComExportTestSuite::icon() const
{
  return this->_d->_icon;
}

void ComExportTestSuite::setIcon(const QIcon& icon)
{
  this->_d->_icon=icon;

  return;
}

} // namespace mbtproject
