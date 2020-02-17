/*
 * This file is part of mobata.
 *
 * mobata is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mobata is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mobata.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SPENATEDITOR_SPENATHOVERHANDLER_HPP
#define SPENATEDITOR_SPENATHOVERHANDLER_HPP

#include "spenateditor_global.hpp"

#include <texteditor/basehoverhandler.h>

namespace Core {
class IEditor;
}

namespace TextEditor {
class ITextEditor;
}

namespace spenateditor {

class SPENATEDITOR_EXPORT SpenatHoverHandler
    : public TextEditor::BaseHoverHandler
{
  Q_OBJECT
public:
  SpenatHoverHandler(QObject *parent = 0);
  virtual ~SpenatHoverHandler();

private:
  bool acceptEditor(Core::IEditor *editor);
  void identifyMatch(TextEditor::ITextEditor *editor, int pos);
  void decorateToolTip();
};

} // namespace spenateditor

#endif // SPENATEDITOR_SPENATHOVERHANDLER_HPP
