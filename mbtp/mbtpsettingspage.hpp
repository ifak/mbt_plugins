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

#include "ui_mbtpsettings.h"
#include "mbtpconstants.hpp"

#include <coreplugin/dialogs/ioptionspage.h>
#include <coreplugin/icore.h>

#include <QWidget>
#include <QFileDialog>
#include <QPointer>
#include <QUuid>
#include <QDebug>

namespace mbtp {

class MbtpSettingsWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit MbtpSettingsWidget(QWidget *parent = nullptr);
    ~MbtpSettingsWidget();

private:
    class Private;
    Private* _d;
};

class MbtpSettingsPage
        : public Core::IOptionsPage
{
    Q_OBJECT

public:
    MbtpSettingsPage();
    ~MbtpSettingsPage();

public:
    QWidget*  createPage(QWidget *parent);
    void apply();
    void finish();
    bool matches(const QString& s) const;
signals:
    void appliedSettings();

private slots:
    void browseMbtpPath();

private:
    Ui::MbtpSettings _ui;
    QString                               _searchKeywords;
    QPointer<MbtpSettingsWidget>  _widget;
};

}// namespace mbtp
