#include "spenateditorplacesettingspage.hpp"
#include "spenateditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace spenateditor {
namespace Internal {

class SpenatEditorPlaceSettingsWidget::Private
{
    friend class SpenatEditorPlaceSettingsWidget;
};

SpenatEditorPlaceSettingsWidget::SpenatEditorPlaceSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

SpenatEditorPlaceSettingsWidget::~SpenatEditorPlaceSettingsWidget()
{
    delete _d;
}

// ------------------ SpenatEditorPlaceSettingsPage
SpenatEditorPlaceSettingsPage::SpenatEditorPlaceSettingsPage()
{
    this->setId(Constants::SPENAT_SETTINGS_ID);
    this->setDisplayName(tr("Places"));
    this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorPlaceSettingsPage::~SpenatEditorPlaceSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* SpenatEditorPlaceSettingsPage::createPage(QWidget *parent)
{

    this->_widget = new SpenatEditorPlaceSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();


    settings->beginGroup(QLatin1String("Place"));

    if(qvariant_cast<QColor>(settings->value(QLatin1String("InitPlaceColor"))).isValid()==false){
        settings->setValue(QLatin1String("InitPlaceColor"),QColor("black"));
    }
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("PlaceShape")))){
        settings->setValue(QLatin1String("PlaceShape"),"roundedRect");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("PlaceColor"))).isValid()==false){
        settings->setValue(QLatin1String("PlaceColor"),QColor("white"));
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("PlaceBorderColor"))).isValid()==false){
        settings->setValue(QLatin1String("PlaceBorderColor"),QColor("black"));
    }
    if(qvariant_cast<QString>(settings->value(QLatin1String("PlaceBorderWidth")))==""){
        settings->setValue(QLatin1String("PlaceBorderWidth"),2);
    }
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("LabelSize")))){
        settings->setValue(QLatin1String("LabelSize"),"12");
    }
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))).isValid()==false){
        settings->setValue(QLatin1String("LabelColor"),QColor("black"));
    }

    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i)!="rect"){
            this->_ui.PlaceShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
        }else{
            break;
        }
    }

    this->_ui.InitPlaceColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("InitPlaceColor"))));

    this->_ui.PlaceShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("PlaceShape"))));
    this->_ui.PlaceColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("PlaceColor"))));
    this->_ui.PlaceBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("PlaceBorderColor"))));
    this->_ui.PlaceBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("PlaceBorderWidth"))));

    this->_ui.LabelSize->setValue(qvariant_cast<double>(settings->value(QLatin1String("LabelSize"))));
    this->_ui.LabelColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LabelColor"))));

    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.InitPlaceColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);

    QObject::connect(this->_ui.PlaceShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PlaceColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PlaceBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);
    QObject::connect(this->_ui.PlaceBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);

    QObject::connect(this->_ui.LabelSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LabelColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &SpenatEditorPlaceSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}
void SpenatEditorPlaceSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void SpenatEditorPlaceSettingsPage::_refreshPreview()
{
    if(this->_comQML != nullptr){

        graphlayout::ComCreateDeclGraph comGraph(this->_graph,this->_model);
        spenatGraphLoadSettings(&comGraph, this->_comQML);
        //place
        comGraph.setInitPlaceColor(this->_ui.InitPlaceColor->color());

        comGraph.setPlaceShape(graphlayout::ShapeEnumFromString(this->_ui.PlaceShape->currentText()));
        comGraph.setPlaceColor(this->_ui.PlaceColor->color());
        comGraph.setPlaceBorderColor(this->_ui.PlaceBorderColor->color());
        comGraph.setPlaceBorderWidth(this->_ui.PlaceBorderWidth->value());

        comGraph.setPlaceLabelSize(this->_ui.LabelSize->value());
        comGraph.setPlaceLabelColor(this->_ui.LabelColor->color());

        QString errorString;
        bool result = comGraph.execute(&errorString);
        if(!result)
        {
            qWarning()<<"error during layout graph creation for place settings page: "<<errorString;
            return;
        }
        result = this->_comQML->layout(&errorString);
        if(!result)
        {
            qWarning()<<"error during layouting of preview spenat graph for place settings page: "<<errorString;
            return;
        }
    }
}

void SpenatEditorPlaceSettingsPage::apply(){
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup(QLatin1String("Place"));

    settings->setValue(QLatin1String("InitPlaceColor"),this->_ui.InitPlaceColor->color());

    settings->setValue(QLatin1String("PlaceShape"),this->_ui.PlaceShape->currentText());
    settings->setValue(QLatin1String("PlaceColor"),this->_ui.PlaceColor->color());
    settings->setValue(QLatin1String("PlaceBorderColor"),this->_ui.PlaceBorderColor->color());
    settings->setValue(QLatin1String("PlaceBorderWidth"),this->_ui.PlaceBorderWidth->value());

    settings->setValue(QLatin1String("LabelSize"),this->_ui.LabelSize->value());
    settings->setValue(QLatin1String("LabelColor"),this->_ui.LabelColor->color());

    settings->endGroup();
    settings->endGroup();
    emit this->appliedSettings();
    return;
}

void SpenatEditorPlaceSettingsPage::finish()
{
    // Do nothing

    return;
}

bool SpenatEditorPlaceSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


