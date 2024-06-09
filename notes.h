#ifndef NOTES_H
#define NOTES_H

#include <QFile>
#include "fstream"

class Notes
{
public:
    Notes(QString filepath = "");
    void addNote();
    void editNote();
    void deleteNote();
    void getNotes();
private:
    QFile file;
    std::fstream f;
};

#endif // NOTES_H
