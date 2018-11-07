#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H

#include "singlenote.h"
#include <QDialog>
#include <QInputDialog>

namespace Ui {
class noteDialog;
}

class noteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit noteDialog(QWidget *parent = nullptr);
    ~noteDialog();

    void addNote(SingleNote *sn);
    void addtags(QStringList tags);

    SingleNote* getExecData();

private slots:
    void on_noteButton_addTag_clicked();
    void on_noteButton_delete_clicked();
    void on_noteButton_deleteTag_clicked();
    void on_noteButton_save_clicked();

private:
    Ui::noteDialog *ui;
    SingleNote *note;
    QStringList tags;

    void displayTags();
    void displayTime();

};

#endif // NOTEDIALOG_H
