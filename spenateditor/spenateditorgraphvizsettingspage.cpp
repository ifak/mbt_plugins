#include "spenateditorgraphvizsettingspage.hpp"
#include "spenateditorpreviewgraph.hpp"
#include <QTimer>

namespace spenateditor {
namespace Internal {

class SpenatEditorGraphvizSettingsWidget::Private
{
    friend class SpenatEditorGraphvizSettingsWidget;
};

SpenatEditorGraphvizSettingsWidget::SpenatEditorGraphvizSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

SpenatEditorGraphvizSettingsWidget::~SpenatEditorGraphvizSettingsWidget()
{
    delete _d;
}

// ------------------ SpenatEditorGraphvizSettingsPage
SpenatEditorGraphvizSettingsPage::SpenatEditorGraphvizSettingsPage()
{
    this->setId(Constants::SPENAT_SETTINGS_ID);
    this->setDisplayName(tr("Graphviz Settings"));
    this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorGraphvizSettingsPage::~SpenatEditorGraphvizSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* SpenatEditorGraphvizSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new SpenatEditorGraphvizSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();
    this->_comQML = new graphlayout::GraphQuickWidget(this->_graph,graphlayout::Dot);
    this->_ui.PreviewLayout->addWidget(this->_comQML);
    QObject::connect(this->_ui.Preview, &QGroupBox::destroyed,this->_comQML,&graphlayout::GraphQuickWidget::deleteLater);
    this->_comQML->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->_comQML->show();

    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup(QLatin1String("Graphviz"));

    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Spline")))){
        settings->setValue(QLatin1String("Spline"),"line");
    }
    if(""==qvariant_cast<QString>(settings->value(QLatin1String("Zoom")))){
        settings->setValue(QLatin1String("Zoom"),"zoom_disabled");
    }
    foreach (QObject* object, this->_ui.SplineBox->children()) {
        QRadioButton* button = qobject_cast<QRadioButton*>(object);
        if(button != nullptr){
            if(button->text()==qvariant_cast<QString>(settings->value(QLatin1String("Spline")))){
                button->setChecked(true);
            }else{
                button->setChecked(false);
            }
            QObject::connect(button, &QRadioButton::clicked, this, &SpenatEditorGraphvizSettingsPage::refreshPreview);
        }
    }
    foreach (QObject* object, this->_ui.AutoZoom->children()) {
        QRadioButton* button = qobject_cast<QRadioButton*>(object);
        if(button != nullptr){
            if(button->objectName()==qvariant_cast<QString>(settings->value(QLatin1String("Zoom")))){
                button->setChecked(true);
            }else{
                button->setChecked(false);
            }
        }
    }

    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.refresh, &QPushButton::clicked, this, &SpenatEditorGraphvizSettingsPage::refreshPreview);
    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout,
                     this, &SpenatEditorGraphvizSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void SpenatEditorGraphvizSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void SpenatEditorGraphvizSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateDeclGraph comGraph(this->_graph,this->_model);
        spenatGraphLoadSettings(&comGraph, this->_comQML);
        foreach (QObject* object, this->_ui.SplineBox->children()) {
            QRadioButton* button = qobject_cast<QRadioButton*>(object);
            if(button != nullptr){
                if(button->isChecked()==true){
                    comGraph.setSplines(graphlayout::SplineEnumFromString(button->text()));
                    break;
                }
            }
        }

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

void SpenatEditorGraphvizSettingsPage::apply(){
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("SpenatEditor"));
    settings->beginGroup(QLatin1String("Graphviz"));

    foreach (QObject* object, this->_ui.SplineBox->children()) {
        QRadioButton* button = qobject_cast<QRadioButton*>(object);
        if(button != nullptr){
            if(button->isChecked()==true){
                settings->setValue(QLatin1String("Spline"), button->text());
                break;
            }
        }
    }
    foreach (QObject* object, this->_ui.AutoZoom->children()) {
        QRadioButton* button = qobject_cast<QRadioButton*>(object);
        if(button != nullptr){
            if(button->isChecked()==true){
                settings->setValue(QLatin1String("Zoom"), button->objectName());
                break;
            }
        }
    }
    settings->endGroup();
    settings->endGroup();
    emit this->appliedSettings();
    return;
}

void SpenatEditorGraphvizSettingsPage::finish()
{
    // Do nothing

    return;
}

bool SpenatEditorGraphvizSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


