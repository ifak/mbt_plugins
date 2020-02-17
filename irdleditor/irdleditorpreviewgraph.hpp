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

#include "mobata/model/requirement/requirementsmodel.hpp"
#include "mobata/model/requirement/requirementitem.hpp"
#include "graphlayout/mscquickwidget.hpp"
#include <mobata/model/msc/msc.hpp>
#include <coreplugin/icore.h>

inline void previewGraph(model::msc::MscModel* model)
{
    model::msc::MscModel* mscModel = dynamic_cast<model::msc::MscModel*>(model);
    QString errorString;
    model::msc::MscComponentItem* comp1 = new model::msc::MscComponentItem("Comp1");
    mscModel->addComponent(comp1, &errorString);
    model::msc::MscComponentItem* comp2 = new model::msc::MscComponentItem("Comp2");
    mscModel->addComponent(comp2, &errorString);
    model::msc::MscSequence* seq = new model::msc::MscSequence("seq");
    seq->addMessage(comp1,comp2,"Message");
    seq->addCheck(comp1,comp2,"Checkmessage");
    model::msc::MscFragmentItem* frag = seq->addFragment(model::msc::AlternativeFragment);
    frag->addRegion("Region1");
    frag->regions().first()->addStatement(comp1,"Statement");
    frag->addRegion("Region2");
    model::msc::MscTimerItem* timer = frag->regions().last()->addTimer(comp1,"Timer");
    timer->setValue(100);
    mscModel->addSequence(seq);
}

inline void irdlGraphLoadSettings(graphlayout::MscQuickWidget* mscQuickWidget)
{
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("IrdlEditor"));
    settings->beginGroup(QLatin1String("Background"));
    mscQuickWidget->setComponentColor(qvariant_cast<QColor>(settings->value(QLatin1String("ComponentColor"), QVariant(QColor("white")))));
    mscQuickWidget->setComponentTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("ComponentTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setLifelineColor(qvariant_cast<QColor>(settings->value(QLatin1String("LifelineColor"), QVariant(QColor("black")))));
    mscQuickWidget->setBackgroundColor(qvariant_cast<QColor>(settings->value(QLatin1String("BackgroundColor"), QVariant(QColor("white")))));
    mscQuickWidget->setHighlightColor(qvariant_cast<QColor>(settings->value(QLatin1String("HighlightColor"), QVariant(QColor("red")))));
    settings->endGroup();
    settings->beginGroup(QLatin1String("Steps"));
    mscQuickWidget->setStatementColor(qvariant_cast<QColor>(settings->value(QLatin1String("StatementColor"), QVariant(QColor("white")))));
    mscQuickWidget->setStatementTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("StatementTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setTimerColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimerColor"), QVariant(QColor("white")))));
    mscQuickWidget->setTimerTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimerTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setTimeoutColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimeoutColor"), QVariant(QColor("white")))));
    mscQuickWidget->setTimeoutTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimeoutTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setSleeptimerColor(qvariant_cast<QColor>(settings->value(QLatin1String("SleeptimerColor"), QVariant(QColor("white")))));
    mscQuickWidget->setSleeptimerTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("SleeptimerTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setMessageColor(qvariant_cast<QColor>(settings->value(QLatin1String("MessageColor"), QVariant(QColor("black")))));
    mscQuickWidget->setMessageTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("MessageTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setCheckmessageColor(qvariant_cast<QColor>(settings->value(QLatin1String("CheckmessageColor"), QVariant(QColor("black")))));
    mscQuickWidget->setCheckmessageTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("CheckmessageTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setFragmentColor(qvariant_cast<QColor>(settings->value(QLatin1String("FragmentColor"), QVariant(QColor("black")))));
    mscQuickWidget->setFragmentTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("FragmentTextcolor"), QVariant(QColor("black")))));
    mscQuickWidget->setRegionColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionColor"), QVariant(QColor("black")))));
    mscQuickWidget->setRegionTextColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionTextcolor"), QVariant(QColor("black")))));
    settings->endGroup();
    settings->endGroup();
}
