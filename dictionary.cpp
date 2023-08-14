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

void Dictionary::changeOne(const QString &word){
    QString letters = "abcdefghijklmnopqrstuvwxyz";
    //deletes
    for(int i = 0; i < word.length(); ++i)
    {
        auto temp = word;
        temp.remove(i,1);
        if(dictionary.find(temp) != dictionary.end()){
            similar.push_back(temp);
        }
        edits.push_back(temp);
    }
    //transposes
    for(int i = 0; i < word.length() - 1; ++i)
    {
        for(int j = i + 1; j < word.length(); ++j)
        {
            auto temp = word;
            std::swap(temp[i], temp[j]);
            if(dictionary.find(temp) != dictionary.end()){
                similar.push_back(temp);
            }
            edits.push_back(temp);
        }
    }

    //replaces
    for(int i = 0; i < word.length(); ++i)
    {
        for(int j = 0; j < 26; ++j)
        {
            auto temp = word;
            temp[i] = letters[j];
            if(dictionary.find(temp) != dictionary.end())
            {
                similar.push_back(temp);
            }
            edits.push_back(temp);
        }
    }

    //inserts
    for(int i = 0; i < word.length() + 1; ++i){
        for(int j = 0; j < 26; ++j)
        {
            auto temp = word;
            temp.insert(i, letters[j]);
            if(dictionary.find(temp) != dictionary.end()){
                similar.push_back(temp);
            }
            edits.push_back(temp);
        }
    }
}

void Dictionary::changeTwo(const QString &word){
    QString letters = "abcdefghijklmnopqrstuvwxyz";
    //deletes
    for(int i = 0; i < word.length(); ++i)
    {
        auto temp = word;
        temp.remove(i,1);
        if(dictionary.find(temp) != dictionary.end()){
            similar.push_back(temp);
        }
    }
    //transposes
    for(int i = 0; i < word.length() - 1; ++i)
    {
        for(int j = i + 1; j < word.length(); ++j)
        {
            auto temp = word;
            std::swap(temp[i], temp[j]);
            if(dictionary.find(temp) != dictionary.end()){
                similar.push_back(temp);
            }
        }
    }

    //replaces
    for(int i = 0; i < word.length(); ++i)
    {
        for(int j = 0; j < 26; ++j)
        {
            auto temp = word;
            temp[i] = letters[j];
            if(dictionary.find(temp) != dictionary.end())
            {
                similar.push_back(temp);
            }
        }
    }

    //inserts
    for(int i = 0; i < word.length() + 1; ++i){
        for(int j = 0; j < 26; ++j)
        {
            auto temp = word;
            temp.insert(i, letters[j]);
            if(dictionary.find(temp) != dictionary.end()){
                similar.push_back(temp);
            }
        }
    }
}

QString Dictionary::findSimilar(const QString& str){
    changeOne(str);

    if(!similar.isEmpty())
    {
        qDebug() << "edit1: " << similar[0];
        auto temp = similar[0];
        clearSimilar();
        return temp;
    }

    for(auto &c : edits)
        changeTwo(c);

    if(!similar.isEmpty())
    {
        auto temp = similar[0];
        clearSimilar();
        qDebug() << "edit2: " << temp;
        return temp;
    }
    else{
        clearSimilar();
        qDebug() << str;
        return str;
    }

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

    for (auto &c : str)
        if(!c.isLetter() && (c != '.' && c != '!' && c != '.' && c != '?' && c != ',' && c!= ';' && c!= ':' && c != '"' && c != ')' && c != '('))
            return false;

    return true;
}

QVector<QString> Dictionary::getSimilar(){
    return similar;
}
void Dictionary::clearSimilar(){
    QVector<QString> temp;
    QVector<QString> temp2;
    std::swap(temp, edits);
    std::swap(temp2, similar);
}

void Dictionary::reset(){}

