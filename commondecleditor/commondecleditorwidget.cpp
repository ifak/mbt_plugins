#include "commondecleditorwidget.hpp"

#include "commondecldocument.hpp"
#include "commondeclhighlighter.hpp"
#include "commondeclindenter.hpp"
#include "commondeclautocompleter.hpp"
#include "commondeclcompletionassist.hpp"
#include "commondecloutlinemodel.hpp"
#include "combuildcommondecloutlinemodel.hpp"
#include "commondecleditorconstants.hpp"

#include <coreplugin/messagemanager.h>
#include <texteditor/fontsettings.h>
#include <utils/qtcassert.h>
#include <utils/uncommentselection.h>

#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QTextBlock>
#include <QTimer>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace commondecleditor{

enum {
  UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150
};

class CommonDeclEditorWidget::Private
{
  friend class CommonDeclEditorWidget;

  Private()
    : _commondeclDocument(new CommonDeclDocument)
  {}

public:
  ~Private()
  {}

private:
  CommonDeclEditor*         _commondeclEditor = nullptr;
  CommonDeclDocument::Ptr   _commondeclDocument;
  QTimer*               _updateOutlineTimer = nullptr;
  QFile                 _commondeclGraphicFile;
};

CommonDeclEditorWidget::CommonDeclEditorWidget(QWidget *parent)
  : DslEditorWidget(parent),
    _d(new Private())
{
  this->_d->_commondeclEditor=new CommonDeclEditor(this);
  this->setBaseTextDocument(this->_d->_commondeclDocument);

  this->baseTextDocument()->setSyntaxHighlighter(new CommonDeclHighlighter(this->document()));
  this->setIndenter(new dsleditor::DslIndenter());
  this->setAutoCompleter(new dsleditor::DslAutoCompleter()/*CommonDeclAutoCompleter()*/);

  this->_d->_updateOutlineTimer = new QTimer(this);
  this->_d->_updateOutlineTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
  this->_d->_updateOutlineTimer->setSingleShot(true);
  connect(this->_d->_updateOutlineTimer, &QTimer::timeout,
          this, &CommonDeclEditorWidget::updateOutlineNow);
  connect(this, &CommonDeclEditorWidget::textChanged,
          this, &CommonDeclEditorWidget::updateOutline);
  connect(this, &CommonDeclEditorWidget::cursorPositionChanged,
          this, &CommonDeclEditorWidget::updateOutlineIndexNow);
}

CommonDeclEditorWidget::~CommonDeclEditorWidget()
{
  if(this->_d->_commondeclGraphicFile.exists())
  {
    this->_d->_commondeclGraphicFile.close();
    this->_d->_commondeclGraphicFile.remove();
  }

  delete this->_d;
}

void CommonDeclEditorWidget::duplicateFrom(BaseTextEditorWidget* editor)
{
  CommonDeclEditorWidget* otherEditorWidget = qobject_cast<CommonDeclEditorWidget*>(editor);
  Q_ASSERT(otherEditorWidget);

  BaseTextEditorWidget::duplicateFrom(otherEditorWidget);

  this->_d->_commondeclGraphicFile.setFileName(otherEditorWidget->_d->_commondeclGraphicFile.fileName());

  return;
}

CommonDeclEditor* CommonDeclEditorWidget::createEditor()
{
  return this->_d->_commondeclEditor;
}

void CommonDeclEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
  return DslEditorWidget::setFontSettings(fs);
}

QVector<TextEditor::TextStyle> CommonDeclEditorWidget::highlighterFormatCategories()
{
  QVector<TextEditor::TextStyle> categories = DslEditorWidget::highlighterFormatCategories();
  categories << TextEditor::C_FUNCTION;

  return categories;
}

QString CommonDeclEditorWidget::wordUnderCursor() const
{
  QTextCursor tc = textCursor();
  const QChar ch = document()->characterAt(tc.position() - 1);
  // make sure that we're not at the start of the next word.
  if (ch.isLetterOrNumber() || ch == QLatin1Char('_'))
    tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::StartOfWord);
  tc.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
  const QString word = tc.selectedText();

  return word;
}

const CommonDeclDocument::Ptr& CommonDeclEditorWidget::commondeclDocument() const
{
  return this->_d->_commondeclDocument;
}

void CommonDeclEditorWidget::unCommentSelection()
{
  return Utils::unCommentSelection(this);
}

void CommonDeclEditorWidget::updateOutline()
{
  this->_d->_updateOutlineTimer->start();
}

QModelIndex CommonDeclEditorWidget::outlineModelIndex()
{
  QModelIndex currentModelIndex = this->indexForPosition(this->position());
  if(currentModelIndex.isValid())
    emit outlineModelIndexChanged(currentModelIndex);

  return currentModelIndex;
}

QTextCharFormat textCharFormat(int tokenType,
                               CommonDeclHighlighter* commondeclHighlighter)
{
  Q_ASSERT(commondeclHighlighter);

  if(tokenType == dslparser::Requirement)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Import)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Message)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Check)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::State)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Duration)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Declaration)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Actor)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Component)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Use)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::All)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Attribute)
    return commondeclHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Description)
    return commondeclHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::As)
    return commondeclHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::At)
    return commondeclHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Signal)
    return commondeclHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Trigger)
    return commondeclHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::This)
    return commondeclHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  //  else if(tokenType == dslparser::Function)
  //    return commondeclHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Port)
    return commondeclHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Place)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::SignalTransition)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::TimeoutTransition)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Type)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Keyword)
    return commondeclHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::UnknownRequirmentTokenType)
    return commondeclHighlighter->textCharFormat(TextEditor::C_TEXT);

  return commondeclHighlighter->textCharFormat(TextEditor::C_TEXT);
}

void CommonDeclEditorWidget::updateOutlineNow()
{
  qDebug()<<"CommonDeclEditorWidget::updateOutlineNow() executed!";

  this->_d->_updateOutlineTimer->stop();

  CommonDeclOutlineModel* commondeclOutlineModel = this->_d->_commondeclDocument->commondeclOutlineModel();
  Q_ASSERT(commondeclOutlineModel);

  QString currText=this->toPlainText();
  QString path = this->_d->_commondeclEditor->document()->fileName();
  path = path.mid(0, path.lastIndexOf(QString("/"))+1);
  ComBuildCommonDeclOutlineModel buildOutlineCommand(currText, commondeclOutlineModel,path);
  buildOutlineCommand.execute();

  //TODO: just a try, maybe we need 'updateOutlineIndexNow()'
  this->outlineModelIndex();

  /// first semantic highlights//////
  CommonDeclHighlighter* commondeclHighlighter = qobject_cast<CommonDeclHighlighter*>(this->baseTextDocument()->syntaxHighlighter());
  Q_ASSERT(commondeclHighlighter);

  QList<QTextEdit::ExtraSelection> extraSelections;
  for(const dslparser::TokenTextLocation& textLoc : commondeclOutlineModel->keywordTextSelections())
  {
    if(!textLoc.isValid())
      continue;

    QTextCursor cursor(this->baseTextDocument()->document());
    cursor.setPosition(textLoc.start());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextCharFormat textFormat = textCharFormat(textLoc.tokenType(), commondeclHighlighter);
    if(!textFormat.isValid())
      continue;

    QTextEdit::ExtraSelection sel;
    sel.cursor = cursor;
    sel.format = textFormat;
    extraSelections.append(sel);
  }
  this->setExtraSelections(OtherSelection, extraSelections);

  ///now warnings and/or errors
  extraSelections.clear();

  QTextCharFormat errorFormat;
  errorFormat.setBackground(Qt::NoBrush);
  errorFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
  errorFormat.setUnderlineColor(Qt::red);

  QSet<int> errors;
  for (const dslparser::DslError& commondeclError : buildOutlineCommand.errors())
  {
    if (! commondeclError.line())
      continue;
    else if (errors.contains((const int)commondeclError.line()))
      continue;

    errors.insert((const int)commondeclError.line());

    QTextBlock block=document()->findBlockByLineNumber((int)commondeclError.line()-1);
    QTextCursor cursor(block);
    cursor.setPosition(block.position()+commondeclError.charPositionInLine());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextEdit::ExtraSelection sel;
    sel.cursor = cursor;
    sel.format = errorFormat;
    sel.format.setToolTip(commondeclError.message());
    extraSelections.append(sel);
  }
  this->setExtraSelections(CodeWarningsSelection, extraSelections);

  return;
}

void CommonDeclEditorWidget::updateOutlineIndexNow()
{
  if (this->_d->_updateOutlineTimer->isActive())
    return; // updateOutlineNow will call this function soon anyway

  this->outlineModelIndex();

  return;
}

QModelIndex CommonDeclEditorWidget::indexForPosition(unsigned cursorPosition) const
{
  using namespace dslparser;

  CommonDeclOutlineModel const* commondeclOutlineModel = this->_d->_commondeclEditor->commondeclOutlineModel();
  Q_ASSERT(commondeclOutlineModel);

  const ModelTextLocations& modelTextLocations = commondeclOutlineModel->modelTextSelections();
  ModelTextLocations::ConstIterator mt_it = modelTextLocations.begin();
  for( ; mt_it != modelTextLocations.end(); ++mt_it)
  {
    const QModelIndex& currModelIndex = mt_it.key();
    const TokenTextLocation& currTokLocation = mt_it.value();

    if(!currTokLocation.isValid())
      continue;

    if(cursorPosition >= unsigned(currTokLocation.start()))
    {
      if(cursorPosition <= (unsigned(currTokLocation.start())
                            + unsigned(currTokLocation.length())))
      {
        return currModelIndex;
      }
    }
  }

  return QModelIndex();
}

IAssistInterface* CommonDeclEditorWidget::createAssistInterface(AssistKind assistKind,
                                                                AssistReason assistReason) const
{
  if (assistKind == TextEditor::Completion)
    return new CommonDeclCompletionAssistInterface(this->document(),
                                                   this->position(),
                                                   editor()->document()->fileName(),
                                                   assistReason);
  return BaseTextEditorWidget::createAssistInterface(assistKind, assistReason);
}

} //end namespace commondecleditor
