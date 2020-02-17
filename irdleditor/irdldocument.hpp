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
#pragma once

#include "irdleditor_global.hpp"

#include <texteditor/basetextdocument.h>

namespace model{ namespace irdl{
class RequirementsModel;
}}

namespace irdleditor{

class IrdlOutlineModel;

class IrdlDocument
    : public TextEditor::BaseTextDocument
{
  Q_OBJECT

public:
  typedef QSharedPointer<IrdlDocument> Ptr;

public:
  explicit IrdlDocument(QObject *parent = 0);
  virtual ~IrdlDocument();

public:
  model::irdl::RequirementsModel* irdlDeclModel();
  IrdlOutlineModel*       irdlOutlineModel();

private:
  Q_DISABLE_COPY(IrdlDocument)
  class Private;
  Private* _d;
};

} //end namespace irdleditor
