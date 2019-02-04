#ifndef SINGLENOTEVIEW_H
#define SINGLENOTEVIEW_H

#include "singlenote.h"
#include <QWidget>

namespace Ui {
class singleNoteView;
}

class singleNoteView : public QWidget {
    Q_OBJECT

public:
    explicit singleNoteView(QWidget *parent = nullptr);
    ~singleNoteView();

    void setNote(SingleNote *sn);
    void showInformaion();
    int getID();

private:
    Ui::singleNoteView *ui;
    SingleNote *note;
    void paintEvent(QPaintEvent *);
};

#endif // SINGLENOTEVIEW_H
