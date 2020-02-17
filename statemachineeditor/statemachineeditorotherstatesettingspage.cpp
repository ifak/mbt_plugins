#include "statemachineeditorotherstatesettingspage.hpp"
#include "statemachineeditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace statemachineeditor {

class StateMachineEditorOtherstateSettingsWidget::Private
{
    friend class StateMachineEditorOtherstateSettingsWidget;
};

StateMachineEditorOtherstateSettingsWidget::StateMachineEditorOtherstateSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

StateMachineEditorOtherstateSettingsWidget::~StateMachineEditorOtherstateSettingsWidget()
{
    delete _d;
}

// ------------------ StateMachineEditorOtherstateSettingsPage
StateMachineEditorOtherstateSettingsPage::StateMachineEditorOtherstateSettingsPage()
{
    this->setId(Constants::statemachine_SETTINGS_ID);
    this->setDisplayName(tr("OtherStates"));
    this->setCategory(Constants::statemachine_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::statemachine_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::statemachine_SETTINGS_CATEGORY_ICON));
}

StateMachineEditorOtherstateSettingsPage::~StateMachineEditorOtherstateSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* StateMachineEditorOtherstateSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new StateMachineEditorOtherstateSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();

    settings->beginGroup(QLatin1String("CompositeState"));
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Shape")))){
        settings->setValue(QLatin1String("Shape"),"rect");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))).isValid()==false){
        settings->setValue(QLatin1String("Color"),QColor("white"));
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))).isValid()==false){
        settings->setValue(QLatin1String("BorderColor"),QColor("black"));
    }
    if(qvariant_cast<QString>(settings->value(QLatin1String("BorderWidth")))==""){
        settings->setValue(QLatin1String("BorderWidth"),2);
    }
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("LabelSize")))){
        settings->setValue(QLatin1String("LabelSize"),"12");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))).isValid()==false){
        settings->setValue(QLatin1String("LabelColor"),QColor("black"));
    }
    settings->endGroup();

    settings->beginGroup(QLatin1String("ConcurrentState"));
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Shape")))){
        settings->setValue(QLatin1String("Shape"),"rect");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))).isValid()==false){
        settings->setValue(QLatin1String("Color"),QColor("white"));
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))).isValid()==false){
        settings->setValue(QLatin1String("BorderColor"),QColor("black"));
    }
    if(qvariant_cast<QString>(settings->value(QLatin1String("BorderWidth")))==""){
        settings->setValue(QLatin1String("BorderWidth"),2);
    }
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("LabelSize")))){
        settings->setValue(QLatin1String("LabelSize"),"12");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))).isValid()==false){
        settings->setValue(QLatin1String("LabelColor"),QColor("black"));
    }

    if(qvariant_cast<QColor>(settings->value(QLatin1String("RegionColor"))).isValid()==false){
        settings->setValue(QLatin1String("RegionColor"),QColor("lightgray"));
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("RegionBorderColor"))).isValid()==false){
        settings->setValue(QLatin1String("RegionBorderColor"),QColor("black"));
    }
    if(qvariant_cast<QString>(settings->value(QLatin1String("RegionBorderWidth")))==""){
        settings->setValue(QLatin1String("RegionBorderWidth"),2.0);
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("RegionLabelColor"))).isValid()==false){
        settings->setValue(QLatin1String("RegionLabelColor"),QColor("black"));
    }
    if(qvariant_cast<QString>(settings->value(QLatin1String("RegionLabelSize")))==""){
        settings->setValue(QLatin1String("RegionLabelSize"), 12);
    }
    settings->endGroup();

    settings->beginGroup(QLatin1String("JunctionState"));
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Shape")))){
        settings->setValue(QLatin1String("Shape"),"circle");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))).isValid()==false){
        settings->setValue(QLatin1String("Color"),QColor("white"));
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))).isValid()==false){
        settings->setValue(QLatin1String("BorderColor"),QColor("black"));
    }
    if(qvariant_cast<QString>(settings->value(QLatin1String("BorderWidth")))==""){
        settings->setValue(QLatin1String("BorderWidth"),2);
    }
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("LabelSize")))){
        settings->setValue(QLatin1String("LabelSize"),"12");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))).isValid()==false){
        settings->setValue(QLatin1String("LabelColor"),QColor("black"));
    }
    settings->endGroup();

    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i)!="rect"){
            this->_ui.CSShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
            this->_ui.PSShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
            this->_ui.JSShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
        }else{
            break;
        }
    }

    settings->beginGroup(QLatin1String("CompositeState"));
    this->_ui.CSShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.CSColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.CSBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
    this->_ui.CSBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
    this->_ui.CSLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.CSLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    settings->endGroup();
    settings->beginGroup(QLatin1String("ConcurrentState"));
    this->_ui.PSShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.PSColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.PSBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
    this->_ui.PSBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
    this->_ui.PSLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.PSLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));

    this->_ui.CRColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionColor"))));
    this->_ui.CRBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionBorderColor"))));
    this->_ui.CRBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("RegionBorderWidth"))));
    this->_ui.CRLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionLabelColor"))));
    this->_ui.CRLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("RegionLabelSize"))));
    settings->endGroup();
    settings->beginGroup(QLatin1String("JunctionState"));
    this->_ui.JSShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.JSColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.JSBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
    this->_ui.JSBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
    this->_ui.JSLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.JSLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    settings->endGroup();
    settings->endGroup();

    //CompositeState
    QObject::connect(this->_ui.CSShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CSColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CSBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CSBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CSLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CSLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    // ConcurrentState
    QObject::connect(this->_ui.PSShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PSColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PSBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PSBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PSLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PSLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);

    QObject::connect(this->_ui.CRColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CRBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CRBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CRLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.CRLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    //JunctionState
    QObject::connect(this->_ui.JSShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.JSColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.JSBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.JSBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.JSLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.JSLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &StateMachineEditorOtherstateSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}
void StateMachineEditorOtherstateSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void StateMachineEditorOtherstateSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateStateMachineGraph comGraph(this->_graph,this->_model);
        stateMachineGraphLoadSettings(&comGraph, this->_comQML);
        //CompositeState
        comGraph.setCompositeShape(graphlayout::ShapeEnumFromString(this->_ui.CSShape->currentText()));
        comGraph.setCompositeColor(this->_ui.CSColor->color());
        comGraph.setCompositeBorderColor(this->_ui.CSBorderColor->color());
        comGraph.setCompositeBorderWidth(this->_ui.CSBorderWidth->value());
        comGraph.setCompositeLabelSize(this->_ui.CSLabelSize->value());
        comGraph.setCompositeLabelColor(this->_ui.CSLabelColor->color());

        //ConcurrentState
        comGraph.setConcurrentShape(graphlayout::ShapeEnumFromString(this->_ui.PSShape->currentText()));
        comGraph.setConcurrentColor(this->_ui.PSColor->color());
        comGraph.setConcurrentBorderColor(this->_ui.PSBorderColor->color());
        comGraph.setConcurrentBorderWidth(this->_ui.PSBorderWidth->value());
        comGraph.setConcurrentLabelSize(this->_ui.PSLabelSize->value());
        comGraph.setConcurrentLabelColor(this->_ui.PSLabelColor->color());

        comGraph.setConcurrentRegionColor(this->_ui.CRColor->color());
        comGraph.setConcurrentRegionBorderColor(this->_ui.CRBorderColor->color());
        comGraph.setConcurrentRegionBorderWidth(this->_ui.CRBorderWidth->value());
        comGraph.setConcurrentRegionLabelColor(this->_ui.CRLabelColor->color());
        comGraph.setConcurrentRegionLabelSize(this->_ui.CRLabelSize->value());

        //JunctionState
        comGraph.setJunctionShape(graphlayout::ShapeEnumFromString(this->_ui.JSShape->currentText()));
        comGraph.setJunctionColor(this->_ui.JSColor->color());
        comGraph.setJunctionBorderColor(this->_ui.JSBorderColor->color());
        comGraph.setJunctionBorderWidth(this->_ui.JSBorderWidth->value());
        comGraph.setJunctionLabelSize(this->_ui.JSLabelSize->value());
        comGraph.setJunctionLabelColor(this->_ui.JSLabelColor->color());

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

void StateMachineEditorOtherstateSettingsPage::apply(){
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));
    settings->beginGroup(QLatin1String("CompositeState"));
    settings->setValue(QLatin1String("Shape"),this->_ui.CSShape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.CSColor->color());
    settings->setValue(QLatin1String("BorderColor"),this->_ui.CSBorderColor->color());
    settings->setValue(QLatin1String("BorderWidth"),this->_ui.CSBorderWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.CSLabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.CSLabelColor->color());
    settings->endGroup();

    settings->beginGroup(QLatin1String("ConcurrentState"));
    settings->setValue(QLatin1String("Shape"),this->_ui.PSShape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.PSColor->color());
    settings->setValue(QLatin1String("BorderColor"),this->_ui.PSBorderColor->color());
    settings->setValue(QLatin1String("BorderWidth"),this->_ui.PSBorderWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.PSLabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.PSLabelColor->color());

    settings->setValue(QLatin1String("RegionColor"),this->_ui.CRColor->color());
    settings->setValue(QLatin1String("RegionBorderColor"),this->_ui.CRBorderColor->color());
    settings->setValue(QLatin1String("RegionBorderWidth"),this->_ui.CRBorderWidth->value());
    settings->setValue(QLatin1String("RegionLabelColor"),this->_ui.CRLabelColor->color());
    settings->setValue(QLatin1String("RegionLabelSize"),this->_ui.CRLabelSize->value());
    settings->endGroup();

    settings->beginGroup(QLatin1String("JunctionState"));
    settings->setValue(QLatin1String("Shape"),this->_ui.JSShape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.JSColor->color());
    settings->setValue(QLatin1String("BorderColor"),this->_ui.JSBorderColor->color());
    settings->setValue(QLatin1String("BorderWidth"),this->_ui.JSBorderWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.JSLabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.JSLabelColor->color());
    settings->endGroup();

    settings->endGroup();
    emit appliedSettings();
    return;
}

void StateMachineEditorOtherstateSettingsPage::finish()
{
    // Do nothing

    return;
}

bool StateMachineEditorOtherstateSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}//end namespace statemachineeditor


