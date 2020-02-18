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

#include "ui_statemachineeditorgraphvizsettings.h"
#include "statemachineeditorconstants.hpp"

#include "graphlayout/layoutglobal.hpp"
#include "graphlayout/graphquickwidget.hpp"
#include "graphlayout/comcreatestatemachinegraph.hpp"
#include "graphlayout/layoutgraph.hpp"
#include "mobata/model/statemachine/statemachine.hpp"

#include <coreplugin/dialogs/ioptionspage.h>
#include <coreplugin/icore.h>

#include <QWidget>
#include <QFileDialog>
#include <QPointer>
#include <QUuid>
#include <QDebug>

namespace statemachineeditor {

class StateMachineEditorGraphvizSettingsWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit StateMachineEditorGraphvizSettingsWidget(QWidget *parent = 0);
    ~StateMachineEditorGraphvizSettingsWidget();

private:
    class Private;
    Private* _d;
};

class StateMachineEditorGraphvizSettingsPage
        : public Core::IOptionsPage
{
    Q_OBJECT

public:
    StateMachineEditorGraphvizSettingsPage();
    ~StateMachineEditorGraphvizSettingsPage();

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

    Ui::StateMachineEditorGraphvizSettings _ui;
    QString _searchKeywords;
    QPointer<StateMachineEditorGraphvizSettingsWidget> _widget;
    model::statemachine::StateMachineModel* _model = new model::statemachine::StateMachineModel();
    graphlayout::LayoutGraph* _graph = new graphlayout::LayoutGraph("Preview");
    graphlayout::GraphQuickWidget* _comQML = nullptr;
};

}// namespace statemachineeditor
