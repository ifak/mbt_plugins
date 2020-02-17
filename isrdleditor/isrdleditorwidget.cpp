#include "isrdleditorwidget.hpp"

#include "isrdldocument.hpp"
#include "isrdlautocompleter.hpp"
#include "isrdlcompletionassist.hpp"
#include "isrdloutlinemodel.hpp"
#include "combuildisrdloutlinemodel.hpp"
//#include "isrdlgraphiceditor.hpp"
#include "isrdleditorconstants.hpp"
#include "dslparser/dslerror.hpp"
#include <dsleditor/dslhighlighter.hpp>
#include <dsleditor/dslindenter.hpp>
#include <dsleditor/dslautocompleter.hpp>

#include <coreplugin/messagemanager.h>
#include <texteditor/fontsettings.h>
#include <utils/qtcassert.h>
#include <utils/uncommentselection.h>
#include <coreplugin/mimedatabase.h>

//#include <mobatawidgets/isrdl/declscene.hpp>

#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QTextBlock>
#include <QTimer>
#include <QDebug>

#include <mobata/memory_leak_start.hpp>

using namespace TextEditor;

namespace isrdleditor{

enum {
    UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150
};

class IsrdlEditorWidget::Private
{
    friend class IsrdlEditorWidget;

    Private()
        : _isrdlDocument(new IsrdlDocument)
    {}

public:
    ~Private()
    {}

private:
    IsrdlEditor*         _isrdlEditor = nullptr;
    IsrdlDocument::Ptr   _isrdlDocument;
    QTimer*               _updateOutlineTimer = nullptr;
//    QFile                 _isrdlGraphicFile;
//    IsrdlGraphicWidget*  _isrdlGraphicWidget = nullptr;
};

IsrdlEditorWidget::IsrdlEditorWidget(QWidget *parent)
    : DslEditorWidget(parent),
      _d(new Private())
{
    this->_d->_isrdlEditor=new IsrdlEditor(this);
    this->setBaseTextDocument(this->_d->_isrdlDocument);

    this->baseTextDocument()->setSyntaxHighlighter(new dsleditor::DslHighlighter(this->document()));
    this->setIndenter(new dsleditor::DslIndenter());
    this->setAutoCompleter(new dsleditor::DslAutoCompleter()/*IsrdlAutoCompleter()*/);

    this->_d->_updateOutlineTimer = new QTimer(this);
    this->_d->_updateOutlineTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
    this->_d->_updateOutlineTimer->setSingleShot(true);
    connect(this->_d->_updateOutlineTimer, &QTimer::timeout,
            this, &IsrdlEditorWidget::updateOutlineNow);
    connect(this, &IsrdlEditorWidget::textChanged,
            this, &IsrdlEditorWidget::updateOutline);
    connect(this, &IsrdlEditorWidget::cursorPositionChanged,
            this, &IsrdlEditorWidget::updateOutlineIndexNow);
}

IsrdlEditorWidget::~IsrdlEditorWidget()
{
//    if(this->_d->_isrdlGraphicFile.exists())
//    {
//        this->_d->_isrdlGraphicFile.close();
//        this->_d->_isrdlGraphicFile.remove();
//    }

    delete this->_d;
}

void IsrdlEditorWidget::duplicateFrom(BaseTextEditorWidget* editor)
{
    IsrdlEditorWidget* otherEditorWidget = qobject_cast<IsrdlEditorWidget*>(editor);
    Q_ASSERT(otherEditorWidget);

    BaseTextEditorWidget::duplicateFrom(otherEditorWidget);

//    this->_d->_isrdlGraphicFile.setFileName(otherEditorWidget->_d->_isrdlGraphicFile.fileName());
//    this->_d->_isrdlGraphicWidget = otherEditorWidget->_d->_isrdlGraphicWidget;

    return;
}

IsrdlEditor* IsrdlEditorWidget::createEditor()
{
    return this->_d->_isrdlEditor;
}

void IsrdlEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    return DslEditorWidget::setFontSettings(fs);
}

QVector<TextEditor::TextStyle> IsrdlEditorWidget::highlighterFormatCategories()
{
    QVector<TextEditor::TextStyle> categories = DslEditorWidget::highlighterFormatCategories();
    categories << TextEditor::C_FUNCTION;

    return categories;
}

QString IsrdlEditorWidget::wordUnderCursor() const
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

const IsrdlDocument::Ptr& IsrdlEditorWidget::isrdlDocument() const
{
    return this->_d->_isrdlDocument;
}

void IsrdlEditorWidget::unCommentSelection()
{
    return Utils::unCommentSelection(this);
}

void IsrdlEditorWidget::updateOutline()
{
    this->_d->_updateOutlineTimer->start();
}

QModelIndex IsrdlEditorWidget::outlineModelIndex()
{
    QModelIndex currentModelIndex = this->indexForPosition(this->position());
    if(currentModelIndex.isValid())
        emit outlineModelIndexChanged(currentModelIndex);

    return currentModelIndex;
}

QTextCharFormat textCharFormat(int tokenType,
                               dsleditor::DslHighlighter* isrdlHighlighter)
{
    Q_ASSERT(isrdlHighlighter);

    if(tokenType == dslparser::AttackVectorEnum)
        return isrdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::IntegrityThreat)
        return isrdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::Source)
        return isrdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::IntegrityEnum)
        return isrdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::ConfidentialityThreat)
        return isrdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::ConfidentialityEnum)
        return isrdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::Privileges)
        return isrdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::PrivilegesEnum)
        return isrdlHighlighter->textCharFormat(TextEditor::C_KEYWORD);
    else if(tokenType == dslparser::Impact)
        return isrdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::SecurityRequirement)
        return isrdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Timeout)
        return isrdlHighlighter->textCharFormat(TextEditor::C_FIELD);
    else if(tokenType == dslparser::Description)
        return isrdlHighlighter->textCharFormat(TextEditor::C_TYPE);
    else if(tokenType == dslparser::Attribute)
        return isrdlHighlighter->textCharFormat(TextEditor::C_FIELD);

    return isrdlHighlighter->textCharFormat(TextEditor::C_TEXT);
}

void IsrdlEditorWidget::updateOutlineNow()
{
    qDebug()<<"IsrdlEditorWidget::updateOutlineNow() executed!";

    this->_d->_updateOutlineTimer->stop();

    IsrdlOutlineModel* isrdlOutlineModel = this->_d->_isrdlDocument->isrdlOutlineModel();
    Q_ASSERT(isrdlOutlineModel);

    QString currText=this->toPlainText();
    QString path = this->_d->_isrdlEditor->document()->fileName();
    QString ending = path.split(".").last();
    path = path.mid(0, path.lastIndexOf(QString("/"))+1);

    Core::MimeDatabase* mdb=Core::ICore::instance()->mimeDatabase();
    Core::MimeType type = mdb->findByType(QString("application/isrdl"));
    bool isIsrdl = false;
    for(auto pattern : type.suffixes()){
        isIsrdl |= pattern == ending;
    }
    dslparser::DslErrorList parsErrors;

    ComBuildIsrdlOutlineModel buildOutlineCommand(currText, isrdlOutlineModel,path);
    buildOutlineCommand.execute();
    parsErrors = buildOutlineCommand.errors();

    //TODO: just a try, maybe we need 'updateOutlineIndexNow()'
    this->outlineModelIndex();

    /// first semantic highlights//////
    dsleditor::DslHighlighter* isrdlHighlighter = qobject_cast<dsleditor::DslHighlighter*>(this->baseTextDocument()->syntaxHighlighter());
    Q_ASSERT(isrdlHighlighter);

    QList<QTextEdit::ExtraSelection> extraSelections;
    for(const dslparser::TokenTextLocation& textLoc : isrdlOutlineModel->keywordTextSelections())
    {
        if(!textLoc.isValid())
            continue;

        QTextCursor cursor(this->baseTextDocument()->document());
        cursor.setPosition(textLoc.start());
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

        QTextCharFormat textFormat = textCharFormat(textLoc.tokenType(), isrdlHighlighter);
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
    for (const dslparser::DslError& isrdlError : parsErrors)
    {
        if (! isrdlError.line())
            continue;
        else if (errors.contains((const int)isrdlError.line()))
            continue;

        errors.insert((const int)isrdlError.line());

        QTextBlock block=document()->findBlockByLineNumber((int)isrdlError.line()-1);
        QTextCursor cursor(block);
        cursor.setPosition(block.position()+isrdlError.charPositionInLine());
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format = errorFormat;
        sel.format.setToolTip(isrdlError.message());
        extraSelections.append(sel);
    }
    this->setExtraSelections(CodeWarningsSelection, extraSelections);

//    if(this->_d->_isrdlGraphicWidget)
//        this->_d->_isrdlGraphicWidget->updateIsrdlGraphic();
//    else
//        qDebug()<<"this->_d->_isrdlGraphicWidget == nullptr";

    return;
}

void IsrdlEditorWidget::updateOutlineIndexNow()
{
    if (this->_d->_updateOutlineTimer->isActive())
        return; // updateOutlineNow will call this function soon anyway

    this->outlineModelIndex();

    return;
}

QModelIndex IsrdlEditorWidget::indexForPosition(unsigned cursorPosition) const
{
    using namespace dslparser;

    IsrdlOutlineModel const* isrdlOutlineModel = this->_d->_isrdlEditor->isrdlOutlineModel();
    Q_ASSERT(isrdlOutlineModel);

    const ModelTextLocations& modelTextLocations = isrdlOutlineModel->modelTextSelections();
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

IAssistInterface* IsrdlEditorWidget::createAssistInterface(AssistKind assistKind,
                                                          AssistReason assistReason) const
{
    if (assistKind == TextEditor::Completion)
        return new IsrdlCompletionAssistInterface(this->document(),
                                                 this->position(),
                                                 editor()->document()->fileName(),
                                                 assistReason);
    return BaseTextEditorWidget::createAssistInterface(assistKind, assistReason);
}

void IsrdlEditorWidget::showIsrdlGraphic()
{
//    QString tempIsrdlGraphicFile = QDir::tempPath();
//    if (!tempIsrdlGraphicFile.endsWith(QLatin1Char('/')))
//        tempIsrdlGraphicFile += QLatin1Char('/');
//    tempIsrdlGraphicFile += QFileInfo(this->editorDocument()->fileName()).baseName()
//            + QLatin1String(".isrdlgraphic");

//    if(this->_d->_isrdlGraphicFile.exists())
//    {
//        this->_d->_isrdlGraphicFile.close();
//        this->_d->_isrdlGraphicFile.rename(tempIsrdlGraphicFile);
//    }
//    else
//        this->_d->_isrdlGraphicFile.setFileName(tempIsrdlGraphicFile);

//    QTC_ASSERT(this->_d->_isrdlGraphicFile.open(QIODevice::ReadWrite), return;);
//    this->_d->_isrdlGraphicFile.close();

//    qDebug()<<"temp-isrdl-graphic-file: "<<this->_d->_isrdlGraphicFile.fileName();

//    Core::IEditor* editor=Core::EditorManager::instance()->openEditor(this->_d->_isrdlGraphicFile.fileName(),
//                                                                      Core::Id(Constants::ISRDLGRAPHIC_ID),
//                                                                      Core::EditorManager::OpenInOtherSplit);

//    Core::EditorManager::instance()->gotoOtherSplit();

//    IsrdlGraphicEditor* isrdlGraphicEditor = qobject_cast<IsrdlGraphicEditor*>(editor);
//    Q_ASSERT(isrdlGraphicEditor);
//    isrdlGraphicEditor->setIsrdlOutlineModel(this->isrdlDocument()->isrdlOutlineModel());
//    isrdlGraphicEditor->setIsrdlDocument(this->isrdlDocument().data());

//    this->_d->_isrdlGraphicWidget = isrdlGraphicEditor->isrdlGraphicWidget();
//    Q_ASSERT(this->_d->_isrdlGraphicWidget);

//    connect(this->_d->_isrdlGraphicWidget, &IsrdlGraphicWidget::destroyed,
//            [=](){ this->_d->_isrdlGraphicWidget->deleteLater(); this->_d->_isrdlGraphicWidget = nullptr;});

//    updateOutlineNow();

    return;
}

void IsrdlEditorWidget::highlightGraphicItemByUuid(const QUuid& itemUuid,
                                                  bool highlight,
                                                  bool unhighlightOtherItems)
{
  Q_UNUSED(itemUuid);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
//    if(this->_d->_isrdlGraphicWidget)
//        this->_d->_isrdlGraphicWidget->highlightGraphicItemByUuid(itemUuid,
//                                                                 highlight,
//                                                                 unhighlightOtherItems);

//    return;
}

void IsrdlEditorWidget::highlightGraphicItemsByUuids(const QList<QUuid>& itemUuids,
                                                    bool highlight,
                                                    bool unhighlightOtherItems)
{
  Q_UNUSED(itemUuids);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
//    if(this->_d->_isrdlGraphicWidget)
//        this->_d->_isrdlGraphicWidget->highlightGraphicItemsByUuids(itemUuids,
//                                                                   highlight,
//                                                                   unhighlightOtherItems);

//    return;
}

void IsrdlEditorWidget::highlightGraphicItemByName(const QString& itemName,
                                                  bool highlight,
                                                  bool unhighlightOtherItems)
{
  Q_UNUSED(itemName);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
//    if(this->_d->_isrdlGraphicWidget)
//        this->_d->_isrdlGraphicWidget->highlightGraphicItemByName(itemName,
//                                                                 highlight,
//                                                                 unhighlightOtherItems);

//    return;
}

void IsrdlEditorWidget::highlightGraphicItemsByNames(const QList<QString> &itemNames,
                                                    bool highlight,
                                                    bool unhighlightOtherItems)
{
  Q_UNUSED(itemNames);
  Q_UNUSED(highlight);
  Q_UNUSED(unhighlightOtherItems);
//    if(this->_d->_isrdlGraphicWidget)
//        this->_d->_isrdlGraphicWidget->highlightGraphicItemsByNames(itemNames,
//                                                                   highlight,
//                                                                   unhighlightOtherItems);

//    return;
}

} //end namespace isrdleditor
