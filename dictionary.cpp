#include "dictionary.h"

Dictionary::Dictionary()
{

}


bool Dictionary::checkDict(const QString &word) const{
    return true;
}


void Dictionary::loadDict(std::ifstream& infile){
    std::string str;

    qDebug()<< "xd";

    while(std::getline(infile, str))
    {
        dictionary[QString::fromStdString(str)] = 1;
        qDebug() << "bruh";
    }

    QHashIterator<QString, int> i(dictionary);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key() << ": " << i.value() << Qt::endl;
    }
}

QString Dictionary::findSimilar(const QString& str){
    return "";
}

QString Dictionary::strip(QString str){
    return "";
}

bool Dictionary::isWord(const QString & str) const{
    return true;
}

void Dictionary::clear(){}

