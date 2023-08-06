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

void Dictionary::changeOne(QString word){
    QString letters = "abcdefghijklmnopqrstuvwxyz";

    //deletes
    for(int i = 0; i < word.length(); ++i)
    {
        auto temp = word.remove(i,1);
        if(dictionary.find(temp) != dictionary.end())
            edits.push_back(temp);
    }
    //transposes
    //replaces
    for(int i = 0; i < word.length(); ++i)
    {
        for(int j = 0; j < 26; ++j)
        {
            auto temp = word;
            temp[i] = letters[j];
            if(dictionary.find(temp) != dictionary.end())
                edits.push_back(temp);
        }
    }

    //inserts
    for(int i = 0; i < word.length() + 1; ++i)
        for(int j = 0; j < 26; ++j)
        {
            auto temp = word.insert(i, letters[i]);
            if(dictionary.find(temp) != dictionary.end())
                edits.push_back(temp);
        }

}

QString Dictionary::findSimilar(const QString& str){
    return "";
}

QString Dictionary::stripWord(const QString& str) const {
    QString word;

    for(auto &c : str)
        if(c.isLetter())
            word.append(c);

    qDebug() << "stripped: " << word;
    return word;
}

bool Dictionary::isWord(const QString & str) const {
    if(str.length() == 1 && str[0].isLetter())
        return true;

    for (auto &c : str)
        if(!c.isLetter() && (c != '.' && c != '!' && c != '.' && c != '?' && c != ',' && c!= ';' && c!= ':' && c != '"' && c != ')' && c != '('))
            return false;

    return true;
}

void Dictionary::clear(){}

