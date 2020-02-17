#include "statemachineeditortransitionsettingspage.hpp"
#include "statemachineeditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace statemachineeditor {

class StateMachineEditorTransitionSettingsWidget::Private
{
    friend class StateMachineEditorTransitionSettingsWidget;
};

StateMachineEditorTransitionSettingsWidget::StateMachineEditorTransitionSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

StateMachineEditorTransitionSettingsWidget::~StateMachineEditorTransitionSettingsWidget()
{
    delete _d;
}

// ------------------ StateMachineEditorTransitionSettingsPage
StateMachineEditorTransitionSettingsPage::StateMachineEditorTransitionSettingsPage()
{
    this->setId(Constants::statemachine_SETTINGS_ID);
    this->setDisplayName(tr("Transitions"));
    this->setCategory(Constants::statemachine_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::statemachine_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::statemachine_SETTINGS_CATEGORY_ICON));
}

StateMachineEditorTransitionSettingsPage::~StateMachineEditorTransitionSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* StateMachineEditorTransitionSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new StateMachineEditorTransitionSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();


    settings->beginGroup(QLatin1String("Transition"));
    //Transition
    if(qvariant_cast<QString>(settings->value(QLatin1String("Shape")))=="")
        settings->setValue(QLatin1String("Shape"),"arrow");
    if(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))).isValid()==false)
        settings->setValue(QLatin1String("Color"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("Arrowsize")))=="")
        settings->setValue(QLatin1String("Arrowsize"),15);
    if(qvariant_cast<QString>(settings->value(QLatin1String("Linewidth")))=="")
        settings->setValue(QLatin1String("Linewidth"),2);
    if(qvariant_cast<QString>(settings->value(QLatin1String("LabelSize")))=="")
        settings->setValue(QLatin1String("LabelSize"),12.0);
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))).isValid()==false)
        settings->setValue(QLatin1String("LabelColor"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("LabelZoomlevel")))=="")
        settings->setValue(QLatin1String("LabelZoomlevel"),1.0);
    settings->endGroup();

    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ArrowEnumToString((graphlayout::ArrowEnum)i)!="Arrow"){
            this->_ui.Shape->addItem(graphlayout::ArrowEnumToString((graphlayout::ArrowEnum)i));
        }else{
            break;
        }
    }

    settings->beginGroup(QLatin1String("Transition"));
    this->_ui.Shape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.Color->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.Arrowsize->setValue(qvariant_cast<int>(settings->value(QLatin1String("Arrowsize"))));
    this->_ui.LineWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("Linewidth"))));
    this->_ui.LabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    this->_ui.LabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.LabelZoom->setValue(qvariant_cast<double>(settings->value(QLatin1String("LabelZoomlevel"))));
    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.Shape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.Color, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.Arrowsize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LineWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LabelZoom, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &StateMachineEditorTransitionSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void StateMachineEditorTransitionSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void StateMachineEditorTransitionSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateStateMachineGraph comGraph(this->_graph,this->_model);
        stateMachineGraphLoadSettings(&comGraph, this->_comQML);
        //transition
        comGraph.setTransitionShape(graphlayout::ArrowEnumFromString(this->_ui.Shape->currentText()));
        comGraph.setTransitionColor(this->_ui.Color->color());
        comGraph.setTransitionArrowsize(this->_ui.Arrowsize->value());
        comGraph.setTransitionWidth(this->_ui.LineWidth->value());
        //Label
        comGraph.setTransitionLabelSize(this->_ui.LabelSize->value());
        comGraph.setTransitionLabelColor(this->_ui.LabelColor->color());
        this->_comQML->setZoomBound(this->_ui.LabelZoom->value());

        QString errorString;
        bool result = comGraph.execute(&errorString);
        if(!result)
        {
            qWarning()<<"error during layout graph creation for arc settings page: "<<errorString;
            return;
        }
        result = this->_comQML->layout(&errorString);
        if(!result)
        {
            qWarning()<<"error during layouting of preview spenat graph for arc settings page: "<<errorString;
            return;
        }
    }

}

void StateMachineEditorTransitionSettingsPage::apply()
{
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));
    settings->beginGroup(QLatin1String("Transition"));

    settings->setValue(QLatin1String("Shape"),this->_ui.Shape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.Color->color());
    settings->setValue(QLatin1String("Arrowsize"),this->_ui.Arrowsize->value());
    settings->setValue(QLatin1String("Linewidth"),this->_ui.LineWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.LabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.LabelColor->color());
    settings->setValue(QLatin1String("LabelZoomlevel"),this->_ui.LabelZoom->value());
    settings->endGroup();
    settings->endGroup();
    emit appliedSettings();
    return;
}

void StateMachineEditorTransitionSettingsPage::finish()
{
    // Do nothing

    return;
}

bool StateMachineEditorTransitionSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}//end namespace statemachineeditor


