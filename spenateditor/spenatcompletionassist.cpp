#include "spenatcompletionassist.hpp"
#include "spenateditorconstants.hpp"

#include <coreplugin/idocument.h>
#include <texteditor/completionsettings.h>
#include <texteditor/codeassist/basicproposalitem.h>
#include <texteditor/codeassist/basicproposalitemlistmodel.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/functionhintproposal.h>
#include <utils/faketooltip.h>
#include <dsleditor/dslproposalitem.hpp>

#include <mobata/model/base/signalitem.hpp>

#include <dslparser/spenat/comcreatespenatproposals.hpp>

#include <QIcon>
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

using namespace spenateditor;
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
// SpenatCompletionAssistProvider
// ----------------------------
bool SpenatCompletionAssistProvider::supportsEditor(const Core::Id& editorId) const
{
  return editorId == Constants::SPENATEDITOR_ID;
}

IAssistProcessor* SpenatCompletionAssistProvider::createProcessor() const
{
  return new SpenatCompletionAssistProcessor;
}

int SpenatCompletionAssistProvider::activationCharSequenceLength() const
{
  return 1;
}

bool SpenatCompletionAssistProvider::isActivationCharSequence(const QString& sequence) const
{
  return (false ? sequence.isEmpty() : isActivationChar(sequence.at(0)));
}

// -----------------------------
// SpenatFunctionHintProposalModel
// -----------------------------
class SpenatFunctionHintProposalModel : public TextEditor::IFunctionHintProposalModel
{
public:
  SpenatFunctionHintProposalModel(QVector<model::base::SignalItem*> signalSymbols)
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

QString SpenatFunctionHintProposalModel::text(int index) const
{
  if(this->_items.isEmpty())
    return QStringLiteral("");

  return this->_items.at(index)->toString();
}

int SpenatFunctionHintProposalModel::activeArgument(const QString &prefix) const
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
// SpenatCompletionAssistProcessor
// -----------------------------
SpenatCompletionAssistProcessor::SpenatCompletionAssistProcessor()
  : _startPosition(0)
  , _keywordIcon(QLatin1String(":/mobata/images/base/type.png"))
  , _placeIcon(QLatin1String(":/mobata/images/spenat/place.png"))
  , _signalIcon(QLatin1String(":/mobata/images/base/signal.png"))
  , _typeIcon(QLatin1String(":/mobata/images/base/type.png"))
  , _functionIcon(QLatin1String(":/mobata/images/base/action.png"))
  , _attributeIcon(QLatin1String(":/mobata/images/base/attribute.png"))
  , _transitionIcon(QLatin1String(":/mobata/images/spenat/transition.png"))
  , _portIcon(QLatin1String(":/mobata/images/base/port.png"))
  , _otherIcon(QLatin1String(":/mobata/images/new.png"))
{}

SpenatCompletionAssistProcessor::~SpenatCompletionAssistProcessor()
{}

QIcon SpenatCompletionAssistProcessor::proposalIcon(const int proposalType)
{
  if(proposalType == dslparser::Keyword)
    return this->_keywordIcon;
  else if(proposalType == dslparser::Signal)
    return this->_signalIcon;
  else if(proposalType == dslparser::Attribute)
    return this->_attributeIcon;
    else if(proposalType == dslparser::Function)
      return this->_functionIcon;
  else if(proposalType == dslparser::Place)
    return this->_placeIcon;
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

IAssistProposal* SpenatCompletionAssistProcessor::perform(const IAssistInterface* interface)
{
  this->_interface.reset(static_cast<const SpenatCompletionAssistInterface *>(interface));

  if (interface->reason() == IdleEditor && !acceptsIdleEditor())
    return 0;

  int currentPosition = this->_interface->position();
  QString spenatText = this->_interface->textAt(0, currentPosition);

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

  return this->createSpenatProposals(spenatText, afterRemoveLength);
}

IAssistProposal* SpenatCompletionAssistProcessor::createSpenatProposals(const QString& spenatText,
                                                                        int afterRemoveLength)
{
  using namespace dslparser;
  using namespace dslparser::spenat;

  ComCreateSpenatProposals proposalsCommand(spenatText);
  QString errorString;
  bool result = proposalsCommand.execute(&errorString);
  if(!result)
    qDebug()<<"error during proposals creation: "<<errorString;

  if(proposalsCommand.spenatProposals().size() == 0)
    return nullptr;

  QChar lastCharBeforeCompletion;
  if(spenatText.length())
    lastCharBeforeCompletion=spenatText.at(spenatText.length()-1);

  for(const DslProposal& spenatProposal : proposalsCommand.spenatProposals())
  {
    QString proposalText = spenatProposal.proposalText;
    //    if(isIdentifierChar(lastCharBeforeCompletion))
    //      proposalText.insert(0, QLatin1Char(' '));

    this->addCompletion(proposalText,
                        this->proposalIcon(spenatProposal.proposalType),
                        afterRemoveLength);
  }

  return this->createContentProposal();
}

IAssistProposal* SpenatCompletionAssistProcessor::createContentProposal() const
{
  IGenericProposalModel *model = new BasicProposalItemListModel(_completions);
  IAssistProposal *proposal = new GenericProposal(_startPosition, model);
  return proposal;
}

IAssistProposal* SpenatCompletionAssistProcessor::createHintProposal(
    const QVector<model::base::SignalItem*>& symbols)
{
  IFunctionHintProposalModel *model = new SpenatFunctionHintProposalModel(symbols);
  IAssistProposal *proposal = new FunctionHintProposal(_startPosition, model);
  return proposal;
}

bool SpenatCompletionAssistProcessor::acceptsIdleEditor() const
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

void SpenatCompletionAssistProcessor::addCompletion(const QString &text,
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
// SpenatCompletionAssistInterface
// -----------------------------
SpenatCompletionAssistInterface::SpenatCompletionAssistInterface(QTextDocument *textDocument,
                                                                 int position,
                                                                 const QString &fileName,
                                                                 TextEditor::AssistReason reason)
  : DefaultAssistInterface(textDocument, position, fileName, reason)
{}
