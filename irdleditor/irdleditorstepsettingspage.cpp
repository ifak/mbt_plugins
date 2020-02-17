#include "irdleditorstepsettingspage.hpp"
#include "irdleditorpreviewgraph.hpp"

namespace irdleditor {
namespace Internal {

class IrdlEditorStepSettingsWidget::Private
{
  friend class IrdlEditorStepSettingsWidget;
};

IrdlEditorStepSettingsWidget::IrdlEditorStepSettingsWidget(QWidget *parent)
  : QWidget(parent), _d(new Private())
{}

IrdlEditorStepSettingsWidget::~IrdlEditorStepSettingsWidget(){
  delete _d;
}

// ------------------ IrdlEditorStepSettingsPage
IrdlEditorStepSettingsPage::IrdlEditorStepSettingsPage(){
  this->setId(Constants::IRDL_SETTINGS_ID);
  this->setDisplayName(tr("Steps"));
  this->setCategory(Constants::IRDL_SETTINGS_CATEGORY);
  this->setDisplayCategory(tr(Constants::IRDL_SETTINGS_TR_CATEGORY));
  this->setCategoryIcon(QLatin1String(Constants::IRDL_SETTINGS_CATEGORY_ICON));
}

IrdlEditorStepSettingsPage::~IrdlEditorStepSettingsPage(){
  delete _model;
//  delete _mscQuickWidget;
}

QWidget* IrdlEditorStepSettingsPage::createPage(QWidget *parent){
  this->_widget = new IrdlEditorStepSettingsWidget(parent);
  this->_ui.setupUi(this->_widget);

  this->_initGraph();
  this->_mscQuickWidget = new graphlayout::MscQuickWidget(this->_model->sequences().first());
  this->_ui.PreviewLayout->addWidget(this->_mscQuickWidget);

  QSettings * settings = Core::ICore::settings();
  settings->beginGroup(QLatin1String("IrdlEditor"));
  settings->beginGroup(QLatin1String("Steps"));

  if(qvariant_cast<QColor>(settings->value(QLatin1String("StatementColor"))).isValid()==false){
    settings->setValue(QLatin1String("StatementColor"),QColor("white"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("StatementTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("StatementTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("TimerColor"))).isValid()==false){
    settings->setValue(QLatin1String("TimerColor"),QColor("white"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("TimerTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("TimerTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("TimeoutColor"))).isValid()==false){
    settings->setValue(QLatin1String("TimeoutColor"),QColor("white"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("TimeoutTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("TimeoutTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("SleeptimerColor"))).isValid()==false){
    settings->setValue(QLatin1String("SleeptimerColor"),QColor("white"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("SleeptimerTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("SleeptimerTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("MessageColor"))).isValid()==false){
    settings->setValue(QLatin1String("MessageColor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("MessageTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("MessageTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("CheckmessageColor"))).isValid()==false){
    settings->setValue(QLatin1String("CheckmessageColor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("CheckmessageTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("CheckmessageTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("FragmentColor"))).isValid()==false){
    settings->setValue(QLatin1String("FragmentColor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("FragmentTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("FragmentTextcolor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("RegionColor"))).isValid()==false){
    settings->setValue(QLatin1String("RegionColor"),QColor("black"));
  }
  if(qvariant_cast<QColor>(settings->value(QLatin1String("RegionTextcolor"))).isValid()==false){
    settings->setValue(QLatin1String("RegionTextcolor"),QColor("black"));
  }

  this->_ui.StatementColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("StatementColor"))));
  this->_ui.StatementTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("StatementTextcolor"))));
  this->_ui.TimerColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimerColor"))));
  this->_ui.TimerTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimerTextcolor"))));
  this->_ui.TimeoutColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimeoutColor"))));
  this->_ui.TimeoutTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("TimeoutTextcolor"))));
  this->_ui.SleeptimerColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("SleeptimerColor"))));
  this->_ui.SleeptimerTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("SleeptimerTextcolor"))));
  this->_ui.MessageColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("MessageColor"))));
  this->_ui.MessageTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("MessageTextcolor"))));
  this->_ui.CheckmessageColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("CheckmessageColor"))));
  this->_ui.CheckmessageTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("CheckmessageTextcolor"))));
  this->_ui.FragmentColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("FragmentColor"))));
  this->_ui.FragmentTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("FragmentTextcolor"))));
  this->_ui.RegionColor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionColor"))));
  this->_ui.RegionTextcolor->setColor(qvariant_cast<QColor>(settings->value(QLatin1String("RegionTextcolor"))));

  settings->endGroup();
  settings->endGroup();

  QObject::connect(this->_ui.StatementColor,          &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.StatementTextcolor,      &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.TimerColor,              &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.TimerTextcolor,          &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.TimeoutColor,            &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.TimeoutTextcolor,        &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.SleeptimerColor,         &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.SleeptimerTextcolor,     &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.MessageColor,            &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.MessageTextcolor,        &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.CheckmessageColor,       &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.CheckmessageTextcolor,   &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.FragmentColor,           &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.FragmentTextcolor,       &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.RegionColor,             &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(this->_ui.RegionTextcolor,         &Utils::QtColorButton::colorChanged,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);

  QObject::connect(this->_ui.refresh,                 &QPushButton::clicked,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);

  QTimer* timer = new QTimer();
  QObject::connect(timer,                             &QTimer::timeout,
                   this,                              &IrdlEditorStepSettingsPage::refreshPreview);
  QObject::connect(timer,                             &QTimer::timeout,
                   timer,                             &QTimer::deleteLater);
  timer->start(1000);

  return this->_widget;
}
void IrdlEditorStepSettingsPage::_initGraph(){
  if (this->_model->sequences().length() == 0){
    previewGraph(this->_model);
  }
}
void IrdlEditorStepSettingsPage::_refreshPreview(){
  if (this->_mscQuickWidget != nullptr){
    irdlGraphLoadSettings(this->_mscQuickWidget);
    this->_mscQuickWidget->setStatementColor(this->_ui.StatementColor->color());
    this->_mscQuickWidget->setStatementTextColor(this->_ui.StatementTextcolor->color());
    this->_mscQuickWidget->setTimerColor(this->_ui.TimerColor->color());
    this->_mscQuickWidget->setTimerTextColor(this->_ui.TimerTextcolor->color());
    this->_mscQuickWidget->setTimeoutColor(this->_ui.TimeoutColor->color());
    this->_mscQuickWidget->setTimeoutTextColor(this->_ui.TimeoutTextcolor->color());
    this->_mscQuickWidget->setSleeptimerColor(this->_ui.SleeptimerColor->color());
    this->_mscQuickWidget->setSleeptimerTextColor(this->_ui.SleeptimerTextcolor->color());
    this->_mscQuickWidget->setMessageColor(this->_ui.MessageColor->color());
    this->_mscQuickWidget->setMessageTextColor(this->_ui.MessageTextcolor->color());
    this->_mscQuickWidget->setCheckmessageColor(this->_ui.CheckmessageColor->color());
    this->_mscQuickWidget->setCheckmessageTextColor(this->_ui.CheckmessageTextcolor->color());
    this->_mscQuickWidget->setFragmentColor(this->_ui.FragmentColor->color());
    this->_mscQuickWidget->setFragmentTextColor(this->_ui.FragmentTextcolor->color());
    this->_mscQuickWidget->setRegionColor(this->_ui.RegionColor->color());
    this->_mscQuickWidget->setRegionTextColor(this->_ui.RegionTextcolor->color());

    QString errorString;
    bool result = this->_mscQuickWidget->layout(&errorString);
    if(!result){
      qWarning()<<"error during layout of preview msc for 'Component and Background' settings page: "<<errorString;
      return;
    }
  }
}

void IrdlEditorStepSettingsPage::apply(){
  QSettings * settings = Core::ICore::settings();
  settings->beginGroup(QLatin1String("IrdlEditor"));
  settings->beginGroup(QLatin1String("Steps"));

  settings->setValue(QLatin1String("StatementColor"),this->_ui.StatementColor->color());
  settings->setValue(QLatin1String("StatementTextcolor"),this->_ui.StatementTextcolor->color());
  settings->setValue(QLatin1String("TimerColor"),this->_ui.TimerColor->color());
  settings->setValue(QLatin1String("TimerTextcolor"),this->_ui.TimerTextcolor->color());
  settings->setValue(QLatin1String("TimeoutColor"),this->_ui.TimeoutColor->color());
  settings->setValue(QLatin1String("TimeoutTextcolor"),this->_ui.TimeoutTextcolor->color());
  settings->setValue(QLatin1String("SleeptimerColor"),this->_ui.SleeptimerColor->color());
  settings->setValue(QLatin1String("SleeptimerTextcolor"),this->_ui.SleeptimerTextcolor->color());
  settings->setValue(QLatin1String("MessageColor"),this->_ui.MessageColor->color());
  settings->setValue(QLatin1String("MessageTextcolor"),this->_ui.MessageTextcolor->color());
  settings->setValue(QLatin1String("CheckmessageColor"),this->_ui.CheckmessageColor->color());
  settings->setValue(QLatin1String("CheckmessageTextcolor"),this->_ui.CheckmessageTextcolor->color());
  settings->setValue(QLatin1String("FragmentColor"),this->_ui.FragmentColor->color());
  settings->setValue(QLatin1String("FragmentTextcolor"),this->_ui.FragmentTextcolor->color());
  settings->setValue(QLatin1String("RegionColor"),this->_ui.RegionColor->color());
  settings->setValue(QLatin1String("RegionTextcolor"),this->_ui.RegionTextcolor->color());

  settings->endGroup();
  settings->endGroup();

  emit this->appliedSettings();
  return;
}

void IrdlEditorStepSettingsPage::finish(){
  // Do nothing

  return;
}

bool IrdlEditorStepSettingsPage::matches(const QString& s) const{
  return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace irdleditor


