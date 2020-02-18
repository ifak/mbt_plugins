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

#include "mobata/model/spenat/spenatdeclmodel.hpp"
#include "mobata/model/spenat/transitionitem.hpp"
#include "mobata/model/spenat/placeitem.hpp"
#include "graphlayout/comcreatedeclgraph.hpp"
#include "graphlayout/graphquickwidget.hpp"
#include "graphlayout/qmlgraphview.hpp"
#include <coreplugin/icore.h>

inline void previewGraph(model::spenat::SpenatDeclModel* model){

    model->addPlace("init");
    model->addInitPlace(model->places().first());
    model->addPlace("place1");
    model->addPlace("place2");
    model::spenat::TransitionItem* trans1 = new model::spenat::TransitionItem(QStringLiteral("trans1"), 0);
    model::spenat::TransitionItem* trans2 = new model::spenat::TransitionItem(QStringLiteral("trans2"), 0);
    model->addTransition(trans1);
    model->addTransition(trans2);

    trans1->addPrePlaceArc(model->places()[0]);
    trans1->addPostPlaceArc(model->places()[1]);

    trans2->addPrePlaceArc(model->places()[0]);
    trans2->addPostPlaceArc(model->places()[1]);
    trans2->addPostPlaceArc(model->places()[2]);
}

inline void spenatGraphLoadSettings(graphlayout::ComCreateDeclGraph* comCreateDeclGraph,
                                    graphlayout::GraphQuickWidget* graphQuickWidget)
{
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    //place
    settings->beginGroup("Place");
    if(qvariant_cast<QColor>(settings->value("InitPlaceColor"))==""){
        settings->endGroup();
        settings->endGroup();
        return;
    }
    comCreateDeclGraph->setInitPlaceColor(qvariant_cast<QColor>(settings->value("InitPlaceColor")));

    comCreateDeclGraph->setPlaceShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value("PlaceShape"))));
    comCreateDeclGraph->setPlaceColor(qvariant_cast<QColor>(settings->value("PlaceColor")));
    comCreateDeclGraph->setPlaceBorderColor(qvariant_cast<QColor>(settings->value("PlaceBorderColor")));
    comCreateDeclGraph->setPlaceBorderWidth(qvariant_cast<double>(settings->value("PlaceBorderWidth")));

    comCreateDeclGraph->setPlaceLabelSize(qvariant_cast<int>(settings->value("LabelSize")));
    comCreateDeclGraph->setPlaceLabelColor(qvariant_cast<QColor>(settings->value("LabelColor")));
    settings->endGroup();
    //transition
    settings->beginGroup("Transition");
    comCreateDeclGraph->setTransitionShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value("TransitionShape"))));
    comCreateDeclGraph->setTransitionColor(qvariant_cast<QColor>(settings->value("TransitionColor")));
    comCreateDeclGraph->setTransitionBorderWidth(qvariant_cast<double>(settings->value("TransitionBorderWidth")));
    comCreateDeclGraph->setTransitionBorderColor(qvariant_cast<QColor>(settings->value("TransitionBorderColor")));
    //SimpleTransition
    comCreateDeclGraph->setTransitionSimple(qvariant_cast<bool>(settings->value("TransitionSimple")));
    comCreateDeclGraph->setTransitionSimpleColor(qvariant_cast<QColor>(settings->value("TransitionSimpleColor")));
    comCreateDeclGraph->setTransitionSimpleArrowtype(graphlayout::ArrowEnumFromString(qvariant_cast<QString>(settings->value("TransitionSimpleArrowtype"))));
    comCreateDeclGraph->setTransitionSimpleArrowsize(qvariant_cast<int>(settings->value("TransitionSimpleArrowsize")));
    comCreateDeclGraph->setTransitionSimpleLinewidth(qvariant_cast<double>(settings->value("TransitionSimpleLineWidth")));
    //TransitionLabel
    comCreateDeclGraph->setTransitionLabelSize(qvariant_cast<int>(settings->value("LabelSize")));
    comCreateDeclGraph->setTransitionLabelColor(qvariant_cast<QColor>(settings->value("LabelColor")));
    comCreateDeclGraph->setTransitionGuardLabelSize(qvariant_cast<int>(settings->value("GuardLabelSize")));
    comCreateDeclGraph->setTransitionActionIndention(qvariant_cast<int>(settings->value("ActionIndention")));
    graphQuickWidget->setZoomBound(qvariant_cast<double>(settings->value("LabelZoomlevel")));
    settings->endGroup();
    //arc
    settings->beginGroup("Arc");
    comCreateDeclGraph->setArcColor(qvariant_cast<QColor>(settings->value("ArcColor")));
    comCreateDeclGraph->setArcArrowsize(qvariant_cast<int>(settings->value("ArcArrowsize")));
    comCreateDeclGraph->setArcLineWidth(qvariant_cast<double>(settings->value("ArcLineWidth")));
    comCreateDeclGraph->setArcShape(graphlayout::ArrowEnumFromString(qvariant_cast<QString>(settings->value("ArcArrowtype"))));
    settings->endGroup();
    //background
    settings->beginGroup("Background");
    graphQuickWidget->setBackgroundColor(qvariant_cast<QColor>(settings->value("BackgroundColor")));
    graphQuickWidget->setTitleColor(qvariant_cast<QColor>(settings->value("TitleColor")));
    graphQuickWidget->setTitleSize(qvariant_cast<int>(settings->value("TitleSize")));
    graphQuickWidget->setTitleShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("TitleShape")))));
    graphQuickWidget->setTitleBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBorderColor"))));
    graphQuickWidget->setTitleBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("TitleBorderWidth"))));
    graphQuickWidget->setTitleBackgroundColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBackgroundColor"))));
    settings->endGroup();
    //Highlight
    settings->beginGroup(QLatin1String("Highlight"));
    comCreateDeclGraph->setHighlightPlaceColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateColor"))));
    comCreateDeclGraph->setHighlightPlaceBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateBorderColor"))));
    comCreateDeclGraph->setHighlightPlaceBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("StateBorderWidth"))));
    comCreateDeclGraph->setHighlightTransitionColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))));
    comCreateDeclGraph->setHighlightTransitionBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionBorderColor"))));
    comCreateDeclGraph->setHighlightTransitionBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("TransitionBorderWidth"))));
    settings->endGroup();
    //Graphviz
    settings->beginGroup("Graphviz");
    comCreateDeclGraph->setSplines(graphlayout::SplineEnumFromString(qvariant_cast<QString>(settings->value("Spline"))));
    comCreateDeclGraph->setOverlap(graphlayout::OverlapEnumFromString(qvariant_cast<QString>(settings->value("Overlap"))));
    graphQuickWidget->setAlgorithm(graphlayout::GraphvizAlgorithmFromString(qvariant_cast<QString>(settings->value("Algorithm"))));
    graphQuickWidget->setZoomType(qvariant_cast<QString>(settings->value("Zoom")));
    settings->endGroup();
    //Labeltext
    settings->beginGroup("Labeltext");
    comCreateDeclGraph->setShowLabelName(qvariant_cast<bool>(settings->value("ShowName")));
    comCreateDeclGraph->setShowLabelGuard(qvariant_cast<bool>(settings->value("ShowGuard")));
    comCreateDeclGraph->setShowLabelAction(qvariant_cast<bool>(settings->value("ShowAction")));
    //comCreateDeclGraph->setStaticEdgeLabels(!qvariant_cast<bool>(settings->value("GraphvizEdgeLabel")));
    //comCreateDeclGraph->setStaticNodeLabels(!qvariant_cast<bool>(settings->value("GraphvizNodeLabel")));
    settings->endGroup();
    settings->endGroup();

    settings->beginGroup(QLatin1String("Graphviz"));
    graphQuickWidget->setGraphvizPath(qvariant_cast<QString>(settings->value(QLatin1String("GraphvizPath"))));
    settings->endGroup();

}
