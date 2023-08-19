#include "documenthandler.h"

#include <QFile>
#include <QFileInfo>
#include <QFileSelector>
#include <QMimeDatabase>
#include <QQmlFile>
#include <QQmlFileSelector>
#include <QQuickTextDocument>
#include <QTextList>
#include <QTextCharFormat>
#include <QStringDecoder>
#include <QTextDocument>
#include <QDebug>
#include <QtConcurrent>

DocumentHandler::DocumentHandler(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
{
    std::ifstream file("wordprocessor/resources/enable1.txt");
    dict.loadDict(file);
}

QQuickTextDocument *DocumentHandler::document() const
{
    return m_document;
}

void DocumentHandler::setDocument(QQuickTextDocument *document)
{
    if (document == m_document)
        return;

    if (m_document)
        disconnect(m_document->textDocument(), &QTextDocument::modificationChanged, this, &DocumentHandler::modifiedChanged);
    m_document = document;

    if (m_document)
        connect(m_document->textDocument(), &QTextDocument::modificationChanged, this, &DocumentHandler::modifiedChanged);
    emit documentChanged();
}

int DocumentHandler::cursorPosition() const
{
    return m_cursorPosition;
}

void DocumentHandler::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;
    reset();
    emit cursorPositionChanged();
}

int DocumentHandler::selectionStart() const
{
    return m_selectionStart;
}

void DocumentHandler::setSelectionStart(int position)
{
    if (position == m_selectionStart)
        return;

    m_selectionStart = position;
    emit selectionStartChanged();
}

int DocumentHandler::selectionEnd() const
{
    return m_selectionEnd;
}

void DocumentHandler::setSelectionEnd(int position)
{
    if (position == m_selectionEnd)
        return;

    m_selectionEnd = position;
    emit selectionEndChanged();
}

QColor DocumentHandler::textColor() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = cursor.charFormat();
    return format.foreground().color();
}

void DocumentHandler::setTextColor(const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    mergeFormatOnWordOrSelection(format);
    emit textColorChanged();
}

Qt::Alignment DocumentHandler::alignment() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return Qt::AlignLeft;
    return textCursor().blockFormat().alignment();
}

void DocumentHandler::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();
    cursor.mergeBlockFormat(format);

    emit alignmentChanged();
}

QString DocumentHandler::fileName() const
{
    const QString filePath = QQmlFile::urlToLocalFileOrQrc(m_fileUrl);
    const QString fileName = QFileInfo(filePath).fileName();
    if (fileName.isEmpty())
        return QStringLiteral("untitled.txt");
    return fileName;
}

QString DocumentHandler::fileType() const
{
    return QFileInfo(fileName()).suffix();
}

QUrl DocumentHandler::fileUrl() const
{
    return m_fileUrl;
}

void DocumentHandler::load(const QUrl &fileUrl)
{
    if (fileUrl == m_fileUrl)
        return;

    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "load() called before DocumentHandler has QQmlEngine";
        return;
    }

    const QUrl path = engine->interceptUrl(fileUrl, QQmlAbstractUrlInterceptor::UrlString);
    const QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
    if (QFile::exists(fileName)) {
        QMimeType mime = QMimeDatabase().mimeTypeForFile(fileName);
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();
            if (QTextDocument *doc = textDocument()) {
                doc->setBaseUrl(path.adjusted(QUrl::RemoveFilename));
                doc->setModified(false);
                QTextOption textOptions = doc->defaultTextOption();
                textOptions.setTabStopDistance(35);
                doc->setDefaultTextOption(textOptions);
                const QSizeF page(612,792);
                doc->setPageSize(page);
                if (mime.inherits("text/markdown"))
                {
                    emit loaded(QString::fromUtf8(data), Qt::MarkdownText);
                }
                else
                {
                    auto encoding = QStringConverter::encodingForHtml(data);
                    if (encoding) {
                        QStringDecoder decoder(*encoding);
                        emit loaded(decoder(data), Qt::AutoText);
                    } else {
                        // fall back to utf8
                        emit loaded(QString::fromUtf8(data), Qt::AutoText);
                    }
                }
            }

            reset();
        }
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

void DocumentHandler::saveAs(const QUrl &fileUrl)
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    const QString filePath = fileUrl.toLocalFile();
    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
    file.close();

    if (fileUrl == m_fileUrl)
        return;

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

void DocumentHandler::reset()
{
    emit alignmentChanged();
    emit textColorChanged();
    emit fontChanged();
}

QTextCursor DocumentHandler::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

QTextDocument *DocumentHandler::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}

void DocumentHandler::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
//    if (!cursor.hasSelection())
//        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}

uint32_t DocumentHandler::count(){
    uint32_t size = 0, counter = 0;
    auto text = m_document->textDocument()->toPlainText();

    if(text.isEmpty())
        return 0;

    if(!text.at(0).isLetterOrNumber())
        ++counter;
    for(int i = 1; i<text.length(); ++i)
    {
        if(text.at(i-1).isSpace() && text.at(i).isLetterOrNumber())
            ++counter;
    }
    return counter;
}

bool DocumentHandler::modified() const
{
    return m_document && m_document->textDocument()->isModified();
}

void DocumentHandler::setModified(bool m)
{
    if (m_document)
        m_document->textDocument()->setModified(m);
}

QFont DocumentHandler::font() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return m_document->textDocument()->defaultFont();
    QTextCharFormat format = cursor.charFormat();
    return format.font();
}

//change signature later
void DocumentHandler::setFont(const QFont & font){

    QTextCursor cursor = textCursor();
    if (!cursor.isNull() && cursor.charFormat().font() == font)
        return;

    auto temp = font;

    // maintain document font size
    if(font.pointSize() > 0)
    {
        temp.setPointSize(cursor.charFormat().font().pointSize());
    }
    else if(font.pixelSize() > 0){
        temp.setPixelSize(cursor.charFormat().font().pixelSize());
    }

    QTextCharFormat format;
    format.setFont(temp);

    mergeFormatOnWordOrSelection(format);

    emit fontChanged();
}

int DocumentHandler::fontSize() const{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return m_document->textDocument()->defaultFont().pointSize();
    return cursor.charFormat().fontPointSize();

}

void DocumentHandler::setFontSize(int size){
    QTextCursor cursor = textCursor();

    if(size < 1)
        return;
    if (!cursor.isNull() && cursor.charFormat().fontPointSize() == size)
        return;

    auto temp = cursor.charFormat().font();
    temp.setPointSize(size);
    QTextCharFormat format;
    format.setFont(temp);

    mergeFormatOnWordOrSelection(format);

    emit fontSizeChanged();
}

bool DocumentHandler::bold() const
{
    const QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return m_document->textDocument()->defaultFont().bold();
    return cursor.charFormat().font().bold();
}

void DocumentHandler::setBold(bool bold)
{
    const QTextCursor cursor = textCursor();
    if (!cursor.isNull() && cursor.charFormat().font().bold() == bold)
        return;

    QFont font = cursor.charFormat().font();
    font.setBold(bold);
    QTextCharFormat format;
    format.setFont(font);
    mergeFormatOnWordOrSelection(format);

    emit boldChanged();
}

bool DocumentHandler::underline() const
{
    const QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return m_document->textDocument()->defaultFont().underline();
    return cursor.charFormat().font().underline();
}

void DocumentHandler::setUnderline(bool underline)
{
    const QTextCursor cursor = textCursor();
    if (!cursor.isNull() && cursor.charFormat().font().underline() == underline)
        return;

    QFont font = cursor.charFormat().font();
    font.setUnderline(underline);
    QTextCharFormat format;
    format.setFont(font);
    mergeFormatOnWordOrSelection(format);

    emit underlineChanged();
}

bool DocumentHandler::italic() const
{
    const QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return m_document->textDocument()->defaultFont().italic();
    return cursor.charFormat().font().italic();
}

void DocumentHandler::setItalic(bool italic)
{
    const QTextCursor cursor = textCursor();
    if (!cursor.isNull() && cursor.charFormat().font().italic() == italic)
        return;

    QFont font = cursor.charFormat().font();
    font.setItalic(italic);
    QTextCharFormat format;
    format.setFont(font);
    mergeFormatOnWordOrSelection(format);

    emit italicChanged();
}

int DocumentHandler::list() const{

    const QTextCursor cursor = textCursor();
    QTextList *list = cursor.currentList();
    //QTextListFormat::Style style = QTextListFormat::ListDisc;

    if(list){
        QTextListFormat::Style style2 = list->format().style();
        return style2;
    }
    else
        return 0;
}

// need to improve
void DocumentHandler::setList(const int list){
    QTextCursor cursor = textCursor();
    QTextListFormat listFormat;
    QTextList *listed = cursor.currentList();
    if(!listed){
        listFormat.setStyle( QTextListFormat::Style(list));
        cursor.createList( listFormat );
    }
    else
    {
        listFormat.setIndent(0);
        listFormat.setStyle(QTextListFormat::Style(0) );
        if(cursor.selectedText().isEmpty())
        {
            listed->setFormat(listFormat);
            //int count = cursor.block().blockNumber() - 3;
            int count = listed->count() - 2;
            listed->removeItem(count);
        }
        else
        {
            listed->setFormat( listFormat );
            //for( int i = listed->count() - 1; i >= 0 ; --i )
                listed->removeItem( listed->count() - 1);
        }
    }
    emit listChanged();
}

Q_INVOKABLE QVector<size_t> DocumentHandler::findTextInstances(const QString &text){
    auto doc = m_document->textDocument()->toPlainText();
    QVector<size_t> instances;
    QTextCursor cursor = textCursor();

    size_t s = 0;
    while((s = doc.indexOf(text, s, Qt::CaseInsensitive)) != -1)
    {
        instances.push_back(s);
        cursor.setPosition(s);
        formats.push_back(cursor.charFormat());
        ++s;
    }

    return instances;
}

Q_INVOKABLE void DocumentHandler::unhighlightText(){
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setBackground(Qt::transparent);

    std::for_each(positions.begin(), positions.end(), [&cursor, &format, this](auto &c){
        cursor.setPosition(c);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, search.length());
        cursor.mergeCharFormat(format);
    });
}

Q_INVOKABLE void DocumentHandler::findAndHighlight(const QString& text){

    unhighlightText();
    positions.clear();
    formats.clear();

    search = text;
    positions = findTextInstances(text);
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setBackground(Qt::yellow);

    std::for_each(positions.begin(), positions.end(), [&cursor, &format, &text](auto &c){
        cursor.setPosition(c);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, text.length());
        cursor.mergeCharFormat(format);
    });
}

void DocumentHandler::timerEvent(QTimerEvent *event){
    qDebug() << event->timerId();
}

void DocumentHandler::removeUnderline(){
    QTextCursor cursor = textCursor();
    QTextCharFormat fmt;
    fmt.setFontUnderline(false);

    std::for_each(misspelledPos.begin(), misspelledPos.end(), [&cursor, &fmt](auto &c){
        cursor.setPosition(c.first);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, c.second);
        cursor.mergeCharFormat(fmt);
    });
}

Q_INVOKABLE bool DocumentHandler::isMisspelled(){
    QTextCharFormat format = textCursor().charFormat();
    return format.underlineColor() == Qt::red;
}

Q_INVOKABLE QString DocumentHandler::getCorrectedWord(){
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString misspell = cursor.selectedText();
    return dict.getCorrected(misspell);
}

Q_INVOKABLE void DocumentHandler::replaceWord(const QString& suggest){
    size_t p = dict.getLastChecked();
    QPair<int, size_t> pos = misspelledPos[p];
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos.first);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, pos.second);
    cursor.insertText(suggest);
}

Q_INVOKABLE void DocumentHandler::runSpellcheck(){
    QFuture<void> future = QtConcurrent::run([this]{return spellcheck();});
    future.waitForFinished();
}

Q_INVOKABLE void DocumentHandler::spellcheck(){

    removeUnderline();
    misspelledPos.clear();
    dict.clearSimilar();
    QString doc = m_document->textDocument()->toPlainText();
    QTextCursor cursor = textCursor();
    QTextCharFormat wavy;

    wavy.setUnderlineColor(Qt::red);
    //WavyUnderline does not work, so SingleUnderline for now.
    wavy.setUnderlineStyle(QTextCharFormat::SingleUnderline);

    // Sliding window for identifying words. Used because stringstream doesn't track indexes
    int i = 0, j = 1;
    while(doc[j] != ' ' && j < doc.length())
        ++j;
    while(j < doc.length())
    {
        while(!doc[i].isLetter() && i < j)
            ++i;
        if(i == 0 || (doc[j] == ' ' && doc[i] != ' '))
        {
            QString temp = (doc.sliced(i, j - i));
            if(dict.isWord(temp))
            {
                QString cleaned = dict.stripWord(temp);
                if(!cleaned.isEmpty() && !dict.checkDict(cleaned)){
                    dict.addError(cleaned);
                    dict.findSimilar(cleaned.toLower());
                    misspelledPos.push_back({i, temp.size()});
                    cursor.setPosition(i);
                    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, cleaned.length());
                    cursor.mergeCharFormat(wavy);
                }
            }
            i = j;
        }
        ++j;
    }

}

#include "moc_documenthandler.cpp"
