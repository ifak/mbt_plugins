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
#ifndef STATEMACHINEEDITOR_STATEMACHINEGRAPHICDOCUMENT_HPP
#define STATEMACHINEEDITOR_STATEMACHINEGRAPHICDOCUMENT_HPP

#include "statemachineeditor_global.hpp"

#include <coreplugin/idocument.h>

namespace view{ namespace statemachine{
class DeclScene;
}}

namespace statemachineeditor {

class STATEMACHINEEDITOR_EXPORT StateMachineGraphicDocument
    : public Core::IDocument
{
  Q_OBJECT

public:
  StateMachineGraphicDocument(QObject *parent = 0);
  virtual ~StateMachineGraphicDocument();

public:
  view::statemachine::DeclScene*  declScene();
  bool                      open(QString *errorString,
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
  Q_DISABLE_COPY(StateMachineGraphicDocument)
  class Private;
  Private* _d;
};

} // namespace statemachineeditor

#endif // STATEMACHINEEDITOR_STATEMACHINEGRAPHICDOCUMENT_HPP