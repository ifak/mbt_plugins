#include "spenateditorarcsettingspage.hpp"
#include "spenateditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace spenateditor {
namespace Internal {

class SpenatEditorArcSettingsWidget::Private
{
    friend class SpenatEditorArcSettingsWidget;
};

SpenatEditorArcSettingsWidget::SpenatEditorArcSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

SpenatEditorArcSettingsWidget::~SpenatEditorArcSettingsWidget()
{
    delete _d;
}

// ------------------ SpenatEditorArcSettingsPage
SpenatEditorArcSettingsPage::SpenatEditorArcSettingsPage()
{
    this->setId(Constants::SPENAT_SETTINGS_ID);
    this->setDisplayName(tr("Arcs"));
    this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorArcSettingsPage::~SpenatEditorArcSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* SpenatEditorArcSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new SpenatEditorArcSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();

    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ArrowEnumToString((graphlayout::ArrowEnum)i)!="Arrow"){
            this->_ui.ArcArrowtype->addItem(graphlayout::ArrowEnumToString((graphlayout::ArrowEnum)i));
        }else{
            break;
        }
    }

    settings->beginGroup(QLatin1String("Arc"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("ArcColor"))).isValid()==false)
        settings->setValue(QLatin1String("ArcColor"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("ArcArrowtype")))=="")
        settings->setValue(QLatin1String("ArcArrowtype"),"Arrow");
    if(qvariant_cast<QString>(settings->value(QLatin1String("ArcArrowsize")))=="")
        settings->setValue(QLatin1String("ArcArrowsize"),15);
    if(qvariant_cast<QString>(settings->value(QLatin1String("ArcLineWidth")))=="")
        settings->setValue(QLatin1String("ArcLineWidth"),2);

    this->_ui.ArcColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("ArcColor"))));
    this->_ui.ArcArrowtype->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("ArcArrowtype"))));
    this->_ui.ArcArrowsize->setValue(qvariant_cast<int>(settings->value(QLatin1String("ArcArrowsize"))));
    this->_ui.ArcLineWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("ArcLineWidth"))));
    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.ArcColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorArcSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ArcArrowtype, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &SpenatEditorArcSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ArcArrowsize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorArcSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ArcLineWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorArcSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &SpenatEditorArcSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &SpenatEditorArcSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void SpenatEditorArcSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void SpenatEditorArcSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){

        graphlayout::ComCreateDeclGraph comGraph(this->_graph,this->_model);
        spenatGraphLoadSettings(&comGraph, this->_comQML);
        //arc
        comGraph.setArcColor(this->_ui.ArcColor->color());
        comGraph.setArcShape(graphlayout::ArrowEnumFromString(this->_ui.ArcArrowtype->currentText()));
        comGraph.setArcLineWidth(this->_ui.ArcLineWidth->value());
        comGraph.setArcArrowsize(this->_ui.ArcArrowsize->value());

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

void SpenatEditorArcSettingsPage::apply()
{
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup("Arc");
    settings->setValue(QLatin1String("ArcColor"),this->_ui.ArcColor->color());
    settings->setValue(QLatin1String("ArcArrowsize"),this->_ui.ArcArrowsize->value());
    settings->setValue(QLatin1String("ArcLineWidth"),this->_ui.ArcLineWidth->value());
    settings->setValue(QLatin1String("ArcArrowtype"),this->_ui.ArcArrowtype->currentText());
    settings->endGroup();
    settings->endGroup();

    emit this->appliedSettings();
    return;
}

void SpenatEditorArcSettingsPage::finish()
{
    // Do nothing

    return;
}

bool SpenatEditorArcSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


