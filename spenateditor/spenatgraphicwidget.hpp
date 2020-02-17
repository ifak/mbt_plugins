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

#include "spenateditor_global.hpp"

#include <graphlayout/graphquickwidget.hpp>
#include <graphlayout/comexportgraph.hpp>
#include <QUuid>

class QToolBar;

namespace spenateditor {

class SpenatGraphicEditor;

class SPENATEDITOR_EXPORT SpenatGraphicWidget
    : public graphlayout::GraphQuickWidget
{
  Q_OBJECT

public:
  SpenatGraphicWidget(QWidget *parent = 0);
  virtual ~SpenatGraphicWidget();

public slots:
  void updateSpenatGraphic();

public:
  QToolBar*             toolBar();
  SpenatGraphicEditor*  spenatGraphicEditor() const;

private:
  void createToolBar();

private:
  Q_DISABLE_COPY(SpenatGraphicWidget)
  class Private;
  Private*  _d;
};

} // namespace spenateditor
