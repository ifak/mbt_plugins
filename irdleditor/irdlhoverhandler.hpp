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
#ifndef IRDLEDITOR_IRDLHOVERHANDLER_HPP
#define IRDLEDITOR_IRDLHOVERHANDLER_HPP

#include "irdleditor_global.hpp"

#include <texteditor/basehoverhandler.h>

namespace Core {
class IEditor;
}

namespace TextEditor {
class ITextEditor;
}

namespace irdleditor {

class IRDLEDITOR_EXPORT IrdlHoverHandler
    : public TextEditor::BaseHoverHandler
{
  Q_OBJECT
public:
  IrdlHoverHandler(QObject *parent = 0);
  virtual ~IrdlHoverHandler();

private:
  bool acceptEditor(Core::IEditor *editor);
  void identifyMatch(TextEditor::ITextEditor *editor, int pos);
  void decorateToolTip();
};

} // namespace irdleditor

#endif // IRDLEDITOR_IRDLHOVERHANDLER_HPP
