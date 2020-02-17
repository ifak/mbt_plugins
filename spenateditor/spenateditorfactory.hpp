/*
 *  This file is part of Spenat Editor Plugin for AppCreator based application.
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

#ifndef SPENATEDITOR_SPENATEDITORFACTORY_HPP
#define SPENATEDITOR_SPENATEDITORFACTORY_HPP

#include <coreplugin/editormanager/ieditorfactory.h>

namespace TextEditor {
class  TextEditorActionHandler;
}

namespace spenateditor{
namespace Internal{

class SpenatEditorPlugin;

class SpenatEditorFactory
    : public Core::IEditorFactory
{
  Q_OBJECT

public:
  explicit SpenatEditorFactory(SpenatEditorPlugin* owner);
  virtual ~SpenatEditorFactory();

public:
  QStringList     mimeTypes() const;
  Core::Id        id() const;
  QString         displayName() const;
  Core::IEditor*  createEditor(QWidget *parent);

private:
  Q_DISABLE_COPY(SpenatEditorFactory)
  class Private;
  Private* _d;
};

} //end namespace Internal
} //end namespace spenateditor

#endif //SPENATEDITOR_SPENATEDITORFACTORY_HPP
