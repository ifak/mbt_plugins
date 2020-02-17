/*
 *  This file is part of Isrdl Editor Plugin for AppCreator based application.
 *
 *  Copyright (C) 2013 Jan Krause <jan.krause.no19@gmail.com>
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
*/

#ifndef ISRDLEDITOR_ISRDLEDITORFACTORY_HPP
#define ISRDLEDITOR_ISRDLEDITORFACTORY_HPP

#include <coreplugin/editormanager/ieditorfactory.h>

namespace TextEditor {
class  TextEditorActionHandler;
}

namespace isrdleditor{
namespace Internal{

class IsrdlEditorPlugin;

class IsrdlEditorFactory
    : public Core::IEditorFactory
{
  Q_OBJECT

public:
  explicit IsrdlEditorFactory(IsrdlEditorPlugin* owner);
  virtual ~IsrdlEditorFactory();

public:
  QStringList     mimeTypes() const;
  Core::Id        id() const;
  QString         displayName() const;
  Core::IEditor*  createEditor(QWidget *parent);

private:
  Q_DISABLE_COPY(IsrdlEditorFactory)
  class Private;
  Private* _d;
};

} //end namespace Internal
} //end namespace isrdleditor

#endif //ISRDLEDITOR_ISRDLEDITORFACTORY_HPP
