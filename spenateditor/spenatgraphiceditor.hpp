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

#include "spenateditor_global.hpp"

#include <dsleditor/dslgraphiceditor.hpp>

#include "spenatgraphicwidget.hpp"

#include <mobata/model/spenat/spenatdeclmodel.hpp>

namespace spenateditor {

class SpenatDocument;
class SpenatEditor;

using model::spenat::SpenatDeclModelPtr;

class SPENATEDITOR_EXPORT SpenatGraphicEditor
    : public dsleditor::DslGraphicEditor
{
  Q_OBJECT

public:
  SpenatGraphicEditor(SpenatGraphicWidget* spenatGraphicWidget);
  virtual ~SpenatGraphicEditor();

public:
  virtual bool              open(QString *errorString,
                                 const QString &fileName,
                                 const QString &realFileName) override;
  virtual Core::IDocument*  document() override;
  virtual QWidget*          toolBar() override;

public:
  void                      setSpenatDeclModel(const SpenatDeclModelPtr& spenatDeclModel);
  const SpenatDeclModelPtr& spenatDeclModel() const;
  void                      setSpenatDocument(SpenatDocument const* spenatDocument);
  SpenatDocument const*     spenatDocument() const;

public:
  SpenatGraphicWidget*  spenatGraphicWidget();
  QString               fileName() const;

private:
  Q_DISABLE_COPY(SpenatGraphicEditor)
  class Private;
  Private* _d;
};

} // namespace spenateditor
