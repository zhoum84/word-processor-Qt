#include "notes.h"

Notes::Notes(QString filepath)
{
    std::fstream f(filepath.toStdString());
}

bool Notes::addNote(const QString &noteId, const QString& noteText){
    if(noteList.contains(noteId))
        return false;
    noteList[noteId] = noteText;
    return true;
}
void Notes::editNote(const QString &noteId, const QString & edited){
    noteList[noteId] = edited;
}
void Notes::deleteNote(const QString &noteId){
    noteList.remove(noteId);

}

bool Notes::checkNote(const QString &noteId){
    return noteList.contains(noteId);
}

QString Notes::getNote(const QString& noteId) const{
    if(noteList.contains(noteId))
        return noteList[noteId];
    return "";
}
QHash<QString, QString> Notes::getNotes() const{
    return noteList;
}
