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

#include "mbtp_global.hpp"

#include <coreplugin/editormanager/ieditorfactory.h>

namespace mbtp {

class MBTPSHARED_EXPORT MbtpAttributesFactory
    : public Core::IEditorFactory
{
    Q_OBJECT
public:
  MbtpAttributesFactory(QObject *parent = nullptr);
  virtual ~MbtpAttributesFactory();

public:
  Core::IEditor*  createEditor(QWidget* parent);
  QStringList     mimeTypes() const;
  Core::Id        id() const;
  QString         displayName() const;
};

} // namespace mbtp
