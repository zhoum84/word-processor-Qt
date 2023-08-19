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

    void addError(const QString & text);

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

    /**
     * @brief  Resets the dictionary.
     *         Used for user-inputted dictionary.
     */
    void reset();



private:
    QHash<QString, int> dictionary;
    QVector<QString> errors;
    QVector<QString> corrected;
    QVector<QString> edits;
    QVector<QString> similar;
    size_t lastChecked;
};

#endif // DICTIONARY_H
