#include "mbtpsettingspage.hpp"
#include <QTimer>
#include <QFileDialog>

namespace mbtp {

class MbtpSettingsWidget::Private
{
    friend class MbtpSettingsWidget;
};

MbtpSettingsWidget::MbtpSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{}

MbtpSettingsWidget::~MbtpSettingsWidget()
{
    delete _d;
}

// ------------------ MbtpSettingsPage
MbtpSettingsPage::MbtpSettingsPage()
{
    this->setId(Constants::MBTP_SETTINGS);
    this->setDisplayName(tr("Mbtp Settings"));
    this->setCategory(Constants::MBTP_SETTINGS_CATEGORY);
    this->setDisplayCategory(tr(Constants::MBTP_SETTINGS_TR_CATEGORY));
    //this->setCategoryIcon(QLatin1String(Constants::MBTP_SETTINGS_CATEGORY_ICON));
}

MbtpSettingsPage::~MbtpSettingsPage()
{
}

QWidget* MbtpSettingsPage::createPage(QWidget *parent)
{
    this->_widget = new MbtpSettingsWidget(parent);
    this->_ui.setupUi(this->_widget);

    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("Mbtp"));

    this->_ui.MbtpPath->setText(qvariant_cast<QString>(settings->value(QLatin1String("MbtpPath"))));

    QObject::connect(this->_ui.MbtpPathBrowse, &QPushButton::clicked,
                     this, &MbtpSettingsPage::browseMbtpPath);

    settings->endGroup();

    return this->_widget;
}

void MbtpSettingsPage::apply(){
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("Mbtp"));

    settings->setValue(QLatin1String("MbtpPath"), this->_ui.MbtpPath->text());

    settings->endGroup();
    emit this->appliedSettings();
    return;
}

void MbtpSettingsPage::finish()
{
    // Do nothing

    return;
}

void MbtpSettingsPage::browseMbtpPath()
{
  QString filename = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(this), "Select Mbtp App", "/home", "Exe (*.exe)");
  if(filename != ""){
    this->_ui.MbtpPath->setText(filename);
  }
}

bool MbtpSettingsPage::matches(const QString& s) const
{
    return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}//end namespace mbtp


