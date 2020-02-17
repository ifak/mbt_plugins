/*
 * This file is part of mobata.
 *
 * mobata is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mobata is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mobata.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <coreplugin/dialogs/ioptionspage.h>

#include <QWidget>
#include <QPointer>
#include <QUuid>

namespace irdleditor {

namespace Internal {

class IrdlEditorSettingsWidget
    : public QWidget
{
  Q_OBJECT

public:
  explicit IrdlEditorSettingsWidget(QWidget *parent = 0);
  ~IrdlEditorSettingsWidget();

private:
  class Private;
  Private* _d;
};

class IrdlEditorSettingsPage
    : public Core::IOptionsPage
{
  Q_OBJECT

public:
  IrdlEditorSettingsPage();
  ~IrdlEditorSettingsPage();

public:
  QWidget*  createPage(QWidget *parent);
  void apply();
  void finish();
  bool matches(const QString& s) const;

private:
  QString                               _searchKeywords;
  QPointer<IrdlEditorSettingsWidget>  _widget;
};

}// namespace Internal
}// namespace irdleditor
