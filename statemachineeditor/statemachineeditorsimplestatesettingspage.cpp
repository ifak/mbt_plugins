#include "statemachineeditorsimplestatesettingspage.hpp"
#include "statemachineeditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace statemachineeditor {

class StateMachineEditorSimplestateSettingsWidget::Private
{
    friend class StateMachineEditorSimplestateSettingsWidget;
};

StateMachineEditorSimplestateSettingsWidget::StateMachineEditorSimplestateSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

StateMachineEditorSimplestateSettingsWidget::~StateMachineEditorSimplestateSettingsWidget()
{
    delete _d;
}

// ------------------ StateMachineEditorSimplestateSettingsPage
StateMachineEditorSimplestateSettingsPage::StateMachineEditorSimplestateSettingsPage()
{
    this->setId(Constants::statemachine_SETTINGS_ID);
    this->setDisplayName(tr("SimpleStates"));
    this->setCategory(Constants::statemachine_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::statemachine_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::statemachine_SETTINGS_CATEGORY_ICON));
}

StateMachineEditorSimplestateSettingsPage::~StateMachineEditorSimplestateSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* StateMachineEditorSimplestateSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new StateMachineEditorSimplestateSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();


    settings->beginGroup(QLatin1String("SimpleState"));
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Shape")))){
        settings->setValue(QLatin1String("Shape"),"roundedRect");
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

    settings->beginGroup(QLatin1String("InitState"));
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Shape")))){
        settings->setValue(QLatin1String("Shape"),"roundedRect");
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
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TokenColor"))).isValid()==false){
        settings->setValue(QLatin1String("TokenColor"),QColor("black"));
    }
    settings->endGroup();

    settings->beginGroup(QLatin1String("FinalState"));
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Shape")))){
        settings->setValue(QLatin1String("Shape"),"roundedRect");
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
            this->_ui.SSShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
            this->_ui.ISShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
            this->_ui.FSShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
        }else{
            break;
        }
    }

    settings->beginGroup(QLatin1String("SimpleState"));
    this->_ui.SSShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.SSColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.SSBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
    this->_ui.SSBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
    this->_ui.SSLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.SSLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    settings->endGroup();
    settings->beginGroup(QLatin1String("InitState"));
    this->_ui.ISShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.ISColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.ISBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
    this->_ui.ISBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
    this->_ui.ISLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.ISLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    this->_ui.ISTokenColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TokenColor"))));
    settings->endGroup();
    settings->beginGroup(QLatin1String("FinalState"));
    this->_ui.FSShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("Shape"))));
    this->_ui.FSColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("Color"))));
    this->_ui.FSBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BorderColor"))));
    this->_ui.FSBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("BorderWidth"))));
    this->_ui.FSLabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));
    this->_ui.FSLabelSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("LabelSize"))));
    settings->endGroup();
    settings->endGroup();

    //SimpleState
    QObject::connect(this->_ui.SSShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SSColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SSBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SSBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SSLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.SSLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    // InitState
    QObject::connect(this->_ui.ISShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ISColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ISBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ISBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ISLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ISLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ISTokenColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    //FinalState
    QObject::connect(this->_ui.FSShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.FSColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.FSBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.FSBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.FSLabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(this->_ui.FSLabelColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &StateMachineEditorSimplestateSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}
void StateMachineEditorSimplestateSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void StateMachineEditorSimplestateSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateStateMachineGraph comGraph(this->_graph,this->_model);
        stateMachineGraphLoadSettings(&comGraph, this->_comQML);
        //SimpleState
        comGraph.setSimpleShape(graphlayout::ShapeEnumFromString(this->_ui.SSShape->currentText()));
        comGraph.setSimpleColor(this->_ui.SSColor->color());
        comGraph.setSimpleBorderColor(this->_ui.SSBorderColor->color());
        comGraph.setSimpleBorderWidth(this->_ui.SSBorderWidth->value());
        comGraph.setSimpleLabelSize(this->_ui.SSLabelSize->value());
        comGraph.setSimpleLabelColor(this->_ui.SSLabelColor->color());

        //InitState
        comGraph.setInitShape(graphlayout::ShapeEnumFromString(this->_ui.ISShape->currentText()));
        comGraph.setInitColor(this->_ui.ISColor->color());
        comGraph.setInitBorderColor(this->_ui.ISBorderColor->color());
        comGraph.setInitBorderWidth(this->_ui.ISBorderWidth->value());
        comGraph.setInitLabelSize(this->_ui.ISLabelSize->value());
        comGraph.setInitLabelColor(this->_ui.ISLabelColor->color());
        comGraph.setInitTokenColor(this->_ui.ISTokenColor->color());

        //FinalState
        comGraph.setFinalShape(graphlayout::ShapeEnumFromString(this->_ui.FSShape->currentText()));
        comGraph.setFinalColor(this->_ui.FSColor->color());
        comGraph.setFinalBorderColor(this->_ui.FSBorderColor->color());
        comGraph.setFinalBorderWidth(this->_ui.FSBorderWidth->value());
        comGraph.setFinalLabelSize(this->_ui.FSLabelSize->value());
        comGraph.setFinalLabelColor(this->_ui.FSLabelColor->color());

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

void StateMachineEditorSimplestateSettingsPage::apply(){
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));
    settings->beginGroup(QLatin1String("SimpleState"));
    settings->setValue(QLatin1String("Shape"),this->_ui.SSShape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.SSColor->color());
    settings->setValue(QLatin1String("BorderColor"),this->_ui.SSBorderColor->color());
    settings->setValue(QLatin1String("BorderWidth"),this->_ui.SSBorderWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.SSLabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.SSLabelColor->color());
    settings->endGroup();

    settings->beginGroup(QLatin1String("InitState"));
    settings->setValue(QLatin1String("Shape"),this->_ui.ISShape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.ISColor->color());
    settings->setValue(QLatin1String("BorderColor"),this->_ui.ISBorderColor->color());
    settings->setValue(QLatin1String("BorderWidth"),this->_ui.ISBorderWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.ISLabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.ISLabelColor->color());
    settings->setValue(QLatin1String("TokenColor"),this->_ui.ISTokenColor->color());
    settings->endGroup();

    settings->beginGroup(QLatin1String("FinalState"));
    settings->setValue(QLatin1String("Shape"),this->_ui.FSShape->currentText());
    settings->setValue(QLatin1String("Color"),this->_ui.FSColor->color());
    settings->setValue(QLatin1String("BorderColor"),this->_ui.FSBorderColor->color());
    settings->setValue(QLatin1String("BorderWidth"),this->_ui.FSBorderWidth->value());
    settings->setValue(QLatin1String("LabelSize"),this->_ui.FSLabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.FSLabelColor->color());
    settings->endGroup();

    settings->endGroup();
    emit appliedSettings();
    return;
}

void StateMachineEditorSimplestateSettingsPage::finish()
{
    // Do nothing

    return;
}

bool StateMachineEditorSimplestateSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}//end namespace statemachineeditor


