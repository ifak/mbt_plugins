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

#include "ui_statemachineeditorbackgroundsettings.h"
#include "statemachineeditorconstants.hpp"

#include "graphlayout/layoutglobal.hpp"
#include "graphlayout/graphquickwidget.hpp"
#include "graphlayout/comcreatestatemachinegraph.hpp"
#include "graphlayout/layoutgraph.hpp"
#include "mobata/model/statemachine/statemachinemodel.hpp"

#include <coreplugin/dialogs/ioptionspage.h>
#include <coreplugin/icore.h>

#include <QWidget>
#include <QFileDialog>
#include <QPointer>
#include <QUuid>
#include <QDebug>

namespace statemachineeditor {

class StateMachineEditorBackgroundSettingsWidget
        : public QWidget
{
    Q_OBJECT

public:
    explicit StateMachineEditorBackgroundSettingsWidget(QWidget *parent = 0);
    ~StateMachineEditorBackgroundSettingsWidget();

private:
    class Private;
    Private* _d;
};

class StateMachineEditorBackgroundSettingsPage
        : public Core::IOptionsPage
{
    Q_OBJECT

public:
    StateMachineEditorBackgroundSettingsPage();
    ~StateMachineEditorBackgroundSettingsPage();

public slots:
    void refreshPreview(){
        _refreshPreview();
    }

public:
    QWidget*  createPage(QWidget *parent);
    void apply();
    void finish();
    bool matches(const QString& s) const;
signals:
    void appliedSettings();
private:
    void _refreshPreview();
    void _initGraph();

    Ui::StateMachineEditorBackgroundSettings _ui;
    QString _searchKeywords;
    QPointer<StateMachineEditorBackgroundSettingsWidget> _widget;
    model::statemachine::StateMachineModel* _model = new model::statemachine::StateMachineModel();
    graphlayout::LayoutGraph* _graph = new graphlayout::LayoutGraph("Preview");
    graphlayout::GraphQuickWidget* _comQML = nullptr;
};

}// namespace statemachineeditor
