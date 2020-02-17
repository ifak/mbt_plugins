#include "irdleditorwidget.hpp"

#include "irdldocument.hpp"
#include "irdlhighlighter.hpp"
#include "irdlcompletionassist.hpp"
#include "irdloutlinemodel.hpp"
#include "combuildirdloutlinemodel.hpp"
#include "irdlgraphiceditor.hpp"
#include "irdleditorconstants.hpp"

#include <dslparser/dslerror.hpp>

#include <coreplugin/messagemanager.h>
#include <texteditor/fontsettings.h>
#include <dsleditor/dslindenter.hpp>
#include <dsleditor/dslautocompleter.hpp>
#include <utils/qtcassert.h>
#include <utils/uncommentselection.h>
#include <coreplugin/mimedatabase.h>

#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QTextBlock>
#include <QTimer>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace irdleditor{

enum {
    UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150
};

class IrdlEditorWidget::Private
{
    friend class IrdlEditorWidget;

    Private()
        : _irdlDocument(new IrdlDocument)
    {}

public:
    ~Private()
    {}

private:
    IrdlEditor*         _irdlEditor = nullptr;
    IrdlDocument::Ptr   _irdlDocument;
    QTimer*               _updateOutlineTimer = nullptr;
    QFile                 _irdlGraphicFile;
    IrdlGraphicWidget*  _irdlGraphicWidget = nullptr;
};

IrdlEditorWidget::IrdlEditorWidget(QWidget *parent)
    : DslEditorWidget(parent),
      _d(new Private())
{
    this->_d->_irdlEditor=new IrdlEditor(this);
    this->setBaseTextDocument(this->_d->_irdlDocument);

    this->baseTextDocument()->setSyntaxHighlighter(new IrdlHighlighter(this->document()));
    this->setIndenter(new dsleditor::DslIndenter());
    this->setAutoCompleter(new dsleditor::DslAutoCompleter()/*IrdlAutoCompleter()*/);

    this->_d->_updateOutlineTimer = new QTimer(this);
    this->_d->_updateOutlineTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
    this->_d->_updateOutlineTimer->setSingleShot(true);
    connect(this->_d->_updateOutlineTimer, &QTimer::timeout,
            this, &IrdlEditorWidget::updateOutlineNow);
    connect(this, &IrdlEditorWidget::textChanged,
            this, &IrdlEditorWidget::updateOutline);
    connect(this, &IrdlEditorWidget::cursorPositionChanged,
            this, &IrdlEditorWidget::updateOutlineIndexNow);
}

IrdlEditorWidget::~IrdlEditorWidget()
{
    if(this->_d->_irdlGraphicFile.exists())
    {
        this->_d->_irdlGraphicFile.close();
        this->_d->_irdlGraphicFile.remove();
    }

    delete this->_d;
}

void IrdlEditorWidget::duplicateFrom(BaseTextEditorWidget* editor)
{
    IrdlEditorWidget* otherEditorWidget = qobject_cast<IrdlEditorWidget*>(editor);
    Q_ASSERT(otherEditorWidget);

    BaseTextEditorWidget::duplicateFrom(otherEditorWidget);

    this->_d->_irdlGraphicFile.setFileName(otherEditorWidget->_d->_irdlGraphicFile.fileName());
    this->_d->_irdlGraphicWidget = otherEditorWidget->_d->_irdlGraphicWidget;

    return;
}

IrdlEditor* IrdlEditorWidget::createEditor()
{
    return this->_d->_irdlEditor;
}

void IrdlEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    return DslEditorWidget::setFontSettings(fs);
}

QVector<TextEditor::TextStyle> IrdlEditorWidget::highlighterFormatCategories()
{
    QVector<TextEditor::TextStyle> categories = DslEditorWidget::highlighterFormatCategories();
    categories << TextEditor::C_FUNCTION;

    return categories;
}

QString IrdlEditorWidget::wordUnderCursor() const
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

const IrdlDocument::Ptr& IrdlEditorWidget::irdlDocument() const
{
    return this->_d->_irdlDocument;
}

void IrdlEditorWidget::unCommentSelection()
{
    return Utils::unCommentSelection(this);
}

void IrdlEditorWidget::updateOutline()
{
    this->_d->_updateOutlineTimer->start();
}

QModelIndex IrdlEditorWidget::outlineModelIndex()
{
    QModelIndex currentModelIndex = this->indexForPosition(this->position());
    if(currentModelIndex.isValid())
        emit outlineModelIndexChanged(currentModelIndex);

    return currentModelIndex;
}

QTextCharFormat textCharFormat(int tokenType,
                               IrdlHighlighter* irdlHighlighter)
{
    Q_ASSERT(irdlHighlighter);

    if(tokenType == dslparser::Requirement)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Import)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Message)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Check)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::State)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Timer)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Alt)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Duration)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Declaration)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Actor)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Component)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Use)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::All)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Attribute)
        return irdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::Description)
        return irdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::As)
        return irdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::At)
        return irdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::Signal)
        return irdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::Trigger)
        return irdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::This)
        return irdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    //  else if(tokenType == dslparser::Function)
    //    return irdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::Port)
        return irdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::Place)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::SignalTransition)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::TimeoutTransition)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Type)
        return irdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Keyword)
        return irdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::UnknownRequirmentTokenType)
        return irdlHighlighter->textCharFormat(TextEditor::C_TEXT);

    return irdlHighlighter->textCharFormat(TextEditor::C_TEXT);
}

void IrdlEditorWidget::updateOutlineNow()
{
    qDebug()<<"IrdlEditorWidget::updateOutlineNow() executed!";

    this->_d->_updateOutlineTimer->stop();

    IrdlOutlineModel* irdlOutlineModel = this->_d->_irdlDocument->irdlOutlineModel();
    Q_ASSERT(irdlOutlineModel);

    QString currText=this->toPlainText();
    QString path = this->_d->_irdlEditor->document()->fileName();
    QString ending = path.split(".").last();
    path = path.mid(0, path.lastIndexOf(QString("/"))+1);

    Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
    Core::MimeType type = mdb->findByType(QString("application/irdl"));
    bool isIrdl = false;
    for(auto pattern : type.suffixes()){
        isIrdl |= pattern == ending;
    }
    dslparser::DslErrorList parsErrors;
    if(isIrdl){
        ComBuildIrdlOutlineModel buildOutlineCommand(currText, irdlOutlineModel,path);
        buildOutlineCommand.execute();
        parsErrors = buildOutlineCommand.errors();
    }
    else{
        ComBuildIrdlDeclOutlineModel buildOutlineCommand(currText, irdlOutlineModel,path);
        buildOutlineCommand.execute();
        parsErrors = buildOutlineCommand.errors();
    }
    //TODO: just a try, maybe we need 'updateOutlineIndexNow()'
    this->outlineModelIndex();

    /// first semantic highlights//////
    IrdlHighlighter* irdlHighlighter = qobject_cast<IrdlHighlighter*>(this->baseTextDocument()->syntaxHighlighter());
    Q_ASSERT(irdlHighlighter);

    QList<QTextEdit::ExtraSelection> extraSelections;
    for(const dslparser::TokenTextLocation& textLoc : irdlOutlineModel->keywordTextSelections())
    {
        if(!textLoc.isValid())
            continue;

        QTextCursor cursor(this->baseTextDocument()->document());
        cursor.setPosition(textLoc.start());
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

        QTextCharFormat textFormat = textCharFormat(textLoc.tokenType(), irdlHighlighter);
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
    for (const dslparser::DslError& irdlError : parsErrors)
    {
        if (! irdlError.line())
            continue;
        else if (errors.contains((const int)irdlError.line()))
            continue;

        errors.insert((const int)irdlError.line());

        QTextBlock block=document()->findBlockByLineNumber((int)irdlError.line()-1);
        QTextCursor cursor(block);
        cursor.setPosition(block.position()+irdlError.charPositionInLine());
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format = errorFormat;
        sel.format.setToolTip(irdlError.message());
        extraSelections.append(sel);
    }
    this->setExtraSelections(CodeWarningsSelection, extraSelections);

    if(this->_d->_irdlGraphicWidget)
        this->_d->_irdlGraphicWidget->updateIrdlGraphic();
    else
        qDebug()<<"this->_d->_irdlGraphicWidget == nullptr";

    return;
}

void IrdlEditorWidget::updateOutlineIndexNow()
{
    if (this->_d->_updateOutlineTimer->isActive())
        return; // updateOutlineNow will call this function soon anyway

    this->outlineModelIndex();

    return;
}

QModelIndex IrdlEditorWidget::indexForPosition(unsigned cursorPosition) const
{
    using namespace dslparser;

    IrdlOutlineModel const* irdlOutlineModel = this->_d->_irdlEditor->irdlOutlineModel();
    Q_ASSERT(irdlOutlineModel);

    const ModelTextLocations& modelTextLocations = irdlOutlineModel->modelTextSelections();
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

IAssistInterface* IrdlEditorWidget::createAssistInterface(AssistKind assistKind,
                                                          AssistReason assistReason) const
{
    if (assistKind == TextEditor::Completion)
        return new IrdlCompletionAssistInterface(this->document(),
                                                 this->position(),
                                                 editor()->document()->fileName(),
                                                 assistReason);
    return BaseTextEditorWidget::createAssistInterface(assistKind, assistReason);
}

void IrdlEditorWidget::showIrdlGraphic()
{
    QString tempIrdlGraphicFile = QDir::tempPath();
    if (!tempIrdlGraphicFile.endsWith(QLatin1Char('/')))
        tempIrdlGraphicFile += QLatin1Char('/');
    tempIrdlGraphicFile += QFileInfo(this->editorDocument()->fileName()).baseName()
            + QLatin1String(".irdlgraphic");

    if(this->_d->_irdlGraphicFile.exists())
    {
        this->_d->_irdlGraphicFile.close();
        this->_d->_irdlGraphicFile.rename(tempIrdlGraphicFile);
    }
    else
        this->_d->_irdlGraphicFile.setFileName(tempIrdlGraphicFile);

    QTC_ASSERT(this->_d->_irdlGraphicFile.open(QIODevice::ReadWrite), return;);
    this->_d->_irdlGraphicFile.close();

    qDebug()<<"temp-irdl-graphic-file: "<<this->_d->_irdlGraphicFile.fileName();

    Core::IEditor* editor=Core::EditorManager::instance()->openEditor(this->_d->_irdlGraphicFile.fileName(),
                                                                      Core::Id(Constants::IRDLGRAPHIC_ID),
                                                                      Core::EditorManager::OpenInOtherSplit);

    Core::EditorManager::instance()->gotoOtherSplit();

    IrdlGraphicEditor* irdlGraphicEditor = qobject_cast<IrdlGraphicEditor*>(editor);
    Q_ASSERT(irdlGraphicEditor);
    irdlGraphicEditor->setIrdlOutlineModel(this->irdlDocument()->irdlOutlineModel());
    irdlGraphicEditor->setIrdlDocument(this->irdlDocument().data());

    this->_d->_irdlGraphicWidget = irdlGraphicEditor->irdlGraphicWidget();
    Q_ASSERT(this->_d->_irdlGraphicWidget);
    connect(this->_d->_irdlGraphicWidget, &IrdlGraphicWidget::selectedItem,
            this, &IrdlEditorWidget::selectedGrapicItemByUuidIn);
    connect(this->_d->_irdlGraphicWidget, &IrdlGraphicWidget::destroyed,
            [=](){ this->_d->_irdlGraphicWidget->deleteLater(); this->_d->_irdlGraphicWidget = nullptr;});

    updateOutlineNow();

    return;
}

void IrdlEditorWidget::highlightGraphicItemByUuid(const QUuid& itemUuid,
                                                  bool highlight,
                                                  bool unhighlightOtherItems)
{
    if(this->_d->_irdlGraphicWidget)
        this->_d->_irdlGraphicWidget->highlightGraphicItemByUuid(itemUuid,
                                                                 highlight,
                                                                 unhighlightOtherItems);

    return;
}

void IrdlEditorWidget::highlightGraphicItemsByUuids(const QList<QUuid>& itemUuids,
                                                    bool highlight,
                                                    bool unhighlightOtherItems)
{
    if(this->_d->_irdlGraphicWidget)
        this->_d->_irdlGraphicWidget->highlightGraphicItemsByUuids(itemUuids,
                                                                   highlight,
                                                                   unhighlightOtherItems);

    return;
}

void IrdlEditorWidget::highlightGraphicItemByName(const QString& itemName,
                                                  bool highlight,
                                                  bool unhighlightOtherItems)
{
    if(this->_d->_irdlGraphicWidget)
        this->_d->_irdlGraphicWidget->highlightGraphicItemByName(itemName,
                                                                 highlight,
                                                                 unhighlightOtherItems);

    return;
}

void IrdlEditorWidget::highlightGraphicItemsByNames(const QList<QString> &itemNames,
                                                    bool highlight,
                                                    bool unhighlightOtherItems)
{
    if(this->_d->_irdlGraphicWidget)
        this->_d->_irdlGraphicWidget->highlightGraphicItemsByNames(itemNames,
                                                                   highlight,
                                                                   unhighlightOtherItems);

    return;
}
void IrdlEditorWidget::selectedGrapicItemByUuidIn(const QUuid& uuid)
{
    //qDebug()<<"irdleditorwidget"<<uuid;
    this->setFocus();
    QModelIndex index = this->_d->_irdlEditor->irdlOutlineModel()->uuidToIndex(uuid);
    if(index == this->_d->_irdlEditor->irdlOutlineModel()->index(0,0)){
      return;
    }
    const dslparser::TokenTextLocation* location = this->_d->_irdlEditor->irdlOutlineModel()->modelTextLocation(index);
    if(!location)
    {
      //qDebug()<<"text location error"<<"irdleditorwidget"<<"Z478";
      return;
    }
    Core::EditorManager *editorManager = Core::EditorManager::instance();
    editorManager->cutForwardNavigationHistory();
    editorManager->addCurrentPositionToNavigationHistory();

    QTextCursor editCursor = this->textCursor();
    editCursor.setPosition(location->start());
    this->setTextCursor(editCursor);
}
} //end namespace irdleditor
