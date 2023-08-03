#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>
#include <QHash>
#include <QDebug>
#include <fstream>
#include <string>
class Dictionary
{
public:
    /**
     *  @brief  Constructor for Dictionary
     */
    Dictionary();

//    /**
//     *  @brief  Subscript operator for checking if the
//     *          string paramater is in the dictionary.
//     *
//     *  @param  The QString to check
//     *
//     *  @return true if the Qstring is found, false otherwise
//     */
//    bool operator[](const QString &str);

//    /**
//     *  @brief  Const subscript operator for checking if the
//     *          string paramater is in the dictionary
//     *
//     *  @param  The QString to check
//     *
//     *  @return true if the Qstring is found, false otherwise
//     */
//    const bool operator[](const QString &str) const;

    /**
     *  @brief  Another version of operator[]. Checks if the string
     *          is in the dictionary
     *
     *  @param  The QString to check
     *
     *  @return true if the Qstring is found, false otherwise
     */
    bool checkDict(const QString &word) const;

    /**
     * @brief   loads the dictionary with words from a file
     *
     * @param   ifstream - the file that contains the words to be used
     */
    void loadDict(std::ifstream& infile);


    /**
     * @brief  Finds a similar word to the misspelled QString
     *
     * This function finds every permutation of the misspelled word, then
     * calls checkDict() to see if the permutation is a word. If it is,
     * returns said permutation. If not, returns the original input.
     *
     * @param  a missed spelled QString
     *
     * @return the similar word if it exists. Otherwise, returns str
     */
    QString findSimilar(const QString& str);

    QString strip(QString str);

    bool isWord(const QString & str) const;

    /**
     * @brief  Clears the dictionary.
     *         Used for user-inputted dictionary.
     */
    void clear();



private:
    QHash<QString, int> dictionary;
};

#endif // DICTIONARY_H
