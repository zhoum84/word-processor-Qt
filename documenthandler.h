#ifndef DOCUMENTHANDLER_H
#define DOCUMENTHANDLER_H

#include <QFont>
#include <QObject>
#include <QTextCursor>
#include <QUrl>
#include <QtQml/qqmlregistration.h>

#include "texthighlighter.h"
#include "dictionary.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
class QQuickTextDocument;
QT_END_NAMESPACE

class DocumentHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)

    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline NOTIFY underlineChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)

    Q_PROPERTY(int list READ list WRITE setList NOTIFY listChanged)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileType READ fileType NOTIFY fileUrlChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl NOTIFY fileUrlChanged)

    Q_PROPERTY(uint32_t count READ count NOTIFY countChanged)
    Q_PROPERTY(bool modified READ modified WRITE setModified NOTIFY modifiedChanged)
    QML_ELEMENT
public:
    explicit DocumentHandler(QObject *parent = nullptr);

    QQuickTextDocument *document() const;
    void setDocument(QQuickTextDocument *document);

    int cursorPosition() const;
    void setCursorPosition(int position);

    int selectionStart() const;
    void setSelectionStart(int position);

    int selectionEnd() const;
    void setSelectionEnd(int position);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    QFont font() const;
    void setFont(const QFont & font);

    int fontSize() const;
    void setFontSize(int size);

    bool bold() const;
    void setBold(bool bold);

    bool underline() const;
    void setUnderline(bool underline);

    bool italic() const;
    void setItalic(bool italic);

    int list() const;
    void setList(int list);

    QString fileName() const;
    QString fileType() const;
    QUrl fileUrl() const;

    uint32_t count();

    bool modified() const;
    void setModified(bool m);

    Q_INVOKABLE QVector<size_t> findTextInstances(const QString &text);
    Q_INVOKABLE void unhighlightText();
    Q_INVOKABLE void findAndHighlight(const QString& text);

    Q_INVOKABLE bool isMisspelled();
    Q_INVOKABLE QString getCorrectedWord();
    Q_INVOKABLE void replaceWord(const QString& suggest);

    void removeUnderline();
    Q_INVOKABLE void runSpellcheck();
    Q_INVOKABLE void spellcheck();

public Q_SLOTS:
    void load(const QUrl &fileUrl);
    void saveAs(const QUrl &fileUrl);

Q_SIGNALS:
    void documentChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();

    void fontChanged();
    void fontSizeChanged();

    void boldChanged();
    void underlineChanged();
    void italicChanged();
    void textColorChanged();
    void alignmentChanged();
    void listChanged();

    void textChanged();
    void fileUrlChanged();

    void loaded(const QString &text, int format);
    void error(const QString &message);

    void countChanged();
    void modifiedChanged();

private:
    void reset();
    QTextCursor textCursor() const;
    QTextDocument *textDocument() const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    //testing
    void timerEvent(QTimerEvent *event) override;

    QQuickTextDocument *m_document;

    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;


    QUrl m_fileUrl;

    QVector<size_t> positions;
    QVector<QTextCharFormat> formats;
    QString search;

    QVector<QPair<size_t, int>> misspelledPos;
    Dictionary dict;
};

#endif // DOCUMENTHANDLER_H
