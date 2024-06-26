#ifndef NOTES_H
#define NOTES_H

#include <QFile>
#include <QHash>
#include <QVector>
#include "fstream"

class Notes
{
public:
    Notes(QString filepath = "");
    bool addNote(const QString &nodeId, const QString& noteText);
    void editNote(const QString &noteId, const QString & edited);
    void deleteNote(const QString &noteId);
    bool checkNote(const QString &noteId);
    QString getNote(const QString& noteId) const;
    QHash<QString, QString> getNotes() const;
    void deleteAllNotes();
private:
    QFile file;
    std::fstream f;
    QString filepath;
    QHash<QString, QString> noteList;
};

#endif // NOTES_H
