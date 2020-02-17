#include "spenateditorhighlightsettingspage.hpp"
#include "spenateditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace spenateditor {
namespace Internal {

class SpenatEditorHighlightSettingsWidget::Private
{
    friend class SpenatEditorHighlightSettingsWidget;
};

SpenatEditorHighlightSettingsWidget::SpenatEditorHighlightSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

SpenatEditorHighlightSettingsWidget::~SpenatEditorHighlightSettingsWidget()
{
    delete _d;
}

// ------------------ SpenatEditorHighlightSettingsPage
SpenatEditorHighlightSettingsPage::SpenatEditorHighlightSettingsPage()
{
    this->setId(Constants::SPENAT_SETTINGS_ID);
    this->setDisplayName(tr("Highlighter"));
    this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorHighlightSettingsPage::~SpenatEditorHighlightSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* SpenatEditorHighlightSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new SpenatEditorHighlightSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));

    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);

    this->_ui.PreviewLayout->addWidget(this->_comQML);

    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);

    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();


    settings->beginGroup(QLatin1String("Highlight"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("StateColor"))).isValid()==false)
        settings->setValue(QLatin1String("StateColor"),QColor("green"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("StateBorderColor"))).isValid()==false)
        settings->setValue(QLatin1String("StateBorderColor"),QColor("green"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("StateBorderWidth")))=="")
        settings->setValue(QLatin1String("StateBorderWidth"),2);
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))).isValid()==false)
        settings->setValue(QLatin1String("TransitionColor"),QColor("green"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionBorderColor"))).isValid()==false)
        settings->setValue(QLatin1String("TransitionBorderColor"),QColor("green"));
    if(qvariant_cast<QString>(settings->value(QLatin1String("TransitionBorderWidth")))=="")
        settings->setValue(QLatin1String("TransitionBorderWidth"),2);

    this->_ui.StateColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateColor"))));
    this->_ui.StateBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateBorderColor"))));
    this->_ui.StateBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("StateBorderWidth"))));
    this->_ui.TransitionColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))));
    this->_ui.TransitionBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionBorderColor"))));
    this->_ui.TransitionBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("TransitionBorderWidth"))));
    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.StateColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.StateBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.StateBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &SpenatEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void SpenatEditorHighlightSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void SpenatEditorHighlightSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateDeclGraph comGraph(this->_graph,this->_model);
        spenatGraphLoadSettings(&comGraph, this->_comQML);
        //highlight
        comGraph.setHighlightPlaceColor(this->_ui.StateColor->color());
        comGraph.setHighlightPlaceBorderColor(this->_ui.StateBorderColor->color());
        comGraph.setHighlightPlaceBorderWidth(this->_ui.StateBorderWidth->value());
        comGraph.setHighlightTransitionColor(this->_ui.TransitionColor->color());
        comGraph.setHighlightTransitionBorderColor(this->_ui.TransitionBorderColor->color());
        comGraph.setHighlightTransitionBorderWidth(this->_ui.TransitionBorderWidth->value());

        QString errorString;
        bool result = comGraph.execute(&errorString);
        if(!result)
        {
            qWarning()<<"error during layout graph creation for highlight settings page: "<<errorString;
            return;
        }
        result = this->_comQML->layout(&errorString);
        if(!result)
        {
            qWarning()<<"error during layouting of preview spenat graph for highlight settings page: "<<errorString;
            return;
        }
    }
}

void SpenatEditorHighlightSettingsPage::apply()
{
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup(QLatin1String("Highlight"));
    settings->setValue(QLatin1String("StateColor"),this->_ui.StateColor->color());
    settings->setValue(QLatin1String("StateBorderColor"),this->_ui.StateBorderColor->color());
    settings->setValue(QLatin1String("StateBorderWidth"),this->_ui.StateBorderWidth->value());
    settings->setValue(QLatin1String("TransitionColor"),this->_ui.TransitionColor->color());
    settings->setValue(QLatin1String("TransitionBorderColor"),this->_ui.TransitionBorderColor->color());
    settings->setValue(QLatin1String("TransitionBorderWidth"),this->_ui.TransitionBorderWidth->value());
    settings->endGroup();
    settings->endGroup();
    emit this->appliedSettings();
    return;
}

void SpenatEditorHighlightSettingsPage::finish()
{
    // Do nothing

    return;
}

bool SpenatEditorHighlightSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


