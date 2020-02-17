#include "spenateditortransitionsettingspage.hpp"
#include "spenateditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace spenateditor {
namespace Internal {

class SpenatEditorTransitionSettingsWidget::Private
{
    friend class SpenatEditorTransitionSettingsWidget;
};

SpenatEditorTransitionSettingsWidget::SpenatEditorTransitionSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

SpenatEditorTransitionSettingsWidget::~SpenatEditorTransitionSettingsWidget()
{
    delete _d;
}

// ------------------ SpenatEditorTransitionSettingsPage
SpenatEditorTransitionSettingsPage::SpenatEditorTransitionSettingsPage()
{
    this->setId(Constants::SPENAT_SETTINGS_ID);
    this->setDisplayName(tr("Transitions"));
    this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorTransitionSettingsPage::~SpenatEditorTransitionSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* SpenatEditorTransitionSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new SpenatEditorTransitionSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();

    this->_ui.SimpleTransition->setChecked(true);
    QObject::connect(this->_ui.SimpleTransition, &QCheckBox::toggled,
                     this->_ui.SimpleTransitionColor,&Utils::QtColorButton::setEnabled);
    QObject::connect(this->_ui.SimpleTransition, &QCheckBox::toggled,
                     this->_ui.SimpleTransitionArrowtype,&QComboBox::setEnabled);
    QObject::connect(this->_ui.SimpleTransition, &QCheckBox::toggled,
                     this->_ui.SimpleTransitionArrowsize,&QSpinBox::setEnabled);
    QObject::connect(this->_ui.SimpleTransition, &QCheckBox::toggled,
                     this->_ui.SimpleTransitionLineWidth,&QDoubleSpinBox::setEnabled);

    settings->beginGroup(QLatin1String("Transition"));
    //Transition
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionShape")))=="")
        settings->setValue(QLatin1String("TransitionShape"),"rect");
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))).isValid()==false)
        settings->setValue(QLatin1String("TransitionColor"),QColor("white"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionBorderWidth")))=="")
        settings->setValue(QLatin1String("TransitionBorderWidth"),2);
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionBorderColor"))).isValid()==false)
        settings->setValue(QLatin1String("TransitionBorderColor"),QColor("black"));
    //SimpleTransition
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionSimple")))=="")
        settings->setValue(QLatin1String("TransitionSimple"),true);
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionSimpleColor"))).isValid()==false)
        settings->setValue(QLatin1String("TransitionSimpleColor"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionSimpleArrowtype")))=="")
        settings->setValue(QLatin1String("TransitionSimpleArrowtype"),"Arrow");
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionSimpleArrowsize")))=="")
        settings->setValue(QLatin1String("TransitionSimpleArrowsize"),15);
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionSimpleLineWidth")))=="")
        settings->setValue(QLatin1String("TransitionSimpleLineWidth"),2);
    //Label
    if(qvariant_cast<QString>(settings->value(QLatin1String("LabelFont")))=="")
        settings->setValue(QLatin1String("LabelFont"),"Calibri");
    if(qvariant_cast<QString>(settings->value(QLatin1String("LabelSize")))=="")
        settings->setValue(QLatin1String("LabelSize"),12.0);
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))).isValid()==false)
        settings->setValue(QLatin1String("LabelColor"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("GuardLabelSize")))=="")
        settings->setValue(QLatin1String("GuardLabelSize"),12.0);
    if(qvariant_cast<QString>(settings->value(QLatin1String("ActionIndention")))=="")
        settings->setValue(QLatin1String("ActionIndention"),2);
    if(qvariant_cast<QString>(settings->value(QLatin1String("LabelZoomlevel")))=="")
        settings->setValue(QLatin1String("LabelZoomlevel"),1.0);
    settings->endGroup();
    //Labeltext
    settings->beginGroup("Labeltext");
    if(qvariant_cast<QString>(settings->value(QLatin1String("ShowName")))=="")
        settings->setValue(QLatin1String("ShowName"),true);
    if(qvariant_cast<QString>(settings->value(QLatin1String("ShowGuard")))=="")
        settings->setValue(QLatin1String("ShowGuard"),true);
    if(qvariant_cast<QString>(settings->value(QLatin1String("ShowAction")))=="")
        settings->setValue(QLatin1String("ShowAction"),false);
    if(qvariant_cast<QString>(settings->value(QLatin1String("GraphvizEdgeLabel")))=="")
        settings->setValue(QLatin1String("GraphvizEdgeLabel"),false);
    if(qvariant_cast<QString>(settings->value(QLatin1String("GraphvizNodeLabel")))=="")
        settings->setValue(QLatin1String("GraphvizNodeLabel"),false);
    settings->endGroup();


    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i)!="rect"){
            this->_ui.TransitionShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
        }else{
            break;
        }
    }
    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ArrowEnumToString((graphlayout::ArrowEnum)i)!="Arrow"){
            this->_ui.SimpleTransitionArrowtype->addItem(graphlayout::ArrowEnumToString((graphlayout::ArrowEnum)i));
        }else{
            break;
        }
    }

    settings->beginGroup(QLatin1String("Transition"));
    this->_ui.TransitionShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("TransitionShape"))));
    this->_ui.TransitionColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))));
    this->_ui.TransitionBorderWidth->setValue(qvariant_cast<int>(settings->value(QLatin1String("TransitionBorderWidth"))));
    this->_ui.TransitionBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionBorderColor"))));

    this->_ui.SimpleTransition->setChecked(qvariant_cast<bool>(settings->value(QLatin1String("TransitionSimple"))));
    this->_ui.SimpleTransitionColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionSimpleColor"))));

    this->_ui.SimpleTransitionArrowtype->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("TransitionSimpleArrowtype"))));
    this->_ui.SimpleTransitionArrowsize->setValue(qvariant_cast<int>(settings->value(QLatin1String("TransitionSimpleArrowsize"))));
    this->_ui.SimpleTransitionLineWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("TransitionSimpleLineWidth"))));

    this->_ui.LabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    this->_ui.LabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.GuardLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("GuardLabelSize"))));
    this->_ui.ActionIndention->setValue(qvariant_cast<int>(settings->value(QLatin1String("ActionIndention"))));
    this->_ui.LabelZoom->setValue(qvariant_cast<double>(settings->value(QLatin1String("LabelZoomlevel"))));
    settings->endGroup();
    settings->beginGroup("Labeltext");
    this->_ui.ShowName->setChecked(qvariant_cast<bool>(settings->value(QLatin1String("ShowName"))));
    this->_ui.ShowGuard->setChecked(qvariant_cast<bool>(settings->value(QLatin1String("ShowGuard"))));
    this->_ui.ShowAction->setChecked(qvariant_cast<bool>(settings->value(QLatin1String("ShowAction"))));
    this->_ui.GraphvizEdgeLabel->setChecked(qvariant_cast<bool>(settings->value(QLatin1String("GraphvizEdgeLabel"))));
    this->_ui.GraphvizNodeLabel->setChecked(qvariant_cast<bool>(settings->value(QLatin1String("GraphvizNodeLabel"))));
    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.TransitionShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);

    QObject::connect(this->_ui.SimpleTransition, &QCheckBox::toggled,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SimpleTransitionColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SimpleTransitionArrowtype, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SimpleTransitionArrowsize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SimpleTransitionLineWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);

    QObject::connect(this->_ui.LabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LabelColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.GuardLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ActionIndention, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LabelZoom, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);

    QObject::connect(this->_ui.ShowName, &QCheckBox::toggled,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ShowGuard, &QCheckBox::toggled,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ShowAction, &QCheckBox::toggled,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.GraphvizEdgeLabel, &QCheckBox::toggled,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.GraphvizNodeLabel, &QCheckBox::toggled,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &SpenatEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void SpenatEditorTransitionSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}

void SpenatEditorTransitionSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){

        graphlayout::ComCreateDeclGraph comGraph(this->_graph,this->_model);
        spenatGraphLoadSettings(&comGraph, this->_comQML);

        //transition
        comGraph.setTransitionShape(graphlayout::ShapeEnumFromString(this->_ui.TransitionShape->currentText()));
        comGraph.setTransitionColor(this->_ui.TransitionColor->color());
        comGraph.setTransitionBorderWidth(this->_ui.TransitionBorderWidth->value());
        comGraph.setTransitionBorderColor(this->_ui.TransitionBorderColor->color());
        //SimpleTransition
        comGraph.setTransitionSimple(this->_ui.SimpleTransition->isChecked());
        comGraph.setTransitionSimpleColor(this->_ui.SimpleTransitionColor->color());
        comGraph.setTransitionSimpleArrowtype(graphlayout::ArrowEnumFromString(this->_ui.SimpleTransitionArrowtype->currentText()));
        comGraph.setTransitionSimpleArrowsize(this->_ui.SimpleTransitionArrowsize->value());
        comGraph.setTransitionSimpleLinewidth(this->_ui.SimpleTransitionLineWidth->value());
        //Label
        comGraph.setTransitionLabelSize(this->_ui.LabelSize->value());
        comGraph.setTransitionLabelColor(this->_ui.LabelColor->color());
        comGraph.setTransitionGuardLabelSize(this->_ui.GuardLabelSize->value());
        comGraph.setTransitionActionIndention(this->_ui.ActionIndention->value());
        this->_comQML->setZoomBound(this->_ui.LabelZoom->value());
        //LabelText
        comGraph.setShowLabelName(this->_ui.ShowName->isChecked());
        comGraph.setShowLabelGuard(this->_ui.ShowGuard->isChecked());
        comGraph.setShowLabelAction(this->_ui.ShowAction->isChecked());
        comGraph.setStaticEdgeLabels(!this->_ui.GraphvizEdgeLabel->isChecked());
        comGraph.setStaticNodeLabels(!this->_ui.GraphvizNodeLabel->isChecked());

        QString errorString;
        bool result = comGraph.execute(&errorString);
        if(!result)
        {
            qWarning()<<"error during layout graph creation for transitions settings page: "<<errorString;
            return;
        }
        result = this->_comQML->layout(&errorString);
        if(!result)
        {
            qWarning()<<"error during layouting of preview spenat graph for transitions settings page: "<<errorString;
            return;
        }
    }
}

void SpenatEditorTransitionSettingsPage::apply()
{
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup(QLatin1String("Transition"));

    settings->setValue(QLatin1String("TransitionShape"),this->_ui.TransitionShape->currentText());
    settings->setValue(QLatin1String("TransitionColor"),this->_ui.TransitionColor->color());
    settings->setValue(QLatin1String("TransitionBorderWidth"),this->_ui.TransitionBorderWidth->value());
    settings->setValue(QLatin1String("TransitionBorderColor"),this->_ui.TransitionBorderColor->color());

    settings->setValue(QLatin1String("TransitionSimple"),this->_ui.SimpleTransition->isChecked());
    settings->setValue(QLatin1String("TransitionSimpleColor"),this->_ui.SimpleTransitionColor->color());
    settings->setValue(QLatin1String("TransitionSimpleArrowtype"),this->_ui.SimpleTransitionArrowtype->currentText());
    settings->setValue(QLatin1String("TransitionSimpleArrowsize"),this->_ui.SimpleTransitionArrowsize->value());
    settings->setValue(QLatin1String("TransitionSimpleLineWidth"),this->_ui.SimpleTransitionLineWidth->value());


    settings->setValue(QLatin1String("LabelSize"),this->_ui.LabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.LabelColor->color());
    settings->setValue(QLatin1String("GuardLabelSize"),this->_ui.GuardLabelSize->value());
    settings->setValue(QLatin1String("ActionIndention"),this->_ui.ActionIndention->value());
    settings->setValue(QLatin1String("LabelZoomlevel"),this->_ui.LabelZoom->value());
    settings->endGroup();
    settings->beginGroup("Labeltext");
    settings->setValue(QLatin1String("ShowName"),this->_ui.ShowName->isChecked());
    settings->setValue(QLatin1String("ShowGuard"),this->_ui.ShowGuard->isChecked());
    settings->setValue(QLatin1String("ShowAction"),this->_ui.ShowAction->isChecked());
    settings->setValue(QLatin1String("GraphvizEdgeLabel"),this->_ui.GraphvizEdgeLabel->isChecked());
    settings->setValue(QLatin1String("GraphvizNodeLabel"),this->_ui.GraphvizNodeLabel->isChecked());
    settings->endGroup();
    settings->endGroup();

    emit this->appliedSettings();
    return;
}

void SpenatEditorTransitionSettingsPage::finish()
{
    // Do nothing

    return;
}

bool SpenatEditorTransitionSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


