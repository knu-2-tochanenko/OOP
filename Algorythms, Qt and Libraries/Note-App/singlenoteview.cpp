#include "singlenoteview.h"
#include "ui_singlenoteview.h"

#include <QPainter>
#include <QStyleOption>

singleNoteView::singleNoteView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::singleNoteView) {
    ui->setupUi(this);
}

singleNoteView::~singleNoteView() {
    delete ui;
}

void singleNoteView::setNote(SingleNote *sn) {
    this->note = sn;
    showInformaion();
}

void singleNoteView::showInformaion() {
    QString noteText = this->note->getText();
    int textSize = noteText.length();
    for (int i = 0; i < textSize; i++)
        if (noteText[i] == '\n') {
            noteText = noteText.left(i);
        }
    QFontMetrics metrics(ui->text->font());
    QString elidedText = metrics.elidedText(noteText, Qt::ElideRight, ui->text->width());
    ui->text->setText(elidedText);
    ui->tags->setText(this->note->getTags());
    ui->time->setText(this->note->getEditedTime().toString(Qt::TextDate) + " " +
                      this->note->getEditedDate().toString(Qt::TextDate));
}

int singleNoteView::getID() {
    return this->note->getID();
}

void singleNoteView::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter *p = new QPainter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, p, this);
}
