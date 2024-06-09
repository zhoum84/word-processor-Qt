#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>
#include <QHash>
#include <QSet>
#include <fstream>
#include <QFile>
#include <QVector>

/**
 * @brief The Dictionary class. Used for spellchecking.
 */
class Dictionary
{
public:
    /**
     *  @brief  Constructor for Dictionary
     */
    Dictionary();

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
     * @brief   loads user dictionary with words from added to a file
     *
     * @param   ifstream - the file that contains the words to be used
     */
    void loadUserDict(std::ifstream& infile);

    /**
     * @brief   adds specific word to user dictionary file
     *
     * @param   QString - the word to add
     */
    void addToUserDict(const QString& text);

    /**
     * @brief   wipes user dictionary file clean
     */
    void clearUserDict();

    QVector<QString> viewUserDict();

    void addError(const QString & text);

    /**
     * @brief getCorrected - finds the suggested correction
     *                       associated with a certain misspelled word.
     * @param text a misspelled word
     * @return the suggested correction or "" if nothing is found
     */
    QString getCorrected(const QString &text);

    void changeOne(const QString &word);

    void changeTwo(const QString &word);

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

    /**
     * @brief  Strips the word of all punctuation.
     *         Used for checking the dictionary file with the word.
     * @param  str - the string to strip
     * @return The stripped string.
     */
    QString stripWord(const QString &str) const;

    /**
     * @brief  Checks if the word consists of only letters and punctuation.
     *         Used to see if a word should be spellchecked.
     *
     * @param  str - the word to be checked.
     * @return True if there are only letters and punctuation. False otherwise.
     */
    bool isWord(const QString & str) const;

    QVector<QString> getSimilar();

    /**
     * @brief Gets last checked word in spellcheck. This is used for replacing a misspelled word with a correct one
     */

    size_t getLastChecked();

    /**
     * @brief  Clears list of edits and similar words.
     *         Used for every single misspelled text.
     */
    void clearSimilar();

    /**
     * @brief  Clears all recorded errors and corrected words.
     *         Used once for every spellcheck as a whole.
     */
    void clearErrors();



private:
    QHash<QString, int> dictionary;
    QSet<QString> userDictionary;
    QFile userDictFile;
    QVector<QString> errors;
    QVector<QString> corrected;
    QVector<QString> edits;
    QVector<QString> similar;
    size_t lastChecked;

};

#endif // DICTIONARY_H
