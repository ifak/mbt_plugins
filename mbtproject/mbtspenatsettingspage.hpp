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
#ifndef DISTESTPROJECT_DISTESTSUTSETTINGSPAGE_HPP
#define DISTESTPROJECT_DISTESTSUTSETTINGSPAGE_HPP

#include <coreplugin/dialogs/ioptionspage.h>

#include "ui_sutsettingspage.h"

#include <QPointer>
#include <QUuid>

namespace distestproject {

class DisTestSutSettings;

namespace Internal {

class DisTestSutSettings;

class DisTestSutSettingsWidget : public QWidget {
    Q_OBJECT

public:
    explicit DisTestSutSettingsWidget(QWidget *parent = 0);
    ~DisTestSutSettingsWidget();

public:
    void initializeConfigView();
    void saveSutSettings() const;
    distestproject::DisTestSutSettings* sutSettings() const;
    const QString& searchKeywords() const;

private:
    void buildTestModel();

private slots:
    void renderSut();
    void changeCompTextFontName(const QFont &font);
    void changeCompTextFontSize(int size);
    void changeCompTextFontBold(bool checked);
    void changeCompTextFontItalic(bool checked);
    void changeCompTextColor();
    void changeCompBoxWidth(int width);
    void changeCompBoxHeight(int height);
    void changeCompBoxColor();
    void changeCompBorderWidth(int width);
    void changeCompBorderColor();
    void changePortBoxWidth(int width);
    void changePortBoxHeight(int height);
    void changePortBoxColor();
    void changePortBorderWidth(int width);
    void changePortBorderColor();
    void changeLinkLineWidth(int width);
    void changeLinkLineColor();
    void changeSutBoxColor();
    void changeSutBoxCompMargin(int margin);
    void changeSutBoxMargin(int margin);
    void changeSutBoxRadius(int radius);

private:
    Ui::DisTestSutSettingsPageUi m_ui;
    class Private;
    Private* _d;
    mutable QString m_searchKeywords;
};

class DisTestSutSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    DisTestSutSettingsPage();
    ~DisTestSutSettingsPage();

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();
    bool matches(const QString &s) const;

private:
    QString m_searchKeywords;
    QPointer<DisTestSutSettingsWidget> m_widget;
};

}
}

#endif // DISTESTSUTSETTINGSPAGE_HPP
