#include "archivednotes.h"
#include "ui_archivednotes.h"

#include <QMenu>

ArchivedNotes::ArchivedNotes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchivedNotes) {
    ui->setupUi(this);

    ui->listWidget_notes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_notes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showListMenu(QPoint)));
}

ArchivedNotes::~ArchivedNotes() {
    delete ui;
}

void ArchivedNotes::setNotes(QVector<SingleNote *> notesList) {
    this->notes = notesList;
    runInterface();
}

QVector<int> ArchivedNotes::getUnarchivedNotes() {
    return this->unarchivedNotes;
}

QVector<int> ArchivedNotes::getDeletedNotes() {
    return this->deletedNotes;
}

void ArchivedNotes::showListMenu(const QPoint &pos) {
    QPoint globalPos = ui->listWidget_notes->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Move from archive", this, SLOT(unarchiveNote()));
    myMenu.addAction("Delete Note",  this, SLOT(deleteNote()));

    myMenu.exec(globalPos);
}

void ArchivedNotes::deleteNote() {
    int listSize = ui->listWidget_notes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        int deleteID;
        int listWidgetSize = ui->listWidget_notes->count();
        for (int i = 0; i < listWidgetSize; i++)
            if (ui->listWidget_notes->item(i) == ui->listWidget_notes->selectedItems()[z]) {
                deleteID = (qobject_cast<singleNoteView*>(ui->listWidget_notes->itemWidget(ui->listWidget_notes->item(ui->listWidget_notes->currentRow()))))->getID();
            }
        this->deletedNotes.push_back(deleteID);
        int notesSize = notes.size();

        for (int i = 0; i < notesSize; i++)
            if (notes[i]->getID() == deleteID) {
                // TODO : Add dialog Y/N
                notes.erase(notes.begin() + i);
                break;
            }
        runInterface();
    }
}

void ArchivedNotes::unarchiveNote() {
    int listSize = ui->listWidget_notes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        int deleteID;
        int listWidgetSize = ui->listWidget_notes->count();
        for (int i = 0; i < listWidgetSize; i++)
            if (ui->listWidget_notes->item(i) == ui->listWidget_notes->selectedItems()[z]) {
                deleteID = (qobject_cast<singleNoteView*>(ui->listWidget_notes->itemWidget(ui->listWidget_notes->item(ui->listWidget_notes->currentRow()))))->getID();
            }

        this->unarchivedNotes.push_back(deleteID);
        int notesSize = notes.size();

        for (int i = 0; i < notesSize; i++)
            if (notes[i]->getID() == deleteID) {
                // TODO : Add dialog Y/N
                notes.erase(notes.begin() + i);
                break;
            }
        runInterface();
    }
}

void ArchivedNotes::runInterface() {
    int notesSize = this->notes.size();
    ui->listWidget_notes->clear();
    for (int i = 0; i < notesSize; i++) {
        // Add single widget
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listWidget_notes);
        ui->listWidget_notes->addItem(listWidgetItem);
        singleNoteView *snv = new singleNoteView;
        snv->setStyleSheet("singleNoteView {border-bottom: 1px solid #BDBDBD}");
        snv->setNote(this->notes[i]);
        listWidgetItem->setSizeHint(QSize(snv->sizeHint().width(), 85));
        ui->listWidget_notes->setItemWidget(listWidgetItem, snv);
    }
}



