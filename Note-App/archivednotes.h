#ifndef ARCHIVEDNOTES_H
#define ARCHIVEDNOTES_H

#include "singlenote.h"
#include "singlenoteview.h"
#include <QDialog>
#include <QVector>

namespace Ui {
class ArchivedNotes;
}

class ArchivedNotes : public QDialog
{
    Q_OBJECT

public:
    explicit ArchivedNotes(QWidget *parent = nullptr);
    ~ArchivedNotes();

    void setNotes(QVector<SingleNote*> notesList);
    QVector<int> getUnarchivedNotes();
    QVector<int> getDeletedNotes();

private slots:
    void showListMenu(const QPoint&);
    void deleteNote();
    void unarchiveNote();

private:
    Ui::ArchivedNotes *ui;
    QVector<SingleNote*> notes;

    QVector<int> unarchivedNotes;
    QVector<int> deletedNotes;

    void runInterface();

    bool checkYN(QString message, QString titile);

};

#endif // ARCHIVEDNOTES_H
