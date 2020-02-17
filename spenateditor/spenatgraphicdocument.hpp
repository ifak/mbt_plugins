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

#include "spenateditor_global.hpp"

#include <coreplugin/idocument.h>

namespace view{ namespace spenat{
class DeclScene;
}}

namespace spenateditor {

class SPENATEDITOR_EXPORT SpenatGraphicDocument
    : public Core::IDocument
{
  Q_OBJECT

public:
  SpenatGraphicDocument(QObject *parent = 0);
  virtual ~SpenatGraphicDocument();

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
  Q_DISABLE_COPY(SpenatGraphicDocument)
  class Private;
  Private* _d;
};

} // namespace spenateditor
