/*
 *  This file is part of CommonDecl Editor Plugin for AppCreator based application.
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


#ifndef COMMONDECLEDITOR_COMMONDECLCOMPLETIONASSIST_HPP
#define COMMONDECLEDITOR_COMMONDECLCOMPLETIONASSIST_HPP

#include "commondecleditor.hpp"

#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/iassistprocessor.h>
#include <texteditor/codeassist/defaultassistinterface.h>
#include <texteditor/codeassist/ifunctionhintproposalmodel.h>

#include <QScopedPointer>
#include <QIcon>

namespace model{ namespace base{
class SignalItem;
}}

namespace TextEditor {
class BasicProposalItem;
}

namespace commondecleditor {

class CommonDeclCompletionAssistInterface;

class CommonDeclCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
public:
  virtual bool supportsEditor(const Core::Id &editorId) const;
  virtual TextEditor::IAssistProcessor *createProcessor() const;

  virtual int activationCharSequenceLength() const;
  virtual bool isActivationCharSequence(const QString& sequence) const;
};

class CommonDeclCompletionAssistProcessor : public TextEditor::IAssistProcessor
{
public:
  CommonDeclCompletionAssistProcessor();
  virtual ~CommonDeclCompletionAssistProcessor();

  virtual TextEditor::IAssistProposal *perform(const TextEditor::IAssistInterface *interface);

private:
  TextEditor::IAssistProposal* createCommonDeclProposals(const QString& commondeclText,
                                                     int afterRemoveLength, const QString &path);
  TextEditor::IAssistProposal* createContentProposal() const;
  TextEditor::IAssistProposal* createHintProposal(const QVector<model::base::SignalItem*>& symbols);
  bool acceptsIdleEditor() const;
  void addCompletion(const QString &text, const QIcon &icon,
                     int afterRemoveLength, int order = 0);
  QIcon proposalIcon(const int proposalType);

private:
  int                                                   _startPosition;
  QScopedPointer<const CommonDeclCompletionAssistInterface> _interface;
  QList<TextEditor::BasicProposalItem*>                 _completions;

  QIcon _keywordIcon;
  QIcon _placeIcon;
  QIcon _signalIcon;
  QIcon _typeIcon;
  QIcon _functionIcon;
  QIcon _attributeIcon;
  QIcon _transitionIcon;
  QIcon _portIcon;
  QIcon _otherIcon;
};

class CommonDeclCompletionAssistInterface : public TextEditor::DefaultAssistInterface
{
public:
  CommonDeclCompletionAssistInterface(QTextDocument *textDocument,
                                  int position,
                                  const QString &fileName,
                                  TextEditor::AssistReason reason);
};

} // commondecleditor

#endif // COMMONDECLEDITOR_COMMONDECLCOMPLETIONASSIST_HPP
