#include "irdleditorbackgroundsettingspage.hpp"
#include "irdleditorpreviewgraph.hpp"

namespace irdleditor {
namespace Internal {

class IrdlEditorBackgroundSettingsWidget::Private
{
    friend class IrdlEditorBackgroundSettingsWidget;
};

IrdlEditorBackgroundSettingsWidget::IrdlEditorBackgroundSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

IrdlEditorBackgroundSettingsWidget::~IrdlEditorBackgroundSettingsWidget(){
    delete _d;
}

// ------------------ IrdlEditorBackgroundSettingsPage
IrdlEditorBackgroundSettingsPage::IrdlEditorBackgroundSettingsPage(){
    this->setId(Constants::IRDL_SETTINGS_ID);
    this->setDisplayName(tr("Components and Background"));
    this->setCategory(Constants::IRDL_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::IRDL_SETTINGS_TR_CATEGORY));
    this->setCategoryIcon(QLatin1String(Constants::IRDL_SETTINGS_CATEGORY_ICON));
}

IrdlEditorBackgroundSettingsPage::~IrdlEditorBackgroundSettingsPage(){
    delete _model;
//    delete _mscQuickWidget;
}

QWidget* IrdlEditorBackgroundSettingsPage::createPage(QWidget *parent){
    this->_widget = new IrdlEditorBackgroundSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    this->_initGraph();
    this->_mscQuickWidget = new graphlayout::MscQuickWidget(this->_model->sequences().first());
    this->_ui.PreviewLayout->addWidget(this->_mscQuickWidget);

    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("IrdlEditor"));
    settings->beginGroup(QLatin1String("Background"));

    if(qvariant_cast<QColor>(settings->value(QLatin1String("ComponentColor"))).isValid()==false)
        settings->setValue(QLatin1String("ComponentColor"),QColor("white"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("ComponentTextcolor"))).isValid()==false)
        settings->setValue(QLatin1String("ComponentTextcolor"),QColor("black"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("LifelineColor"))).isValid()==false)
        settings->setValue(QLatin1String("LifelineColor"),QColor("black"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("BackgroundColor"))).isValid()==false)
        settings->setValue(QLatin1String("BackgroundColor"),QColor("white"));
    if(qvariant_cast<QColor>(settings->value(QLatin1String("HighlightColor"))).isValid()==false)
        settings->setValue(QLatin1String("HighlightColor"),QColor("red"));

    this->_ui.ComponentColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("ComponentColor"))));
    this->_ui.ComponentTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("ComponentTextcolor"))));
    this->_ui.LifelineColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("LifelineColor"))));
    this->_ui.BackgroundColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("BackgroundColor"))));
    this->_ui.HighlightColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("HighlightColor"))));

    settings->endGroup();
    settings->endGroup();

    QObject::connect(this->_ui.ComponentColor,        &Utils::QtColorButton::colorChanged,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.ComponentTextcolor,    &Utils::QtColorButton::colorChanged,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.LifelineColor,         &Utils::QtColorButton::colorChanged,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.BackgroundColor,       &Utils::QtColorButton::colorChanged,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(this->_ui.HighlightColor,        &Utils::QtColorButton::colorChanged,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);

    QObject::connect(this->_ui.refresh,               &QPushButton::clicked,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);

    QTimer* timer = new QTimer();
    QObject::connect(timer,                           &QTimer::timeout,
                     this,                            &IrdlEditorBackgroundSettingsPage::refreshPreview);
    QObject::connect(timer,                           &QTimer::timeout,
                     timer,                           &QTimer::deleteLater);
    timer->start(2000);

    return this->_widget;
}

void IrdlEditorBackgroundSettingsPage::_initGraph(){
    if (this->_model->sequences().length() == 0){
        previewGraph(this->_model);
    }
}
void IrdlEditorBackgroundSettingsPage::_refreshPreview(){
    if (this->_mscQuickWidget != nullptr){
        irdlGraphLoadSettings(this->_mscQuickWidget);
        this->_mscQuickWidget->setComponentColor(this->_ui.ComponentColor->color());
        this->_mscQuickWidget->setComponentTextColor(this->_ui.ComponentTextcolor->color());
        this->_mscQuickWidget->setLifelineColor(this->_ui.LifelineColor->color());
        this->_mscQuickWidget->setBackgroundColor(this->_ui.BackgroundColor->color());
        this->_mscQuickWidget->setHighlightColor(this->_ui.HighlightColor->color());

        QString errorString;
        bool result = this->_mscQuickWidget->layout(&errorString);
        if(!result){
            qWarning()<<"error during layout of preview msc for 'Component and Background' settings page: "<<errorString;
            return;
        }
        //this->_mscQuickWidget->zoomFitWidth();
    }
}

void IrdlEditorBackgroundSettingsPage::apply(){
    QSettings* settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("IrdlEditor"));
    settings->beginGroup(QLatin1String("Background"));
    settings->setValue(QLatin1String("ComponentColor"),this->_ui.ComponentColor->color());
    settings->setValue(QLatin1String("ComponentTextcolor"),this->_ui.ComponentTextcolor->color());
    settings->setValue(QLatin1String("LifelineColor"),this->_ui.LifelineColor->color());
    settings->setValue(QLatin1String("BackgroundColor"),this->_ui.BackgroundColor->color());
    settings->setValue(QLatin1String("HighlightColor"),this->_ui.HighlightColor->color());
    settings->endGroup();
    settings->endGroup();

    emit this->appliedSettings();
    return;
}

void IrdlEditorBackgroundSettingsPage::finish(){
    // Do nothing

    return;
}

bool IrdlEditorBackgroundSettingsPage::matches(const QString& s) const{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace irdleditor


