#include "dictionary.h"

#include <QTextStream>
Dictionary::Dictionary()
{

}


bool Dictionary::checkDict(const QString &word) const {
    return dictionary.find(word) != dictionary.end();
}


void Dictionary::loadDict(std::ifstream& infile) {
    std::string str;
    while(std::getline(infile, str))
        dictionary[QString::fromStdString(str)] = 1;
}

QString Dictionary::findSimilar(const QString& str){
    return "";
}

QString Dictionary::stripWord(const QString& str) const {
    QString word;
    for(auto &c : str)
    {
        if(c.isLetter())
        {
            word.append(c);
        }
    }
    qDebug() << "stripped: " << word;
    return word;
}

bool Dictionary::isWord(const QString & str) const {
    if(str.length() == 1 && str[0].isLetter())
        return true;
    for (auto &c : str)
    {
        if(!c.isLetter() && (c != '.' && c != '!' && c != '.' && c != '?' && c != ',' && c!= ';' && c!= ':' && c != '"' && c != ')' && c != '('))
            return false;
    }
    return true;
}

void Dictionary::clear(){}

