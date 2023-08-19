#include "dictionary.h"

#include <QTextStream>
Dictionary::Dictionary()
{

}


bool Dictionary::checkDict(const QString &word) const {
    return dictionary.find(word.toLower()) != dictionary.end();
}


void Dictionary::loadDict(std::ifstream& infile) {
    std::string str;
    while(std::getline(infile, str))
        dictionary[QString::fromStdString(str)] = 1;
}

void Dictionary::addError(const QString &text)
{
    errors.push_back(text);
}

QString Dictionary::getCorrected(const QString &text)
{
    for(int i = 0; i < errors.size(); ++i)
    {
        if(errors[i] == text)
        {
            lastChecked = i;
            return corrected[i];
        }
    }
    return "";
}

size_t Dictionary::getLastChecked(){
    return lastChecked;
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
        corrected.push_back(temp);
        clearSimilar();
        return temp;
    }

    for(auto &c : edits)
        changeTwo(c);

    if(!similar.isEmpty())
    {
        auto temp = similar[0];
        corrected.push_back(temp);
        clearSimilar();
        qDebug() << "edit2: " << temp;
        return temp;
    }
    else{
        corrected.push_back("");
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
    return word;
}

bool Dictionary::isWord(const QString & str) const {


    for (auto &c : str){
        if(!c.isLetter() && (c != '.' && c != '!' && c != '.' && c != '?' && c != ',' && c!= ';' && c!= ':' && c != '"' && c != ')' && c != '('))
            return false;
        if(c != str[0] && c.isUpper())
            return false;
    }
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

void Dictionary::clearErrors(){
    QVector<QString> temp;
    QVector<QString> temp2;
    std::swap(temp, errors);
    std::swap(temp2, corrected);
}

void Dictionary::reset(){}

