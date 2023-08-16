#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>


class textHighlighter : public QSyntaxHighlighter
{
public:

    explicit textHighlighter(QTextDocument *parent);
    ~textHighlighter();
    void highlightText(QString & str);
    void highlightText(int position, int length);
    void highlightBlock(const QString &text);
private:
    QTextDocument* parent;
};

#endif // TEXTHIGHLIGHTER_H
