#include "irdleditorsettingspage.hpp"

#include "irdleditorconstants.hpp"

namespace irdleditor {
namespace Internal {

class IrdlEditorSettingsWidget::Private
{
  friend class IrdlEditorSettingsWidget;
};

IrdlEditorSettingsWidget::IrdlEditorSettingsWidget(QWidget *parent)
  : QWidget(parent), _d(new Private())
{}

IrdlEditorSettingsWidget::~IrdlEditorSettingsWidget()
{
  delete _d;
}

// ------------------ IrdlEditorSettingsPage
IrdlEditorSettingsPage::IrdlEditorSettingsPage()
{
  this->setId(Constants::IRDL_SETTINGS_ID);
  this->setDisplayName(tr("Irdl Editor"));
  this->setCategory(Constants::IRDL_SETTINGS_CATEGORY);
  this->setDisplayCategory(tr(Constants::IRDL_SETTINGS_TR_CATEGORY));
  this->setCategoryIcon(QLatin1String(Constants::IRDL_SETTINGS_CATEGORY_ICON));
}

IrdlEditorSettingsPage::~IrdlEditorSettingsPage()
{
  // Do nothing
}

QWidget* IrdlEditorSettingsPage::createPage(QWidget *parent)
{
  this->_widget = new IrdlEditorSettingsWidget(parent);

  return this->_widget;
}

void IrdlEditorSettingsPage::apply()
{

  return;
}

void IrdlEditorSettingsPage::finish()
{
  // Do nothing

  return;
}

bool IrdlEditorSettingsPage::matches(const QString& s) const
{
  return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace irdleditor


