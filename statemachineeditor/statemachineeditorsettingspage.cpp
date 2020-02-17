#include "statemachineeditorsettingspage.hpp"

#include "statemachineeditorconstants.hpp"

namespace statemachineeditor {
namespace Internal {

class StateMachineEditorSettingsWidget::Private
{
  friend class StateMachineEditorSettingsWidget;
};

StateMachineEditorSettingsWidget::StateMachineEditorSettingsWidget(QWidget *parent)
  : QWidget(parent), _d(new Private())
{}

StateMachineEditorSettingsWidget::~StateMachineEditorSettingsWidget()
{
  delete _d;
}

// ------------------ StateMachineEditorSettingsPage
StateMachineEditorSettingsPage::StateMachineEditorSettingsPage()
{
  this->setId(Constants::statemachine_SETTINGS_ID);
  this->setDisplayName(tr("statemachine Editor"));
  this->setCategory(Constants::statemachine_SETTINGS_CATEGORY);
  this->setDisplayCategory(tr(Constants::statemachine_SETTINGS_TR_CATEGORY));
  this->setCategoryIcon(QLatin1String(Constants::statemachine_SETTINGS_CATEGORY_ICON));
}

StateMachineEditorSettingsPage::~StateMachineEditorSettingsPage()
{
  // Do nothing
}

QWidget* StateMachineEditorSettingsPage::createPage(QWidget *parent)
{
  this->_widget = new StateMachineEditorSettingsWidget(parent);

  return this->_widget;
}

void StateMachineEditorSettingsPage::apply()
{

  return;
}

void StateMachineEditorSettingsPage::finish()
{
  // Do nothing

  return;
}

bool StateMachineEditorSettingsPage::matches(const QString& s) const
{
  return this->_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}// end namespace Internal
}//end namespace statemachineeditor


