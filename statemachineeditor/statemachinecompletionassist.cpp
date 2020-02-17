/*
 *  This file is part of statemachine Editor Plugin for AppCreator based application.
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

#include "statemachinecompletionassist.hpp"
#include "statemachineeditorconstants.hpp"

#include <coreplugin/idocument.h>
#include <texteditor/completionsettings.h>
#include <texteditor/codeassist/basicproposalitem.h>
#include <texteditor/codeassist/basicproposalitemlistmodel.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/functionhintproposal.h>
#include <utils/faketooltip.h>
#include <dsleditor/dslproposalitem.hpp>

#include <mobata/model/base/signalitem.hpp>

#include <dslparser/statemachine/comcreatestatemachineproposals.hpp>

#include <QIcon>
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

using namespace statemachineeditor;
using namespace TextEditor;

namespace {

enum CompletionOrder {
  SpecialMemberOrder = -5
};


bool isActivationChar(const QChar &ch)
{
  return ch == QLatin1Char('(') || ch == QLatin1Char('.') || ch == QLatin1Char(',');
}

bool isIdentifierChar(QChar ch)
{
  return ch.isLetterOrNumber() || ch == QLatin1Char('_');
}

bool isDelimiter(QChar ch)
{
  switch (ch.unicode()) {
  case '{':
  case '}':
  case '[':
  case ']':
  case ')':
  case '?':
  case '!':
  case ':':
  case ';':
  case ',':
  case '+':
  case '-':
  case '*':
  case '/':
    return true;

  default:
    return false;
  }
}

bool checkStartOfIdentifier(const QString &word)
{
  if (! word.isEmpty()) {
    const QChar ch = word.at(0);
    if (ch.isLetter() || ch == QLatin1Char('_'))
      return true;
  }

  return false;
}

} // Anonymous

// ----------------------------
// StateMachineCompletionAssistProvider
// ----------------------------
bool StateMachineCompletionAssistProvider::supportsEditor(const Core::Id& editorId) const
{
  return editorId == Constants::STATEMACHINEEDITOR_ID;
}

IAssistProcessor* StateMachineCompletionAssistProvider::createProcessor() const
{
  return new StateMachineCompletionAssistProcessor;
}

int StateMachineCompletionAssistProvider::activationCharSequenceLength() const
{
  return 1;
}

bool StateMachineCompletionAssistProvider::isActivationCharSequence(const QString& sequence) const
{
  return (false ? sequence.isEmpty() : isActivationChar(sequence.at(0)));
}

// -----------------------------
// StateMachineFunctionHintProposalModel
// -----------------------------
class StateMachineFunctionHintProposalModel : public TextEditor::IFunctionHintProposalModel
{
public:
  StateMachineFunctionHintProposalModel(QVector<model::base::SignalItem*> signalSymbols)
    :_items(signalSymbols)
    ,_currentArg(-1)
  {}

  virtual void reset() {}
  virtual int size() const { return _items.size(); }
  virtual QString text(int index) const;
  virtual int activeArgument(const QString &prefix) const;

private:
  QVector<model::base::SignalItem*> _items;
  mutable int                       _currentArg;
};

QString StateMachineFunctionHintProposalModel::text(int index) const
{
  if(this->_items.isEmpty())
    return QStringLiteral("");

  return this->_items.at(index)->toString();
}

int StateMachineFunctionHintProposalModel::activeArgument(const QString &prefix) const
{
  const QByteArray &str = prefix.toLatin1();
  int argnr = 0;
  // int parcount = 0;
  //  GLSL::Lexer lexer(0, str.constData(), str.length());
  //  GLSL::Token tk;
  //  QList<GLSL::Token> tokens;
  //  do {
  //    lexer.yylex(&tk);
  //    tokens.append(tk);
  //  } while (tk.isNot(GLSL::Parser::EOF_SYMBOL));
  //  for (int i = 0; i < tokens.count(); ++i) {
  //    const GLSL::Token &tk = tokens.at(i);
  //    if (tk.is(GLSL::Parser::T_LEFT_PAREN))
  //      ++parcount;
  //    else if (tk.is(GLSL::Parser::T_RIGHT_PAREN))
  //      --parcount;
  //    else if (! parcount && tk.is(GLSL::Parser::T_COMMA))
  //      ++argnr;
  //  }

  //  if (parcount < 0)
  //    return -1;

  //  if (argnr != m_currentArg)
  //    m_currentArg = argnr;

  return argnr;
}

// -----------------------------
// statemachineCompletionAssistProcessor
// -----------------------------
StateMachineCompletionAssistProcessor::StateMachineCompletionAssistProcessor()
  : _startPosition(0)
  , _keywordIcon(QLatin1String(":/mobata/images/base/type.png"))
  , _stateIcon(QLatin1String(":/mobata/images/statemachine/state.png"))
  , _signalIcon(QLatin1String(":/mobata/images/base/signal.png"))
  , _typeIcon(QLatin1String(":/mobata/images/base/type.png"))
  , _functionIcon(QLatin1String(":/mobata/images/base/action.png"))
  , _attributeIcon(QLatin1String(":/mobata/images/base/attribute.png"))
  , _transitionIcon(QLatin1String(":/mobata/images/statemachine/transition.png"))
  , _portIcon(QLatin1String(":/mobata/images/base/port.png"))
  , _otherIcon(QLatin1String(":/mobata/images/new.png"))
{}

StateMachineCompletionAssistProcessor::~StateMachineCompletionAssistProcessor()
{}

QIcon StateMachineCompletionAssistProcessor::proposalIcon(const int proposalType)
{
  if(proposalType == dslparser::Keyword)
    return this->_keywordIcon;
  else if(proposalType == dslparser::Signal)
    return this->_signalIcon;
  else if(proposalType == dslparser::Attribute)
    return this->_attributeIcon;
  else if(proposalType == dslparser::Function)
    return this->_functionIcon;
  else if(proposalType == dslparser::State)
    return this->_stateIcon;
  else if(proposalType == dslparser::Port)
    return this->_portIcon;
  else if(proposalType == dslparser::SignalTransition)
    return this->_transitionIcon;
  else if(proposalType == dslparser::TimeoutTransition)
    return this->_transitionIcon;
  else if(proposalType == dslparser::Type)
    return this->_typeIcon;

  return this->_otherIcon;
}

IAssistProposal* StateMachineCompletionAssistProcessor::perform(const IAssistInterface* interface)
{
  this->_interface.reset(static_cast<const StateMachineCompletionAssistInterface *>(interface));

  if (interface->reason() == IdleEditor && !acceptsIdleEditor())
    return 0;

  int currentPosition = this->_interface->position();
  QString statemachineText = this->_interface->textAt(0, currentPosition);

  int leftPosition = currentPosition ? currentPosition - 1 : 0;
  QChar ch = this->_interface->characterAt(leftPosition);
  while (ch.isLetterOrNumber() || ch == QLatin1Char('_'))
    ch = this->_interface->characterAt(--leftPosition);
  this->_startPosition = currentPosition ? leftPosition + 1 : 0;

  int rightPosition = currentPosition;
  ch = this->_interface->characterAt(rightPosition);
  while (ch.isLetterOrNumber() || ch == QLatin1Char('_'))
    ch = this->_interface->characterAt(++rightPosition);

  int afterRemoveLength = rightPosition-currentPosition;
  if(afterRemoveLength<0)
    afterRemoveLength=0;

  return  this->createStateMachineProposals(statemachineText, afterRemoveLength);
}

IAssistProposal* StateMachineCompletionAssistProcessor::createStateMachineProposals(const QString& stateMachineText,
                                                                                    int afterRemoveLength)
{
  using namespace dslparser;
  using namespace dslparser::statemachine;

  ComCreateStateMachineProposals proposalsCommand(stateMachineText);
  QString errorString;
  bool result = proposalsCommand.execute(&errorString);
  if(!result)
    qDebug()<<"error during proposals creation: "<<errorString;

  if(proposalsCommand.statemachineProposals().size() == 0)
    return nullptr;

  QChar lastCharBeforeCompletion;
  if(stateMachineText.length())
    lastCharBeforeCompletion=stateMachineText.at(stateMachineText.length()-1);

  for(const DslProposal& stateMachineProposal : proposalsCommand.statemachineProposals())
  {
    QString proposalText = stateMachineProposal.proposalText;
    //    if(isIdentifierChar(lastCharBeforeCompletion))
    //      proposalText.insert(0, QLatin1Char(' '));

    this->addCompletion(proposalText,
                        this->proposalIcon(stateMachineProposal.proposalType),
                        afterRemoveLength);
  }

  return this->createContentProposal();
}

IAssistProposal* StateMachineCompletionAssistProcessor::createContentProposal() const
{
  IGenericProposalModel *model = new BasicProposalItemListModel(_completions);
  IAssistProposal *proposal = new GenericProposal(_startPosition, model);
  return proposal;
}

IAssistProposal* StateMachineCompletionAssistProcessor::createHintProposal(
    const QVector<model::base::SignalItem*>& symbols)
{
  IFunctionHintProposalModel *model = new StateMachineFunctionHintProposalModel(symbols);
  IAssistProposal *proposal = new FunctionHintProposal(_startPosition, model);
  return proposal;
}

bool StateMachineCompletionAssistProcessor::acceptsIdleEditor() const
{
  const int cursorPosition = this->_interface->position();
  const QChar ch = this->_interface->characterAt(cursorPosition - 1);

  const QChar characterUnderCursor = this->_interface->characterAt(cursorPosition);

  if (isIdentifierChar(ch) && (characterUnderCursor.isSpace() ||
                               characterUnderCursor.isNull() ||
                               isDelimiter(characterUnderCursor)))
  {
    int pos = _interface->position() - 1;
    for (; pos != -1; --pos) {
      if (! isIdentifierChar(this->_interface->characterAt(pos)))
        break;
    }
    ++pos;

    const QString word = _interface->textAt(pos, cursorPosition - pos);
    if (word.length() > 2 && checkStartOfIdentifier(word))
    {
      for (int i = 0; i < word.length(); ++i)
      {
        if (! isIdentifierChar(word.at(i)))
          return false;
      }
      return true;
    }
  }

  return isActivationChar(ch);
}

void StateMachineCompletionAssistProcessor::addCompletion(const QString &text,
                                                          const QIcon &icon,
                                                          int afterRemoveLength,
                                                          int order)
{
  dsleditor::DslProposalItem* item = new dsleditor::DslProposalItem;
  item->setText(text);
  item->setIcon(icon);
  item->setAfterRemoveLength(afterRemoveLength);
  item->setOrder(order);
  this->_completions.append(item);

  return;
}

// -----------------------------
// statemachineCompletionAssistInterface
// -----------------------------
StateMachineCompletionAssistInterface::StateMachineCompletionAssistInterface(QTextDocument *textDocument,
                                                                             int position,
                                                                             const QString &fileName,
                                                                             TextEditor::AssistReason reason)
  : DefaultAssistInterface(textDocument, position, fileName, reason)
{}
