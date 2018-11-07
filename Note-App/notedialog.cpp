#include "notedialog.h"
#include "ui_notedialog.h"

#include <QMessageBox>
#include <QDebug>

noteDialog::noteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::noteDialog) {
    ui->setupUi(this);
}

noteDialog::~noteDialog() {
    delete ui;
}

void noteDialog::addNote(SingleNote *note) {
    this->note = note;
    ui->noteText->setText(note->getText());
}

void noteDialog::addtags(QStringList tags) {
    this->tags = tags;
    displayTags();
}

SingleNote *noteDialog::getExecData() {
    int ID = this->note->getID();
    QTime creationTime = this->note->getCreationTime();
    QTime editedTime = QTime::currentTime();
    QDate creationDate = this->note->getCreationDate();
    QDate editedDate = QDate::currentDate();
    QString text = ui->noteText->toPlainText();
    QStringList tags = note->getTagsList();

    SingleNote *newNote = new SingleNote(ID, creationTime, creationDate, text, tags);
    newNote->setEditedTime(editedTime);
    newNote->setEditedDate(editedDate);
    return newNote;
}

void noteDialog::on_noteButton_addTag_clicked() {
    bool ok;
    QString tag = QInputDialog::getItem(this, tr("Add tag : "),
                                    tr("Tag:"), this->tags, 0, false, &ok);
    if (note->checkForTag(tag)) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","There is already a \"" + tag + "\" tag!");
        messageBox.setFixedSize(600,200);
    }
    else {
        this->note->addTag(tag);
    }
    displayTags();
}

void noteDialog::on_noteButton_delete_clicked() {
    // Make this note empty
    ui->noteText->setText("");
    this->note->setText(ui->noteText->toPlainText());
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->close();
}

void noteDialog::on_noteButton_deleteTag_clicked() {
    bool ok;
    QString tag = QInputDialog::getItem(this, tr("Add tag : "),
                                    tr("Tag:"), this->tags, 0, false, &ok);
    if (!this->note->checkForTag(tag)) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","There is already a \"" + tag + "\" tag!");
        messageBox.setFixedSize(600,200);
    }
    else {
        this->note->deleteTag(tag);
        if (this->note->getTagsList().size() == 0)
            this->note->addTag("uncategorized");
    }
    displayTags();
}

void noteDialog::on_noteButton_save_clicked() {
    this->note->setText(ui->noteText->toPlainText());
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->close();
}

void noteDialog::displayTags() {
    QStringList noteTags = this->note->getTagsList();
    int tagSize = noteTags.size();
    QString tagsString = "";
    if (tagSize != 0) {
        for (int i = 0; i < tagSize - 1; i++) {
            tagsString += noteTags[i] + ", ";
        }
        tagsString += noteTags[tagSize - 1];
    }
    else {
        this->note->addTag("uncategorized");
        tagsString = "uncategorized";
    }
    ui->label_tags_list->setText(tagsString);
}
