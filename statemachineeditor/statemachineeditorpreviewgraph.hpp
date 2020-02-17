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

#include <mobata/model/base/signalitem.hpp>
#include <mobata/model/base/portitem.hpp>
#include <mobata/model/statemachine/statemachine.hpp>
#include <mobata/model/statemachine/statemachinemodel.hpp>
#include <mobata/model/statemachine/simplestateitem.hpp>
#include <mobata/model/statemachine/abstractstateitem.hpp>
#include <mobata/model/statemachine/transitionitem.hpp>
#include <mobata/model/statemachine/finalstateitem.hpp>
#include <mobata/model/statemachine/regionitem.hpp>
#include <mobata/model/statemachine/compositestateitem.hpp>
#include <mobata/model/statemachine/concurrentstateitem.hpp>
#include "graphlayout/comcreatestatemachinegraph.hpp"
#include "graphlayout/graphquickwidget.hpp"
#include "graphlayout/qmlgraphview.hpp"
#include <coreplugin/icore.h>

inline void previewGraph(model::statemachine::StateMachineModel* smModel){
  QString errorString;
  //Init-State
  model::statemachine::SimpleStateItem* init = new model::statemachine::SimpleStateItem("initState");
  smModel->setInitState(init, &errorString);
  Q_ASSERT(init);
  //Choice-State
  model::statemachine::ChoiceStateItem* choice = new model::statemachine::ChoiceStateItem("choiceState");
  smModel->addState(choice, &errorString);
  Q_ASSERT(choice);
  //Junction-State
  model::statemachine::JunctionStateItem* junction = new model::statemachine::JunctionStateItem("junctionState");
  smModel->addState(junction, &errorString);
  Q_ASSERT(junction);
  //Composite-State
  model::statemachine::CompositeStateItem* composite = new model::statemachine::CompositeStateItem("compositeState");
  smModel->addState(composite, &errorString);
  Q_ASSERT(composite);
  //Simple-State
  model::statemachine::SimpleStateItem* comState1 = new model::statemachine::SimpleStateItem("comSimpleState1", "comEntry", "comExit");
  composite->addState(comState1, &errorString);
  Q_ASSERT(comState1);
  //Simple-State
  model::statemachine::SimpleStateItem* comState2 = new model::statemachine::SimpleStateItem("comSimpleState2", "comEntry", "comExit");
  composite->addState(comState2, &errorString);
  Q_ASSERT(comState2);
  //add transitions

  model::statemachine::TransitionItem* t01 = smModel->addTimeoutTransition(QStringLiteral("Transition"),
                                                                          20,
                                                                          comState1,
                                                                          comState2,
                                                                          "",
                                                                          "Transition");

  Q_ASSERT(t01);



  model::statemachine::TransitionItem* t04 = smModel->addTimeoutTransition(QStringLiteral("Transition"),
                                                                          10,
                                                                          choice,
                                                                          junction,
                                                                          "",
                                                                          "Transition");
  Q_ASSERT(t04);

  //TransitionItem from init to state1
  model::statemachine::TransitionItem* t1 = smModel->addTimeoutTransition(QStringLiteral("Transition"),
                                                                          20,
                                                                          init,
                                                                          choice,
                                                                          "",
                                                                          "initTrans");
  Q_ASSERT(t1);

  //TransitionItem from state1 to comState1
  model::statemachine::TransitionItem* t2 = smModel->addTimeoutTransition(QStringLiteral("Transition"),
                                                                          10,
                                                                          choice,
                                                                          composite,
                                                                          "",
                                                                          "simpleTrans");
  Q_ASSERT(t2);

  smModel->setName("Preview");
}
inline void stateMachineGraphLoadSettings(graphlayout::ComCreateStateMachineGraph* comGraph, graphlayout::GraphQuickWidget* comQML){
  QSettings * settings = Core::ICore::settings();
  settings->beginGroup(QLatin1String("StateMachineEditor"));

  //SimpleState
  settings->beginGroup(QLatin1String("SimpleState"));
  if(qvariant_cast<QColor>(settings->value("Color"))==""){
    settings->endGroup();
    settings->endGroup();
    return;
  }
  comGraph->setSimpleShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setSimpleColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setSimpleBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
  comGraph->setSimpleBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
  comGraph->setSimpleLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comGraph->setSimpleLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
  settings->endGroup();

  //InitState
  settings->beginGroup(QLatin1String("InitState"));
  comGraph->setInitShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setInitColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setInitBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
  comGraph->setInitBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
  comGraph->setInitLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comGraph->setInitLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
  comGraph->setInitTokenColor(qvariant_cast<QColor>(settings->value(QLatin1String("TokenColor"))));
  settings->endGroup();

  //FinalState
  settings->beginGroup(QLatin1String("FinalState"));
  comGraph->setFinalShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setFinalColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setFinalBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
  comGraph->setFinalBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
  comGraph->setFinalLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comGraph->setFinalLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
  settings->endGroup();

  //CompositeState
  settings->beginGroup(QLatin1String("CompositeState"));
  comGraph->setCompositeShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setCompositeColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setCompositeBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
  comGraph->setCompositeBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
  comGraph->setCompositeLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comGraph->setCompositeLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
  settings->endGroup();

  //ConcurrentState
  settings->beginGroup(QLatin1String("ConcurrentState"));
  comGraph->setConcurrentShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setConcurrentColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setConcurrentBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
  comGraph->setConcurrentBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
  comGraph->setConcurrentLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comGraph->setConcurrentLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));

  comGraph->setConcurrentRegionColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionColor"))));
  comGraph->setConcurrentRegionBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionBorderColor"))));
  comGraph->setConcurrentRegionBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("RegionBorderWidth"))));
  comGraph->setConcurrentRegionLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionLabelColor"))));
  comGraph->setConcurrentRegionLabelSize(qvariant_cast<int>(settings->value(QLatin1String("RegionLabelSize"))));
  settings->endGroup();

  //JunctionState
  settings->beginGroup(QLatin1String("JunctionState"));
  comGraph->setJunctionShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setJunctionColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setJunctionBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
  comGraph->setJunctionBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
  comGraph->setJunctionLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comGraph->setJunctionLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
  settings->endGroup();

  //Transition
  settings->beginGroup(QLatin1String("Transition"));
  comGraph->setTransitionShape(graphlayout::ArrowEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))));
  comGraph->setTransitionColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
  comGraph->setTransitionArrowsize(qvariant_cast<int>(settings->value(QLatin1String("Arrowsize"))));
  comGraph->setTransitionWidth(qvariant_cast<double>(settings->value(QLatin1String("Linewidth"))));
  comGraph->setTransitionLabelSize(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
  comGraph->setTransitionLabelColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
  comQML->setZoomBound(qvariant_cast<double>(settings->value(QLatin1String("LabelZoomlevel"))));
  settings->endGroup();

  //Background and Title
  settings->beginGroup(QLatin1String("Background"));
  comQML->setBackgroundColor(qvariant_cast<QColor>(settings->value(QLatin1String("BackgroundColor"))));
  comQML->setTitleColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleColor"))));
  comQML->setTitleSize(qvariant_cast<int>(settings->value(QLatin1String("TitleSize"))));
  comQML->setTitleShape(graphlayout::ShapeEnumFromString(qvariant_cast<QString>(settings->value(QLatin1String("TitleShape")))));
  comQML->setTitleBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBorderColor"))));
  comQML->setTitleBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("TitleBorderWidth"))));
  comQML->setTitleBackgroundColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBackgroundColor"))));
  settings->endGroup();

  //Highlight
  settings->beginGroup(QLatin1String("Highlight"));
  comGraph->setHighlightStateColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateColor"))));
  comGraph->setHighlightStateBorderColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateBorderColor"))));
  comGraph->setHighlightStateBorderWidth(qvariant_cast<double>(settings->value(QLatin1String("StateBorderWidth"))));
  comGraph->setHighlightTransitionColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))));
  settings->endGroup();

  //Graphviz
  settings->beginGroup("Graphviz");
  comGraph->setSplines(graphlayout::SplineEnumFromString(qvariant_cast<QString>(settings->value("Spline"))));
  comGraph->setOverlap(graphlayout::OverlapEnumFromString(qvariant_cast<QString>(settings->value("Overlap"))));
  comQML->setAlgorithm(graphlayout::GraphvizAlgorithmFromString(qvariant_cast<QString>(settings->value("Algorithm"))));
  comQML->setZoomType(qvariant_cast<QString>(settings->value("Zoom")));
  settings->endGroup();
  settings->endGroup();

  settings->beginGroup(QLatin1String("Graphviz"));
  comQML->setGraphvizPath(qvariant_cast<QString>(settings->value(QLatin1String("GraphvizPath"))));
  settings->endGroup();
}
