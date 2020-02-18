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

#include <coreplugin/idocument.h>

namespace view{ namespace irdl{
class DeclScene;
}}

namespace irdleditor {

class IRDLEDITOR_EXPORT IrdlGraphicDocument
    : public Core::IDocument
{
  Q_OBJECT

public:
  IrdlGraphicDocument(QObject *parent = 0);
  virtual ~IrdlGraphicDocument();

public:
  bool  open(QString *errorString,
             const QString &fileName,
             const QString &realFileName);

public:
  virtual bool save(QString *errorString,
                    const QString &fileName = QString(),
                    bool autoSave = false);
  virtual QString fileName() const;
  virtual bool isFileReadOnly() const;

  virtual QString defaultPath() const;
  virtual QString suggestedFileName() const;
  virtual QString mimeType() const;

  virtual bool shouldAutoSave() const;
  virtual bool isModified() const;
  virtual bool isSaveAsAllowed() const;

  virtual bool reload(QString *errorString,
                      ReloadFlag flag,
                      ChangeType type);
  virtual void rename(const QString &newName);

private:
  Q_DISABLE_COPY(IrdlGraphicDocument)
  class Private;
  Private* _d;
};

} // namespace irdleditor
