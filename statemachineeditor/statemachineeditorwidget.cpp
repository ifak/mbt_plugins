#include "statemachineeditorwidget.hpp"

#include "statemachinedocument.hpp"
#include "statemachinehighlighter.hpp"
#include "statemachinecompletionassist.hpp"
#include "statemachineoutlinemodel.hpp"
#include "combuildstatemachineoutlinemodel.hpp"
#include "statemachinegraphiceditor.hpp"
#include "statemachineeditorconstants.hpp"
#include "statemachineeditorpreviewgraph.hpp"

#include <coreplugin/messagemanager.h>
#include <texteditor/fontsettings.h>
#include <dsleditor/dslindenter.hpp>
#include <dsleditor/dslautocompleter.hpp>
#include <utils/qtcassert.h>
#include <utils/uncommentselection.h>

#include <projectexplorer/buildstep.h>

#include <graphlayout/layoutglobal.hpp>
#include <graphlayout/comlayoutdeclmodel.hpp>
#include <graphlayout/layoutgraph.hpp>
#include <graphlayout/comcreatestatemachinegraph.hpp>
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

namespace statemachineeditor{

enum {
  UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150
};

class StateMachineEditorWidget::Private
{
  friend class StateMachineEditorWidget;

  Private()
    : _stateMachineEditor(0),
      _statemachineDocument(new StateMachineDocument)
  {}

public:
  ~Private()
  {}

private:
  StateMachineEditor*                 _stateMachineEditor;
  StateMachineDocument::Ptr           _statemachineDocument;
  QTimer*                             _updateOutlineTimer;
  QFile                               _stateMachineGraphicFile;
  QPointer<StateMachineGraphicWidget> _stateMachineGraphicWidget = nullptr;
};

StateMachineEditorWidget::StateMachineEditorWidget(QWidget *parent)
  : DslEditorWidget(parent),
    _d(new Private())
{
  this->_d->_stateMachineEditor=new StateMachineEditor(this);
  this->setBaseTextDocument(this->_d->_statemachineDocument);

  this->baseTextDocument()->setSyntaxHighlighter(new StateMachineHighlighter(this->document()));
  this->setIndenter(new dsleditor::DslIndenter());
  this->setAutoCompleter(new dsleditor::DslAutoCompleter()/*StateMachineAutoCompleter()*/);

  this->_d->_updateOutlineTimer = new QTimer(this);
  this->_d->_updateOutlineTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
  this->_d->_updateOutlineTimer->setSingleShot(true);
  connect(this->_d->_updateOutlineTimer, &QTimer::timeout,
          this, &StateMachineEditorWidget::updateOutlineNow);
  connect(this, &StateMachineEditorWidget::textChanged,
          this, &StateMachineEditorWidget::updateOutline);
  connect(this, &StateMachineEditorWidget::cursorPositionChanged,
          this, &StateMachineEditorWidget::updateOutlineIndexNow);
}

StateMachineEditorWidget::~StateMachineEditorWidget()
{
  if(this->_d->_stateMachineGraphicFile.exists())
  {
    this->_d->_stateMachineGraphicFile.close();
    this->_d->_stateMachineGraphicFile.remove();
  }

  if(this->_d->_stateMachineGraphicWidget)
    this->_d->_stateMachineGraphicWidget
      ->stateMachineGraphicEditor()
      ->removeConnectedTextEditor(this->_d->_stateMachineEditor);

  delete this->_d;
}

void StateMachineEditorWidget::duplicateFrom(BaseTextEditorWidget* editor)
{
  StateMachineEditorWidget* otherEditorWidget = qobject_cast<StateMachineEditorWidget*>(editor);
  Q_ASSERT(otherEditorWidget);

  BaseTextEditorWidget::duplicateFrom(otherEditorWidget);

  this->_d->_stateMachineGraphicFile.setFileName(otherEditorWidget->_d->_stateMachineGraphicFile.fileName());
  this->_d->_stateMachineGraphicWidget = otherEditorWidget->_d->_stateMachineGraphicWidget;

  if(this->_d->_stateMachineGraphicWidget)
    this->_d->_stateMachineGraphicWidget
      ->stateMachineGraphicEditor()
      ->addConnectedTextEditor(this->_d->_stateMachineEditor);

  return;
}

StateMachineEditor* StateMachineEditorWidget::createEditor()
{
  return this->_d->_stateMachineEditor;
}

void StateMachineEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
  return DslEditorWidget::setFontSettings(fs);
}

QVector<TextEditor::TextStyle> StateMachineEditorWidget::highlighterFormatCategories()
{
  QVector<TextEditor::TextStyle> categories = DslEditorWidget::highlighterFormatCategories();
  categories << TextEditor::C_FUNCTION;

  return categories;
}

QString StateMachineEditorWidget::wordUnderCursor() const
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

const StateMachineDocument::Ptr& StateMachineEditorWidget::stateMachineDocument() const
{
  return this->_d->_statemachineDocument;
}

void StateMachineEditorWidget::unCommentSelection()
{
  return Utils::unCommentSelection(this);
}

void StateMachineEditorWidget::updateOutline()
{
  this->_d->_updateOutlineTimer->start();
}

QModelIndex StateMachineEditorWidget::outlineModelIndex()
{
  QModelIndex currentModelIndex = this->indexForPosition(this->position());
  if(currentModelIndex.isValid())
    emit outlineModelIndexChanged(currentModelIndex);

  return currentModelIndex;
}

QTextCharFormat textCharFormat(int tokenType,
                               StateMachineHighlighter* stateMachineHighlighter)
{
  Q_ASSERT(stateMachineHighlighter);

  if(tokenType == dslparser::StateMachine)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Attribute)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Guard)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::EntryAction)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::ExitAction)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Enabled)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Action)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Source)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Target)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::SimpleState)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::CompositeState)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::ConcurrentState)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::JunctionState)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::FinalState)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::InitState)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Region)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::StateRegion)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Signal)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Trigger)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::This)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Function)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Port)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Place)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::SignalTransition)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::TimeoutTransition)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::JunctionTransition)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Type)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Keyword)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::UnknownStateMachineTokenType)
    return stateMachineHighlighter->textCharFormat(TextEditor::C_TEXT);

  return stateMachineHighlighter->textCharFormat(TextEditor::C_TEXT);
}

void StateMachineEditorWidget::updateOutlineNow()
{
  qDebug()<<"StateMachineEditorWidget::updateOutlineNow() executed!";

  this->_d->_updateOutlineTimer->stop();

  StateMachineOutlineModelPtr stateMachineOutlineModel = this->_d->_statemachineDocument->statemachineOutlineModel();
  Q_ASSERT(stateMachineOutlineModel);

  QString currText=this->toPlainText();
  ComBuildStateMachineOutlineModel buildOutlineCommand(currText, stateMachineOutlineModel.data());
  buildOutlineCommand.execute();

  //TODO: just a try, maybe we need 'updateOutlineIndexNow()'
  this->outlineModelIndex();

  /// first semantic highlights//////
  StateMachineHighlighter* stateMachineHighlighter = qobject_cast<StateMachineHighlighter*>(this->baseTextDocument()->syntaxHighlighter());
  Q_ASSERT(stateMachineHighlighter);

  QList<QTextEdit::ExtraSelection> extraSelections;
  for(const dslparser::TokenTextLocation& textLoc : stateMachineOutlineModel->keywordTextSelections())
  {
    if(!textLoc.isValid())
      continue;

    QTextCursor cursor(this->baseTextDocument()->document());
    cursor.setPosition(textLoc.start());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextCharFormat textFormat = textCharFormat(textLoc.tokenType(), stateMachineHighlighter);
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
  for (const dslparser::DslError& statemachineError : buildOutlineCommand.errors())
  {
    if (! statemachineError.line())
      continue;
    else if (errors.contains((const int)statemachineError.line()))
      continue;

    errors.insert((const int)statemachineError.line());

    QTextBlock block=document()->findBlockByLineNumber((int)statemachineError.line()-1);
    QTextCursor cursor(block);
    cursor.setPosition(block.position()+statemachineError.charPositionInLine());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextEdit::ExtraSelection sel;
    sel.cursor = cursor;
    sel.format = errorFormat;
    sel.format.setToolTip(statemachineError.message());
    extraSelections.append(sel);
  }
  this->setExtraSelections(CodeWarningsSelection, extraSelections);

  if(this->_d->_stateMachineGraphicWidget){
    QString layoutFilePath = QFileInfo(this->editorDocument()->fileName()).filePath();
    layoutFilePath.replace(QFileInfo(this->editorDocument()->fileName()).suffix(), "layout", Qt::CaseSensitive);
    this->_d->_stateMachineGraphicWidget->updateStateMachineGraphic(layoutFilePath);
  }
	
  return;
}

void StateMachineEditorWidget::updateOutlineIndexNow()
{
  if (this->_d->_updateOutlineTimer->isActive())
    return; // updateOutlineNow will call this function soon anyway

  this->outlineModelIndex();

  return;
}

QModelIndex StateMachineEditorWidget::indexForPosition(unsigned cursorPosition) const
{
  using namespace dslparser;

  const StateMachineOutlineModelPtr stateMachineOutlineModel = this->_d->_stateMachineEditor->statemachineOutlineModel();
  Q_ASSERT(stateMachineOutlineModel);

  const ModelTextLocations& modelTextLocations = stateMachineOutlineModel->modelTextSelections();
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

IAssistInterface* StateMachineEditorWidget::createAssistInterface(AssistKind assistKind,
                                                                  AssistReason assistReason) const
{
  if (assistKind == TextEditor::Completion)
    return new StateMachineCompletionAssistInterface(this->document(),
                                                     this->position(),
                                                     editor()->document()->fileName(),
                                                     assistReason);

  return BaseTextEditorWidget::createAssistInterface(assistKind, assistReason);
}

void StateMachineEditorWidget::showStateMachineGraphic()
{
  QString tempStateMachineGraphicFile = QDir::tempPath();
  if (!tempStateMachineGraphicFile.endsWith(QLatin1Char('/')))
    tempStateMachineGraphicFile += QLatin1Char('/');
  tempStateMachineGraphicFile += QFileInfo(this->editorDocument()->fileName()).baseName()
                                 + QLatin1String(".statemachinegraphic");

  if(this->_d->_stateMachineGraphicFile.exists())
  {
    this->_d->_stateMachineGraphicFile.close();
    this->_d->_stateMachineGraphicFile.rename(tempStateMachineGraphicFile);
  }
  else
    this->_d->_stateMachineGraphicFile.setFileName(tempStateMachineGraphicFile);

  QTC_ASSERT(this->_d->_stateMachineGraphicFile.open(QIODevice::ReadWrite), return;);
  this->_d->_stateMachineGraphicFile.close();

  Core::IEditor* editor=Core::EditorManager::instance()->openEditor(this->_d->_stateMachineGraphicFile.fileName(),
                                                                    Core::Id(Constants::statemachineGRAPHIC_ID),
                                                                    Core::EditorManager::OpenInOtherSplit);

  Core::EditorManager::instance()->gotoOtherSplit();

  StateMachineGraphicEditor* stateMachineGraphicEditor = qobject_cast<StateMachineGraphicEditor*>(editor);
  if(!stateMachineGraphicEditor)
    return;

  stateMachineGraphicEditor->setStateMachineModel(this->stateMachineDocument()->statemachineOutlineModel());
  stateMachineGraphicEditor->setStateMachineDocument(this->stateMachineDocument().data());
  stateMachineGraphicEditor->addConnectedTextEditor(this->_d->_stateMachineEditor);

  this->_d->_stateMachineGraphicWidget = stateMachineGraphicEditor->stateMachineGraphicWidget();
  Q_ASSERT(this->_d->_stateMachineGraphicWidget);
  connect(this->_d->_stateMachineGraphicWidget, &StateMachineGraphicWidget::selectedItem,
          this, &StateMachineEditorWidget::selectedGrapicItemByUuidIn);
  updateOutlineNow();


  return;
}

void StateMachineEditorWidget::highlightGraphicItemByUuid(const QUuid& itemUuid,
                                                          bool highlight,
                                                          bool unhighlightOtherItems)
{
  if(this->_d->_stateMachineGraphicWidget)
    this->_d->_stateMachineGraphicWidget->highlightGraphicItemByUuid(itemUuid,
                                                                     highlight,
                                                                     unhighlightOtherItems);

  return;
}

void StateMachineEditorWidget::highlightGraphicItemsByUuids(const QList<QUuid>& itemUuids,
                                                            bool highlight,
                                                            bool unhighlightOtherItems)
{
  if(this->_d->_stateMachineGraphicWidget)
    this->_d->_stateMachineGraphicWidget->highlightGraphicItemsByUuids(itemUuids,
                                                                       highlight,
                                                                       unhighlightOtherItems);

  return;
}

void StateMachineEditorWidget::highlightGraphicItemByName(const QString& itemName,
                                                          bool highlight,
                                                          bool unhighlightOtherItems)
{
  if(this->_d->_stateMachineGraphicWidget)
    this->_d->_stateMachineGraphicWidget->highlightGraphicItemByName(itemName,
                                                                     highlight,
                                                                     unhighlightOtherItems);

  return;
}

void StateMachineEditorWidget::highlightGraphicItemsByNames(const QList<QString> &itemNames,
                                                            bool highlight,
                                                            bool unhighlightOtherItems)
{
  if(this->_d->_stateMachineGraphicWidget)
    this->_d->_stateMachineGraphicWidget->highlightGraphicItemsByNames(itemNames,
                                                                       highlight,
                                                                       unhighlightOtherItems);

  return;
}

void StateMachineEditorWidget::selectedGrapicItemByUuidIn(const QUuid& uuid)
{
    qDebug()<<"statemachineeditorwidget"<<uuid;
    this->setFocus();
    QModelIndex index = this->_d->_stateMachineEditor->statemachineOutlineModel().data()->uuidToIndex(uuid);
    if(index == this->_d->_stateMachineEditor->statemachineOutlineModel().data()->index(0,0))
    {
      return;
    }
    const dslparser::TokenTextLocation* location =
        this->_d->_stateMachineEditor->statemachineOutlineModel()->modelTextLocation(index);
    if(!location)
    {
      qDebug()<<"text location error"<<"statemachineeditorwidget"<<"Z485";
      return;
    }
    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    QTextCursor editCursor = this->textCursor();
    editCursor.setPosition(location->start());
    this->setTextCursor(editCursor);
}

} //end namespace statemachineeditor
