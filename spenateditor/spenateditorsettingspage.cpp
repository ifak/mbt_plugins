#include "spenateditorsettingspage.hpp"

#include "spenateditorconstants.hpp"

namespace spenateditor {
namespace Internal {

class SpenatEditorSettingsWidget::Private
{
  friend class SpenatEditorSettingsWidget;
};

SpenatEditorSettingsWidget::SpenatEditorSettingsWidget(QWidget *parent)
  : QWidget(parent), _d(new Private())
{}

SpenatEditorSettingsWidget::~SpenatEditorSettingsWidget()
{
  delete _d;
}

// ------------------ SpenatEditorSettingsPage
SpenatEditorSettingsPage::SpenatEditorSettingsPage()
{
  this->setId(Constants::SPENAT_SETTINGS_ID);
  this->setDisplayName(tr("Spenat Editor"));
  this->setCategory(Constants::SPENAT_SETTINGS_CATEGORY);
  this->setDisplayCategory(tr(Constants::SPENAT_SETTINGS_TR_CATEGORY));
  this->setCategoryIcon(QLatin1String(Constants::SPENAT_SETTINGS_CATEGORY_ICON));
}

SpenatEditorSettingsPage::~SpenatEditorSettingsPage()
{
  // Do nothing
}

QWidget* SpenatEditorSettingsPage::createPage(QWidget *parent)
{
  this->_widget = new SpenatEditorSettingsWidget(parent);

  return this->_widget;
}

void SpenatEditorSettingsPage::apply()
{

  return;
}

void SpenatEditorSettingsPage::finish()
{
  // Do nothing

  return;
}

bool SpenatEditorSettingsPage::matches(const QString& s) const
{
  return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace spenateditor


