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

#include "statemachineeditor_global.hpp"

#include <dsleditor/dslgraphiceditor.hpp>

#include "statemachinegraphicwidget.hpp"

#include <mobata/model/statemachine/statemachinemodel.hpp>

namespace statemachineeditor {

class StateMachineDocument;
class StateMachineEditor;

using model::statemachine::StateMachineModelPtr;

class STATEMACHINEEDITOR_EXPORT StateMachineGraphicEditor
    : public dsleditor::DslGraphicEditor
{
  Q_OBJECT

public:
  StateMachineGraphicEditor(StateMachineGraphicWidget* stateMachineGraphicWidget);
  virtual ~StateMachineGraphicEditor();

public:
  virtual bool              open(QString *errorString,
                                 const QString &fileName,
                                 const QString &realFileName) override;
  virtual Core::IDocument*  document() override;
  virtual QWidget*          toolBar() override;

public:
  void                        setStateMachineModel(const StateMachineModelPtr& stateMachineModel);
  const StateMachineModelPtr& stateMachineModel() const;
  void                        setStateMachineDocument(StateMachineDocument const* stateMachineDocument);
  StateMachineDocument const* stateMachineDocument() const;

public:
  StateMachineGraphicWidget*  stateMachineGraphicWidget();
  QString                     fileName() const;

private:
  Q_DISABLE_COPY(StateMachineGraphicEditor)
  class Private;
  Private* _d;
};

} // namespace statemachineeditor
