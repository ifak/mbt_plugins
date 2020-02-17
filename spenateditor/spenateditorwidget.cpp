#include "spenateditorwidget.hpp"

#include "spenatdocument.hpp"
#include "spenathighlighter.hpp"
#include "spenatcompletionassist.hpp"
#include "spenatoutlinemodel.hpp"
#include "combuildspenatoutlinemodel.hpp"
#include "spenatgraphiceditor.hpp"
#include "spenateditorconstants.hpp"

#include <coreplugin/messagemanager.h>

#include <texteditor/fontsettings.h>

#include <dsleditor/dslindenter.hpp>
#include <dsleditor/dslautocompleter.hpp>

#include <utils/qtcassert.h>
#include <utils/uncommentselection.h>

#include <mobatawidgets/spenat/declscene.hpp>

#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QTextBlock>
#include <QTimer>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace spenateditor{

enum {
  UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150
};

class SpenatEditorWidget::Private
{
  friend class SpenatEditorWidget;

  Private()
    : _spenatDocument(new SpenatDocument)
  {}

public:
  ~Private()
  {}

private:
  SpenatEditor*                 _spenatEditor = nullptr;
  SpenatDocument::Ptr           _spenatDocument;
  QTimer*                       _updateOutlineTimer = nullptr;
  QFile                         _spenatGraphicFile;
  QPointer<SpenatGraphicWidget> _spenatGraphicWidget = nullptr;
};

SpenatEditorWidget::SpenatEditorWidget(QWidget *parent)
  : DslEditorWidget(parent),
    _d(new Private())
{
  this->_d->_spenatEditor=new SpenatEditor(this);
  this->setBaseTextDocument(this->_d->_spenatDocument);

  this->baseTextDocument()->setSyntaxHighlighter(new SpenatHighlighter(this->document()));
  this->setIndenter(new dsleditor::DslIndenter());
  this->setAutoCompleter(new dsleditor::DslAutoCompleter());

  this->_d->_updateOutlineTimer = new QTimer(this);
  this->_d->_updateOutlineTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
  this->_d->_updateOutlineTimer->setSingleShot(true);
  connect(this->_d->_updateOutlineTimer, &QTimer::timeout,
          this, &SpenatEditorWidget::updateOutlineNow);
  connect(this, &SpenatEditorWidget::textChanged,
          this, &SpenatEditorWidget::updateOutline);
  connect(this, &SpenatEditorWidget::cursorPositionChanged,
          this, &SpenatEditorWidget::updateOutlineIndexNow);
}

SpenatEditorWidget::~SpenatEditorWidget()
{
  if(this->_d->_spenatGraphicFile.exists())
  {
    this->_d->_spenatGraphicFile.close();
    this->_d->_spenatGraphicFile.remove();
  }

  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->spenatGraphicEditor()->removeConnectedTextEditor(this->_d->_spenatEditor);

  delete this->_d;
}

void SpenatEditorWidget::duplicateFrom(BaseTextEditorWidget* editor)
{
  SpenatEditorWidget* otherEditorWidget = qobject_cast<SpenatEditorWidget*>(editor);
  Q_ASSERT(otherEditorWidget);

  BaseTextEditorWidget::duplicateFrom(otherEditorWidget);

  this->_d->_spenatGraphicFile.setFileName(otherEditorWidget->_d->_spenatGraphicFile.fileName());
  this->_d->_spenatGraphicWidget = otherEditorWidget->_d->_spenatGraphicWidget;

  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->spenatGraphicEditor()->addConnectedTextEditor(this->_d->_spenatEditor);

  return;
}

SpenatEditor* SpenatEditorWidget::createEditor()
{
  return this->_d->_spenatEditor;
}

void SpenatEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
  return DslEditorWidget::setFontSettings(fs);
}

QVector<TextEditor::TextStyle> SpenatEditorWidget::highlighterFormatCategories()
{
  QVector<TextEditor::TextStyle> categories = DslEditorWidget::highlighterFormatCategories();
  categories << TextEditor::C_FUNCTION;

  return categories;
}

const SpenatDocument::Ptr& SpenatEditorWidget::spenatDocument() const
{
  return this->_d->_spenatDocument;
}

void SpenatEditorWidget::unCommentSelection()
{
  return Utils::unCommentSelection(this);
}

void SpenatEditorWidget::updateOutline()
{
  this->_d->_updateOutlineTimer->start();
}

QModelIndex SpenatEditorWidget::outlineModelIndex()
{
  QModelIndex currentModelIndex = this->indexForPosition(this->position());
  if(currentModelIndex.isValid())
    emit outlineModelIndexChanged(currentModelIndex);

  return currentModelIndex;
}

QTextCharFormat textCharFormat(int tokenType,
                               SpenatHighlighter* spenatHighlighter)
{
  Q_ASSERT(spenatHighlighter);

  if(tokenType == dslparser::Spenat)
    return spenatHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Attribute)
    return spenatHighlighter->textCharFormat(TextEditor::C_FIELD);
  else if(tokenType == dslparser::Signal)
    return spenatHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Trigger)
    return spenatHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::This)
    return spenatHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Function)
    return spenatHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Port)
    return spenatHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::Place)
    return spenatHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::SignalTransition)
    return spenatHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::TimeoutTransition)
    return spenatHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Type)
    return spenatHighlighter->textCharFormat(TextEditor::C_TYPE);
  else if(tokenType == dslparser::Keyword)
    return spenatHighlighter->textCharFormat(TextEditor::C_KEYWORD);
  else if(tokenType == dslparser::UnknownSpenatTokenType)
    return spenatHighlighter->textCharFormat(TextEditor::C_TEXT);

  return spenatHighlighter->textCharFormat(TextEditor::C_TEXT);
}

void SpenatEditorWidget::updateOutlineNow()
{
  //  qDebug()<<"SpenatEditorWidget::updateOutlineNow() executed!";

  this->_d->_updateOutlineTimer->stop();

  SpenatOutlineModelPtr spenatOutlineModel = this->_d->_spenatDocument->spenatOutlineModel();
  Q_ASSERT(spenatOutlineModel);

  QString currText=this->toPlainText();
  ComBuildSpenatOutlineModel buildOutlineCommand(currText, spenatOutlineModel.data());
  buildOutlineCommand.execute();

  //TODO: just a try, maybe we need 'updateOutlineIndexNow()'
  this->outlineModelIndex();

  /// first semantic highlights//////
  SpenatHighlighter* spenatHighlighter = qobject_cast<SpenatHighlighter*>(this->baseTextDocument()->syntaxHighlighter());
  Q_ASSERT(spenatHighlighter);

  QList<QTextEdit::ExtraSelection> extraSelections;
  for(const dslparser::TokenTextLocation& textLoc : spenatOutlineModel->keywordTextSelections())
  {
    if(!textLoc.isValid())
      continue;

    QTextCursor cursor(this->baseTextDocument()->document());
    cursor.setPosition(textLoc.start());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextCharFormat textFormat = textCharFormat(textLoc.tokenType(), spenatHighlighter);
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
  for (const dslparser::DslError& spenatError : buildOutlineCommand.errors())
  {
    if (! spenatError.line())
      continue;
    else if (errors.contains((const int)spenatError.line()))
      continue;

    errors.insert((const int)spenatError.line());

    QTextBlock block=document()->findBlockByLineNumber((int)spenatError.line()-1);
    QTextCursor cursor(block);
    cursor.setPosition(block.position()+spenatError.charPositionInLine());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QTextEdit::ExtraSelection sel;
    sel.cursor = cursor;
    sel.format = errorFormat;
    sel.format.setToolTip(spenatError.message());
    extraSelections.append(sel);
  }
  this->setExtraSelections(CodeWarningsSelection, extraSelections);

  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->updateSpenatGraphic();

  return;
}

void SpenatEditorWidget::updateOutlineIndexNow()
{
  if (this->_d->_updateOutlineTimer->isActive())
    return; // updateOutlineNow will call this function soon anyway

  this->outlineModelIndex();

  return;
}

QModelIndex SpenatEditorWidget::indexForPosition(unsigned cursorPosition) const
{
  using namespace dslparser;

  const SpenatOutlineModelPtr spenatOutlineModel = this->_d->_spenatEditor->spenatOutlineModel();
  Q_ASSERT(spenatOutlineModel);

  const ModelTextLocations& modelTextLocations = spenatOutlineModel->modelTextSelections();
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

IAssistInterface* SpenatEditorWidget::createAssistInterface(AssistKind assistKind,
                                                            AssistReason assistReason) const
{
  if (assistKind == TextEditor::Completion)
    return new SpenatCompletionAssistInterface(this->document(),
                                               this->position(),
                                               editor()->document()->fileName(),
                                               assistReason);
  return BaseTextEditorWidget::createAssistInterface(assistKind, assistReason);
}

void SpenatEditorWidget::showSpenatGraphic()
{
  QString tempSpenatGraphicFile = QDir::tempPath();
  if (!tempSpenatGraphicFile.endsWith(QLatin1Char('/')))
    tempSpenatGraphicFile += QLatin1Char('/');
  tempSpenatGraphicFile += QFileInfo(this->editorDocument()->fileName()).baseName()
                           + QLatin1String(".spenatgraphic");

  if(this->_d->_spenatGraphicFile.exists())
  {
    this->_d->_spenatGraphicFile.close();
    this->_d->_spenatGraphicFile.rename(tempSpenatGraphicFile);
  }
  else
    this->_d->_spenatGraphicFile.setFileName(tempSpenatGraphicFile);

  QTC_ASSERT(this->_d->_spenatGraphicFile.open(QIODevice::ReadWrite), return;);
  this->_d->_spenatGraphicFile.close();

  Core::IEditor* editor=Core::EditorManager::instance()->openEditor(this->_d->_spenatGraphicFile.fileName(),
                                                                    Core::Id(Constants::SPENATGRAPHIC_ID),
                                                                    Core::EditorManager::OpenInOtherSplit);

  Core::EditorManager::instance()->gotoOtherSplit();

  SpenatGraphicEditor* spenatGraphicEditor = qobject_cast<SpenatGraphicEditor*>(editor);
  if(!spenatGraphicEditor)
    return;

  spenatGraphicEditor->setSpenatDeclModel(this->spenatDocument()->spenatOutlineModel());
  spenatGraphicEditor->setSpenatDocument(this->spenatDocument().data());
  spenatGraphicEditor->addConnectedTextEditor(this->_d->_spenatEditor);

  this->_d->_spenatGraphicWidget = spenatGraphicEditor->spenatGraphicWidget();
  Q_ASSERT(this->_d->_spenatGraphicWidget);
  connect(this->_d->_spenatGraphicWidget, &SpenatGraphicWidget::selectedItem,
          this, &SpenatEditorWidget::selectedGrapicItemByUuidIn);

  this->updateOutlineNow();

  return;
}

void SpenatEditorWidget::highlightGraphicItemByUuid(const QUuid& itemUuid,
                                                    bool highlight,
                                                    bool unhighlightOtherItems)
{
  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->highlightGraphicItemByUuid(itemUuid,
                                                               highlight,
                                                               unhighlightOtherItems);

  return;
}

void SpenatEditorWidget::highlightGraphicItemsByUuids(const QList<QUuid>& itemUuids,
                                                      bool highlight,
                                                      bool unhighlightOtherItems)
{
  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->highlightGraphicItemsByUuids(itemUuids,
                                                                 highlight,
                                                                 unhighlightOtherItems);

  return;
}

void SpenatEditorWidget::highlightGraphicItemByName(const QString& itemName,
                                                    bool highlight,
                                                    bool unhighlightOtherItems)
{
  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->highlightGraphicItemByName(itemName,
                                                               highlight,
                                                               unhighlightOtherItems);

  return;
}

void SpenatEditorWidget::highlightGraphicItemsByNames(const QList<QString> &itemNames,
                                                      bool highlight,
                                                      bool unhighlightOtherItems)
{
  if(this->_d->_spenatGraphicWidget)
    this->_d->_spenatGraphicWidget->highlightGraphicItemsByNames(itemNames,
                                                                 highlight,
                                                                 unhighlightOtherItems);

  return;
}

void SpenatEditorWidget::selectedGrapicItemByUuidIn(const QUuid& uuid)
{
  //    qDebug()<<"spenateditorwidget"<<uuid;
  QTC_ASSERT(this->_d->_spenatEditor->spenatOutlineModel(), return;);

  this->setFocus();
  QModelIndex index = this->_d->_spenatEditor->spenatOutlineModel().data()->uuidToIndex(uuid);
  const dslparser::TokenTextLocation* location = this->_d->_spenatEditor->spenatOutlineModel().data()->modelTextLocation(index);

  Core::EditorManager *editorManager = Core::EditorManager::instance();
  editorManager->cutForwardNavigationHistory();
  editorManager->addCurrentPositionToNavigationHistory();

  QTextCursor editCursor = this->textCursor();
  editCursor.setPosition(location->start());
  this->setTextCursor(editCursor);
}

} //end namespace spenateditor
