#include "statemachineeditorhighlightsettingspage.hpp"
#include "statemachineeditorpreviewgraph.hpp"

#include <QQuickView>
#include <QTimer>

namespace statemachineeditor {

class StateMachineEditorHighlightSettingsWidget::Private
{
    friend class StateMachineEditorHighlightSettingsWidget;
};

StateMachineEditorHighlightSettingsWidget::StateMachineEditorHighlightSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

StateMachineEditorHighlightSettingsWidget::~StateMachineEditorHighlightSettingsWidget()
{
    delete _d;
}

// ------------------ StateMachineEditorHighlightSettingsPage
StateMachineEditorHighlightSettingsPage::StateMachineEditorHighlightSettingsPage()
{
    this->setId(Constants::statemachine_SETTINGS_ID);
    this->setDisplayName(tr("Highlighter"));
    this->setCategory(Constants::statemachine_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::statemachine_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::statemachine_SETTINGS_CATEGORY_ICON));
}

StateMachineEditorHighlightSettingsPage::~StateMachineEditorHighlightSettingsPage()
{
    delete _graph;
    delete _model;
}

QWidget* StateMachineEditorHighlightSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new StateMachineEditorHighlightSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));

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
    settings->endGroup();

    settings->beginGroup(QLatin1String("Highlight"));
    this->_ui.StateColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateColor"))));
    this->_ui.StateBorderColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("StateBorderColor"))));
    this->_ui.StateBorderWidth->setValue(qvariant_cast<double>(settings->value(QLatin1String("StateBorderWidth"))));
    this->_ui.TransitionColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TransitionColor"))));
    settings->endGroup();

    settings->endGroup();

    QObject::connect(this->_ui.StateColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.StateBorderColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.StateBorderWidth, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::valueChanged),
                     this, &StateMachineEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(this->_ui.TransitionColor, &Utils::QtColorButton::colorChanged,
                     this, &StateMachineEditorHighlightSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh, &QPushButton::clicked,
                     this, &StateMachineEditorHighlightSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer,&QTimer::timeout,
                     this, &StateMachineEditorHighlightSettingsPage::refreshPreview);
    QObject::connect(timer, &QTimer::timeout,
                     timer, &QTimer::deleteLater);
    timer->start(1000);

    return this->_widget;
}

void StateMachineEditorHighlightSettingsPage::_initGraph(){
    if(this->_graph->nodes().isEmpty()==true){
        previewGraph(this->_model);
    }
}
void StateMachineEditorHighlightSettingsPage::_refreshPreview(){
    if(this->_comQML != nullptr){
        graphlayout::ComCreateStateMachineGraph comGraph(this->_graph,this->_model);
        stateMachineGraphLoadSettings(&comGraph, this->_comQML);
        //highlight
        comGraph.setHighlightStateColor(this->_ui.StateColor->color());
        comGraph.setHighlightStateBorderColor(this->_ui.StateBorderColor->color());
        comGraph.setHighlightStateBorderWidth(this->_ui.StateBorderWidth->value());
        comGraph.setHighlightTransitionColor(this->_ui.TransitionColor->color());

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

void StateMachineEditorHighlightSettingsPage::apply()
{
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("StateMachineEditor"));
    settings->beginGroup(QLatin1String("Highlight"));
    settings->setValue(QLatin1String("StateColor"),this->_ui.StateColor->color());
    settings->setValue(QLatin1String("StateBorderColor"),this->_ui.StateBorderColor->color());
    settings->setValue(QLatin1String("StateBorderWidth"),this->_ui.StateBorderWidth->value());
    settings->setValue(QLatin1String("TransitionColor"),this->_ui.TransitionColor->color());
    settings->endGroup();
    settings->endGroup();
    emit appliedSettings();
    return;
}

void StateMachineEditorHighlightSettingsPage::finish()
{
    // Do nothing

    return;
}

bool StateMachineEditorHighlightSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}//end namespace statemachineeditor


