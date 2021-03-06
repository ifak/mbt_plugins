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
#pragma once

#include "irdleditor_global.hpp"

#include <coreplugin/editormanager/ieditor.h>

#include "irdlgraphicwidget.hpp"

namespace irdleditor {

class IrdlOutlineModel;
class IrdlDocument;

class IRDLEDITOR_EXPORT IrdlGraphicEditor
    : public Core::IEditor
{
  Q_OBJECT

public:
  IrdlGraphicEditor(IrdlGraphicWidget* irdlGraphicWidget);
  ~IrdlGraphicEditor();

public:
  virtual bool              createNew(const QString &contents = QString());
  virtual bool              open(QString *errorString,
                                 const QString &fileName,
                                 const QString &realFileName);
  virtual Core::IDocument*  document();
  virtual Core::Id          id() const;
  virtual QString           displayName() const;
  virtual void              setDisplayName(const QString &title);
  virtual bool              isTemporary() const;
  virtual QWidget*          toolBar();

public:
  void                      setIrdlOutlineModel(IrdlOutlineModel const* irdlOutlineModel);
  IrdlOutlineModel const* irdlOutlineModel() const;
  void                      setIrdlDocument(IrdlDocument const* irdlDocument);
  IrdlDocument const*     irdlDocument() const;

public:
  IrdlGraphicWidget*  irdlGraphicWidget();
  QString               fileName() const;

private:
  Q_DISABLE_COPY(IrdlGraphicEditor)
  class Private;
  Private* _d;
};

} // namespace irdleditor
