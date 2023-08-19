#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>

//This is a class for better text highlighting. Currently WIP.
class textHighlighter : public QSyntaxHighlighter
{
public:
    explicit textHighlighter(QTextDocument *parent);
    ~textHighlighter();
    void unHighlight();
    void highlightText(QString & str);
    void highlightText(int position, int length);
    void highlightBlock(const QString &text);
private:
    QTextDocument* parent;
    QVector<size_t> pos;
};

#endif // TEXTHIGHLIGHTER_H
