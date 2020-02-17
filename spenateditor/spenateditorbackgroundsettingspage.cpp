#include "spenateditorbackgroundsettingspage.hpp"
#include "spenateditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace spenateditor {
namespace Internal {

class SpenatEditorBackgroundSettingsWidget::Private
{
    friend class SpenatEditorBackgroundSettingsWidget;
};

SpenatEditorBackgroundSettingsWidget::SpenatEditorBackgroundSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

SpenatEditorBackgroundSettingsWidget::~SpenatEditorBackgroundSettingsWidget()
{
    delete _d;
}

// ------------------ SpenatEditorBackgroundSettingsPage
SpenatEditorBackgroundSettingsPage::SpenatEditorBackgroundSettingsPage()
{
    this->setId(Constants::SPENAT_SETTINGS_ID);
    this->setDisplayName(tr("Title and Background"));
    this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorBackgroundSettingsPage::~SpenatEditorBackgroundSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* SpenatEditorBackgroundSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new SpenatEditorBackgroundSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();


    settings->beginGroup(QLatin1String("Background"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("BackgroundColor"))).isValid()==false)
        settings->setValue(QLatin1String("BackgroundColor"),QColor("white"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TitleColor"))).isValid()==false)
        settings->setValue(QLatin1String("TitleColor"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("TitleSize")))=="")
        settings->setValue(QLatin1String("TitleSize"),16);
    if(qvariant_cast<QString>(settings->value(QLatin1String("TitleShape")))=="")
        settings->setValue(QLatin1String("TitleShape"),graphlayout::ShapeEnumToString(graphlayout::Plaintext));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBorderColor"))).isValid()==false)
        settings->setValue(QLatin1String("TitleBorderColor"),QColor("black"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("TitleBorderWidth")))=="")
        settings->setValue(QLatin1String("TitleBorderWidth"),2);
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBackgroundColor"))).isValid()==false)
        settings->setValue(QLatin1String("TitleBackgroundColor"),QColor("white"));


    for(int i = 0; i>=0; i++){
        if(i==0 || graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i)!="rect"){
            this->_ui.TitleShape->addItem(graphlayout::ShapeEnumToString((graphlayout::ShapeEnum)i));
        }else{
            break;
        }
    }

    this->_ui.BackgroundColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BackgroundColor"))));
    this->_ui.TitleColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleColor"))));
    this->_ui.TitleSize->setValue(qvariant_cast<int>(settings->value(QLatin1String("TitleSize"))));
    this->_ui.TitleShape->setCurrentText(qvariant_cast<QString>(settings->value(QLatin1String("TitleShape"))));
    this->_ui.TitleBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBorderColor"))));
    this->_ui.TitleBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("TitleBorderWidth"))));
    this->_ui.TitleBackgroundColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TitleBackgroundColor"))));
    settings->endGroup();

    settings->endGroup();

    QObject::connect(this->_ui.BackgroundColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TitleColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TitleSize, static_cast<void (QSpinBox::*)(const QString&)>(&QSpinBox::valueChanged),
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TitleShape, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TitleBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TitleBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TitleBackgroundColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &SpenatEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void SpenatEditorBackgroundSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void SpenatEditorBackgroundSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateDeclGraph comGraph(this->_graph,this->_model);
        spenatGraphLoadSettings(&comGraph, this->_comQML);
        //background
        this->_comQML->setBackgroundColor(this->_ui.BackgroundColor->color());
        this->_comQML->setTitleColor(this->_ui.TitleColor->color());
        this->_comQML->setTitleSize(this->_ui.TitleSize->value());
        this->_comQML->setTitleShape(graphlayout::ShapeEnumFromString(this->_ui.TitleShape->currentText()));
        this->_comQML->setTitleBorderColor(this->_ui.TitleBorderColor->color());
        this->_comQML->setTitleBorderWidth(this->_ui.TitleBorderWidth->value());
        this->_comQML->setTitleBackgroundColor(this->_ui.TitleBackgroundColor->color());

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

void SpenatEditorBackgroundSettingsPage::apply()
{
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup(QLatin1String("Background"));
    settings->setValue(QLatin1String("BackgroundColor"),this->_ui.BackgroundColor->color());
    settings->setValue(QLatin1String("TitleColor"),this->_ui.TitleColor->color());
    settings->setValue(QLatin1String("TitleSize"),this->_ui.TitleSize->value());
    settings->setValue(QLatin1String("TitleShape"),this->_ui.TitleShape->currentText());
    settings->setValue(QLatin1String("TitleBorderColor"),this->_ui.TitleBorderColor->color());
    settings->setValue(QLatin1String("TitleBorderWidth"),this->_ui.TitleBorderWidth->value());
    settings->setValue(QLatin1String("TitleBackgroundColor"),this->_ui.TitleBackgroundColor->color());
    settings->endGroup();
    settings->endGroup();

    emit this->appliedSettings();
    return;
}

void SpenatEditorBackgroundSettingsPage::finish()
{
    // Do nothing

    return;
}

bool SpenatEditorBackgroundSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


