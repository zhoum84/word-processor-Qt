#include "texthighlighter.h"

textHighlighter::textHighlighter(QTextDocument *parent) :QSyntaxHighlighter(parent), parent(parent)
{

}

textHighlighter::~textHighlighter(){

}

void textHighlighter::highlightText(QString &text){
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    setFormat(0, text.size(), myClassFormat);
}

void textHighlighter::highlightText(int position, int length){
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    setFormat(position, length, myClassFormat);

}

#include <QRegularExpression>
void textHighlighter::highlightBlock(const QString &text){

    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);

    QRegularExpression expression("\\b[A-Za-z]+\\b");
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    setFormat(0, 50, myClassFormat);
//    while (i.hasNext()) {
//        QRegularExpressionMatch match = i.next();
//        setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
//    }

}
