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

#include <QSettings>
#include <mobatawidgets/graph/types.hpp>

namespace mbtproject {

class MBTPROJECTSHARED_EXPORT MbtSpenatSettings
{
public:
    MbtSpenatSettings();
    MbtSpenatSettings(const MbtSpenatSettings& source);

public:
    void toSettings(const QString &category, QSettings *settings) const;
    void fromSettings(const QString &category, const QSettings *settings);

    view::graph::NodeOptions compOptionsFromSettings() const;
    view::graph::NodeOptions portOptionsFromSettings() const;
    view::graph::ArcOptions linkOptionsFromSettings() const;

    const QString& compTextFontSeries() const;
    void setCompTextFontSeries(const QString &fontName);
    const int& compTextFontSize() const;
    void setCompTextFontSize(const int fontSize);
    const bool& compTextFontBold() const;
    void setCompTextFontBold(const bool isBold);
    const bool& compTextFontItalic() const;
    void setCompTextFontItalic(const bool isItalic);
    const QString& compTextColorName() const;
    void setCompTextColorName(const QString &colorName);
    const qreal& compBoxWidth() const;
    void setCompBoxWidth(const qreal width);
    const qreal& compBoxHeight() const;
    void setCompBoxHeight(const qreal height);
    const QString& compBoxColorName() const;
    void setCompBoxColorName(const QString &colorName);
    const int& compBorderWidth() const;
    void setCompBorderWidth(const int width);
    const QString& compBorderColorName() const;
    void setCompBorderColorName(const QString &colorName);

    const qreal& portBoxWidth() const;
    void setPortBoxWidth(const qreal width);
    const qreal& portBoxHeight() const;
    void setPortBoxHeight(const qreal height);
    const QString& portBoxColorName() const;
    void setPortBoxColorName(const QString &colorName);
    const int& portBorderWidth() const;
    void setPortBorderWidth(const int width);
    const QString& portBorderColorName() const;
    void setPortBorderColorName(const QString &colorName);

    const int& linkLineWidth() const;
    void setLinkLineWidth(const int width);
    const QString& linkLineColorName() const;
    void setLinkLineColorName(const QString &colorName);

    const QString& sutBoxColorName() const;
    void setSutBoxColorName(const QString &colorName);
    const qreal& sutBoxCompMargin() const;
    void setSutBoxCompMargin(const qreal margin);
    const qreal& sutBoxMargin() const;
    void setSutBoxMargin(const qreal margin);
    const qreal& sutBoxRadius() const;
    void setSutBoxRadius(const qreal radius);

private:
    QString _compTextFontSeries;
    int     _compTextFontSize;
    bool    _compTextFontBold;
    bool    _compTextFontItalic;
    QString _compTextColorName;
    qreal   _compBoxWidth;
    qreal   _compBoxHeight;
    QString _compBoxColorName;
    int     _compBorderWidth;
    QString _compBorderColorName;
    qreal   _portBoxWidth;
    qreal   _portBoxHeight;
    QString _portBoxColorName;
    int     _portBorderWidth;
    QString _portBorderColorName;
    int     _linkLineWidth;
    QString _linkLineColorName;
    QString _sutBoxColorName;
    qreal   _sutBoxCompMargin;
    qreal   _sutBoxMargin;
    qreal   _sutBoxRadius;
};

inline bool operator==(const MbtSpenatSettings &p1, const MbtSpenatSettings &p2)
{
    return (!p1.compTextFontSeries().compare(p2.compTextFontSeries()))
            && (p1.compTextFontSize() == p2.compTextFontSize())
            && (p1.compTextFontBold() == p2.compTextFontBold())
            && (p1.compTextFontItalic() == p2.compTextFontItalic())
            && (!p1.compTextColorName().compare(p2.compTextColorName()))
            && (p1.compBoxWidth() == p2.compBoxWidth())
            && (p1.compBoxHeight() == p2.compBoxHeight())
            && (!p1.compBoxColorName().compare(p2.compBoxColorName()))
            && (p1.compBorderWidth() == p2.compBorderWidth())
            && (!p1.compBorderColorName().compare(p2.compBorderColorName()))
            && (p1.portBoxWidth() == p2.portBoxWidth())
            && (p1.portBoxHeight() == p2.portBoxHeight())
            && (!p1.portBoxColorName().compare(p2.portBoxColorName()))
            && (p1.portBorderWidth() == p2.portBorderWidth())
            && (!p1.portBorderColorName().compare(p2.portBorderColorName()))
            && (p1.linkLineWidth() == p2.linkLineWidth())
            && (!p1.linkLineColorName().compare(p2.linkLineColorName()))
            && (!p1.sutBoxColorName().compare(p2.sutBoxColorName()))
            && (p1.sutBoxCompMargin() == p2.sutBoxCompMargin())
            && (p1.sutBoxMargin() == p2.sutBoxMargin())
            && (p1.sutBoxRadius() == p2.sutBoxRadius());
}

}
