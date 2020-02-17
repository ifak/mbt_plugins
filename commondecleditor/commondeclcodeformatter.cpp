#include "commondeclcodeformatter.hpp"

#include <texteditor/tabsettings.h>

#include <mobata/memory_leak_start.hpp>

namespace commondecleditor {

CommonDeclCodeFormatter::CommonDeclCodeFormatter()
{}

CommonDeclCodeFormatter::CommonDeclCodeFormatter(const TextEditor::TabSettings &tabSettings)
{
  setTabSize(tabSettings.m_tabSize);
  setIndentSize(tabSettings.m_indentSize);
}

CommonDeclCodeFormatter::~CommonDeclCodeFormatter()
{}

} // namespace commondecleditor

