#include "simsystemeditorwidget.hpp"

#include "simsystemdocument.hpp"
#include "simsystemoutlinemodel.hpp"
#include "combuildsimsystemoutlinemodel.hpp"

#include <coreplugin/messagemanager.h>
#include <texteditor/fontsettings.h>
#include <utils/qtcassert.h>
#include <utils/uncommentselection.h>
#include <dsleditor/dslhighlighter.hpp>
#include <dsleditor/dslindenter.hpp>
#include <dsleditor/dslautocompleter.hpp>
#include <simsystemcompletionassist.hpp>

#include <projectexplorer/buildstep.h>

#include <graphlayout/layoutglobal.hpp>
#include <graphlayout/comlayoutdeclmodel.hpp>
#include <graphlayout/layoutgraph.hpp>
#include <graphlayout/graphquickwidget.hpp>

#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QTextBlock>
#include <QTimer>
#include <QDebug>
#include <QQuickView>
#include <QHBoxLayout>
#include <QFileSystemWatcher>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace specsim{

enum {
  UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150
};

class SimSystemEditorWidget::Private
{
  friend class SimSystemEditorWidget;

  Private()
    : _simSystemEditor(0),
      _simsystemDocument(new SimSystemDocument)
  {}

public:
  ~Private()
  {}

private:
  SimSystemEditor*                 _simSystemEditor;
  SimSystemDocument::Ptr           _simsystemDocument;
  QTimer*                             _updateOutlineTimer;
};

SimSystemEditorWidget::SimSystemEditorWidget(QWidget *parent)
  : DslEditorWidget(parent),
    _d(new Private())
{
  this->_d->_simSystemEditor=new SimSystemEditor(this);
  this->setBaseTextDocument(this->_d->_simsystemDocument);

  this->baseTextDocument()->setSyntaxHighlighter(new dsleditor::DslHighlighter(this->document()));
  this->setIndenter(new dsleditor::DslIndenter());
  this->setAutoCompleter(new dsleditor::DslAutoCompleter()/*SpenatAutoCompleter()*/);

  this->_d->_updateOutlineTimer = new QTimer(this);
  this->_d->_updateOutlineTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
  this->_d->_updateOutlineTimer->setSingleShot(true);
  connect(this->_d->_updateOutlineTimer, &QTimer::timeout,
          this, &SimSystemEditorWidget::updateOutlineNow);
  connect(this, &SimSystemEditorWidget::textChanged,
          this, &SimSystemEditorWidget::updateOutline);
  connect(this, &SimSystemEditorWidget::cursorPositionChanged,
          this, &SimSystemEditorWidget::updateOutlineIndexNow);
}

SimSystemEditorWidget::~SimSystemEditorWidget()
{
  delete this->_d;
}

void SimSystemEditorWidget::duplicateFrom(BaseTextEditorWidget* editor)
{
  SimSystemEditorWidget* otherEditorWidget = qobject_cast<SimSystemEditorWidget*>(editor);
  Q_ASSERT(otherEditorWidget);

  BaseTextEditorWidget::duplicateFrom(otherEditorWidget);

  return;
}

SimSystemEditor* SimSystemEditorWidget::createEditor()
{
  return this->_d->_simSystemEditor;
}

void SimSystemEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
  return DslEditorWidget::setFontSettings(fs);
}

QVector<TextEditor::TextStyle> SimSystemEditorWidget::highlighterFormatCategories()
{
  QVector<TextEditor::TextStyle> categories = DslEditorWidget::highlighterFormatCategories();
  categories << TextEditor::C_FUNCTION;

  return categories;
}

QString SimSystemEditorWidget::wordUnderCursor() const
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

const SimSystemDocument::Ptr& SimSystemEditorWidget::simSystemDocument() const
{
  return this->_d->_simsystemDocument;
}

void SimSystemEditorWidget::unCommentSelection()
{
  return Utils::unCommentSelection(this);
}

void SimSystemEditorWidget::updateOutline()
{
  this->_d->_updateOutlineTimer->start();
}

QModelIndex SimSystemEditorWidget::outlineModelIndex()
{
  QModelIndex currentModelIndex = this->indexForPosition(this->position());
  if(currentModelIndex.isValid())
    emit outlineModelIndexChanged(currentModelIndex);

  return currentModelIndex;
}

QTextCharFormat textCharFormat(int tokenType,
                               dsleditor::DslHighlighter* simSystemHighlighter)
{
  Q_ASSERT(simSystemHighlighter);

  if(tokenType == dslparser::SimSystem)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::SimSystem)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Connection)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Config)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::StartSequence)
    return simSystemHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Trigger)
    return simSystemHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::This)
    return simSystemHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Attribute)
    return simSystemHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Function)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Import)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::SignalTransition)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::TimeoutTransition)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::JunctionTransition)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Type)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Keyword)
    return simSystemHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::UnknownSimSystemTokenType)
    return simSystemHighlighter->textCharFormat(TextEditor::C_TEXT);

  return simSystemHighlighter->textCharFormat(TextEditor::C_TEXT);
}

void SimSystemEditorWidget::updateOutlineNow()
{
  qDebug()<<"SimSystemEditorWidget::updateOutlineNow() executed!";

  this->_d->_updateOutlineTimer->stop();

  SimSystemOutlineModelPtr simSystemOutlineModel = this->_d->_simsystemDocument->simsystemOutlineModel();
  Q_ASSERT(simSystemOutlineModel);
  simSystemOutlineModel->resetModel();

  QString currText=this->toPlainText();
  QString path = this->_d->_simSystemEditor->document()->fileName();
  path = path.mid(0, path.lastIndexOf(QString("/"))+1);

  ComBuildSimSystemOutlineModel buildOutlineCommand(currText, simSystemOutlineModel.data(),path);
  buildOutlineCommand.execute();

  //TODO: just a try, maybe we need 'updateOutlineIndexNow()'
  this->outlineModelIndex();

  /// first semantic highlights//////
  dsleditor::DslHighlighter* simSystemHighlighter = qobject_cast<dsleditor::DslHighlighter*>(this->baseTextDocument()->syntaxHighlighter());
  Q_ASSERT(simSystemHighlighter);

  QList<QTextEdit::ExtraSelection> extraSelections;
  for(const dslparser::TokenTextLocation& textLoc : simSystemOutlineModel->keywordTextSelections())
  {
    if(!textLoc.isValid())
      continue;

    QTextCursor cursor(this->baseTextDocument()->document());
    cursor.setPosition(textLoc.start());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextCharFormat textFormat = textCharFormat(textLoc.tokenType(), simSystemHighlighter);
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
  for (const dslparser::DslError& simsystemError : buildOutlineCommand.errors())
  {
    if (! simsystemError.line())
      continue;
    else if (errors.contains((const int)simsystemError.line()))
      continue;

    errors.insert((const int)simsystemError.line());

    QTextBlock block=document()->findBlockByLineNumber((int)simsystemError.line()-1);
    QTextCursor cursor(block);
    cursor.setPosition(block.position()+simsystemError.charPositionInLine());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextEdit::ExtraSelection sel;
    sel.cursor = cursor;
    sel.format = errorFormat;
    sel.format.setToolTip(simsystemError.message());
    extraSelections.append(sel);
  }
  this->setExtraSelections(CodeWarningsSelection, extraSelections);
	
  return;
}

void SimSystemEditorWidget::updateOutlineIndexNow()
{
  if (this->_d->_updateOutlineTimer->isActive())
    return; // updateOutlineNow will call this function soon anyway

  this->outlineModelIndex();

  return;
}

QModelIndex SimSystemEditorWidget::indexForPosition(unsigned cursorPosition) const
{
  using namespace dslparser;

  const SimSystemOutlineModelPtr simSystemOutlineModel = this->_d->_simSystemEditor->simsystemOutlineModel();
  Q_ASSERT(simSystemOutlineModel);

  const ModelTextLocations& modelTextLocations = simSystemOutlineModel->modelTextSelections();
  ModelTextLocations::ConstIterator mt_it = modelTextLocations.begin();
  QHash<QModelIndex,int> indexList;
  for( ; mt_it != modelTextLocations.end(); ++mt_it)
  {
    const QModelIndex& currModelIndex = mt_it.key();
    const TokenTextLocation& currTokLocation = mt_it.value();
    if(!currTokLocation.isValid())
      continue;

    if(cursorPosition >= unsigned(currTokLocation.start()))
    {
      if(cursorPosition <= (unsigned(currTokLocation.end())))
      {
        if(currModelIndex.isValid())
          indexList.insert(currModelIndex,currTokLocation.length());
      }
    }
  }
  if(!indexList.size()) return QModelIndex();

  int cursmallest = indexList.values().first();
  for(auto index : indexList.values()){
    if(index<cursmallest){
      cursmallest = index;
    }
  }
  return indexList.key(cursmallest);
}

IAssistInterface* SimSystemEditorWidget::createAssistInterface(AssistKind assistKind,
                                                                  AssistReason assistReason) const
{
  if (assistKind == TextEditor::Completion)
    return new SimSystemCompletionAssistInterface(this->document(),
                                                     this->position(),
                                                     editor()->document()->fileName(),
                                                     assistReason);

  return BaseTextEditorWidget::createAssistInterface(assistKind, assistReason);
}

void SimSystemEditorWidget::showSimSystemGraphic()
{
  QString tempSimSystemGraphicFile = QDir::tempPath();
  if (!tempSimSystemGraphicFile.endsWith(QLatin1Char('/')))
    tempSimSystemGraphicFile += QLatin1Char('/');
  tempSimSystemGraphicFile += QFileInfo(this->editorDocument()->fileName()).baseName()
                                 + QLatin1String(".simsystemgraphic");

  Core::EditorManager::instance()->gotoOtherSplit();

  updateOutlineNow();


  return;
}

void SimSystemEditorWidget::highlightGraphicItemByUuid(const QUuid& itemUuid,
                                                          bool highlight,
                                                          bool unhighlightOtherItems)
{
  Q_UNUSED(itemUuid);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
  return;
}

void SimSystemEditorWidget::highlightGraphicItemsByUuids(const QList<QUuid>& itemUuids,
                                                            bool highlight,
                                                            bool unhighlightOtherItems)
{
  Q_UNUSED(itemUuids);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
  return;
}

void SimSystemEditorWidget::highlightGraphicItemByName(const QString& itemName,
                                                          bool highlight,
                                                          bool unhighlightOtherItems)
{
  Q_UNUSED(itemName);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
  return;
}

void SimSystemEditorWidget::highlightGraphicItemsByNames(const QList<QString> &itemNames,
                                                            bool highlight,
                                                            bool unhighlightOtherItems)
{
  Q_UNUSED(itemNames);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
  return;
}

void SimSystemEditorWidget::selectedGrapicItemByUuidIn(const QUuid& uuid)
{
    qDebug()<<"simsystemeditorwidget"<<uuid;
    this->setFocus();
    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    QTextCursor editCursor = this->textCursor();
    this->setTextCursor(editCursor);
}

} //end namespace specsim
