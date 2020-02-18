/*
 * This file is part of mbt_plugins.
 *
 * mbt_plugins is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mbt_plugins is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mbt_plugins.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "mbtproject_global.hpp"

#include <coreplugin/inavigationwidgetfactory.h>

#include <QStyledItemDelegate>
#include <QTreeView>

class QTemporaryFile;

namespace Core {
class IEditor;
}

namespace model{ namespace ts{
class TestCaseItem;
}}

namespace ProjectExplorer{
class Project;
}

namespace mbtproject{

class MbtProjectModel;

class MBTPROJECTSHARED_EXPORT MbtSpecModelView
    : public QTreeView
{
  Q_OBJECT

public:
  MbtSpecModelView();
  virtual ~MbtSpecModelView();

private slots:
  void updateCurrentItem(Core::IEditor*);
  void contextMenuRequested(QPoint pos);
  void currentProjectChanged(ProjectExplorer::Project* currentProject);

private:
  bool                    eventFilter(QObject *obj, QEvent *event);
  void                    activateEditor(const QModelIndex &index);
  MbtProjectModel const*  mbtProjectModel() const;

protected:
  virtual void currentChanged(const QModelIndex& current,
                              const QModelIndex& previous);

private:
  Q_DISABLE_COPY(MbtSpecModelView)
  class Private;
  Private* _d;
};

class MbtSpecModelViewFactory : public Core::INavigationWidgetFactory
{
  Q_OBJECT
public:
  MbtSpecModelViewFactory();
  ~MbtSpecModelViewFactory();

public:
  QString displayName() const;
  int priority() const;
  Core::Id id() const;
  QKeySequence activationSequence() const;
  Core::NavigationView createWidget();
};

} // namespace mbtproject
