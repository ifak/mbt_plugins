/*
 * This file is part of mbt_plugins.
 *
 * mbt_plugins is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mbt_plugins is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mbt_plugins.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef STATEMACHINEEDITOR_STATEMACHINEHOVERHANDLER_HPP
#define STATEMACHINEEDITOR_STATEMACHINEHOVERHANDLER_HPP

#include "statemachineeditor_global.hpp"

#include <texteditor/basehoverhandler.h>

namespace Core {
class IEditor;
}

namespace TextEditor {
class ITextEditor;
}

namespace statemachineeditor {

class STATEMACHINEEDITOR_EXPORT StateMachineHoverHandler
    : public TextEditor::BaseHoverHandler
{
  Q_OBJECT
public:
  StateMachineHoverHandler(QObject *parent = 0);
  virtual ~StateMachineHoverHandler();

private:
  bool acceptEditor(Core::IEditor *editor);
  void identifyMatch(TextEditor::ITextEditor *editor, int pos);
  void decorateToolTip();
};

} // namespace statemachineeditor

#endif // STATEMACHINEEDITOR_STATEMACHINEHOVERHANDLER_HPP
