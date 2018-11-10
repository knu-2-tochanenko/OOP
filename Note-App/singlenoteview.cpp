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
    ui->text->setText(this->note->getText());
    ui->tags->setText(this->note->getTags());
    ui->time->setText(this->note->getCreationTime().toString(Qt::TextDate) + " " +
                      this->note->getCreationDate().toString(Qt::TextDate));
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
