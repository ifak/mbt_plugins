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

#include "ui_spenateditorbackgroundsettings.h"
#include "spenateditorconstants.hpp"

#include "graphlayout/layoutglobal.hpp"
#include "graphlayout/graphquickwidget.hpp"
#include "graphlayout/comcreatedeclgraph.hpp"
#include "graphlayout/layoutgraph.hpp"
#include "mobata/model/spenat/spenatdeclmodel.hpp"

#include <coreplugin/dialogs/ioptionspage.h>
#include <coreplugin/icore.h>

#include <QWidget>
#include <QFileDialog>
#include <QPointer>
#include <QUuid>
#include <QDebug>

namespace spenateditor {

namespace Internal {

class SpenatEditorBackgroundSettingsWidget
        : public QWidget
{
    Q_OBJECT

public:
    explicit SpenatEditorBackgroundSettingsWidget(QWidget *parent = 0);
    ~SpenatEditorBackgroundSettingsWidget();

private:
    class Private;
    Private* _d;
};

class SpenatEditorBackgroundSettingsPage
        : public Core::IOptionsPage
{
    Q_OBJECT

public:
    SpenatEditorBackgroundSettingsPage();
    ~SpenatEditorBackgroundSettingsPage();

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

    Ui::SpenatEditorBackgroundSettings _ui;
    QString _searchKeywords;
    QPointer<SpenatEditorBackgroundSettingsWidget> _widget;
    model::spenat::SpenatDeclModel* _model = new model::spenat::SpenatDeclModel("Preview");
    graphlayout::LayoutGraph* _graph = new graphlayout::LayoutGraph("Preview");
    graphlayout::GraphQuickWidget* _comQML = nullptr;
};

}// namespace Internal
}// namespace spenateditor
