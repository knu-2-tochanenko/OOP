#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QTime>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Right button for list
    ui->list_tags->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->list_tags, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    ui->list_tags->addItem("uncategorized");
    ui->list_tags->addItem("work");
    ui->list_tags->addItem("university");
    this->tags.push_back("work");
    this->tags.push_back("university");

    if (!readJSON("backup.json")) {
        // TODO : If the is no safe
        qDebug() << "There is no file\n";
    }
}

MainWindow::~MainWindow() {
    if (!writeJSON("backup.json"))
        qDebug()<<"Backup was corrapted!\n";
    delete ui;
}

// Funciton to add new item
void MainWindow::addTagFunction() {
    bool ok;
    QString tagText = QInputDialog::getText(this, tr("Create New Tag"), tr("Tag name:"), QLineEdit::Normal, "default", &ok);
    tagText = tagText.toLower();
    if (ok && !tagText.isEmpty()) {
        // Chek for similar tag
        int numberOfTags = ui->list_tags->count();
        bool hasSame = false;
        for (int i = 0; i < numberOfTags; i++)
            if (ui->list_tags->item(i)->text() == tagText)
                hasSame = true;
        if (!hasSame) {
            ui->list_tags->addItem(tagText);
            this->tags.push_back(tagText);
        }
        else {
            // If tag is already available
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Error","There is already a \"" + tagText + "\" tag!");
            messageBox.setFixedSize(600,200);
        }
    }
}

void MainWindow::on_button_newTag_clicked() {
    addTagFunction();
}

void MainWindow::on_button_backup_clicked() {
    // TODO : Make backup function
}

void MainWindow::on_button_newNote_clicked() {
    bool ok;
    QString noteText = QInputDialog::getMultiLineText(this, tr("New Note"), tr("Note text:"), "default", &ok);
    QTime creatingTime = QTime::currentTime();
    QDate creationDate = QDate::currentDate();
    QString text = noteText;
    QStringList noteTags;
    int ID = this->maxID + 1;
    this->maxID++;

    // Get list of tags
    bool moreTags = true;
    while (moreTags) {
        // Check if user wants to put some tags
        if (checkYN()) {
            // Add new tag
            QString tag = QInputDialog::getItem(this, tr("Add tag : "),
                                            tr("Tag:"), this->tags, 0, false, &ok);
            if (noteTags.contains(tag)) {
                QMessageBox messageBox;
                messageBox.critical(nullptr,"Error","There is already a \"" + tag + "\" tag!");
                messageBox.setFixedSize(600,200);
            }
            else
                noteTags.push_back(tag);
        }
        else {
            // End adding tags
            if (noteTags.size() == 0)
                noteTags.push_back("uncategorized");
            moreTags = false;
        }
    }

    SingleNote *sn = new SingleNote(ID, creatingTime, creationDate, text, noteTags);
    this->notes.push_back(sn);
    debugNote(sn);
    // TODO : Make updating notes table
}

void MainWindow::on_button_toggleArchive_clicked() {
    // TODO : Make opening archive window
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = ui->list_tags->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("New tag", this, SLOT(addTagItem()));
    myMenu.addAction("Edit", this, SLOT(editTagItem()));
    myMenu.addAction("Add to filter", this, SLOT(addTagToFilter()));
    myMenu.addAction("Delete",  this, SLOT(deleteTagItem()));

    myMenu.exec(globalPos);
}

void MainWindow::addTagItem() {
    addTagFunction();
}

void MainWindow::editTagItem() {
    // TODO : Make edit funciton
}

void MainWindow::addTagToFilter() {
    // TODO : Make add to filter funcion
}

bool MainWindow::readJSON(QString filePath) {
    bool correctJSON = true;
    QFile readNotesFile;
    readNotesFile.setFileName(filePath);
    if (!readNotesFile.open(QFile::ReadOnly | QFile::Text))
        return false;

    QString jsonData = readNotesFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObject = document.object();

    // Get array of notes
    if (jsonObject.contains("notes") && jsonObject["notes"].isArray()) {
        QJsonArray jsonArray = jsonObject["notes"].toArray();
        int arraySize = jsonArray.count();
        for (int i = 0; i < arraySize; i++) {
            SingleNote *note = new SingleNote(jsonArray[i].toObject());
            this->notes.push_back(note);
        }
    }
    else {
        correctJSON = false;
    }

    // Get array of archived notes
    if (jsonObject.contains("archive") && jsonObject["archive"].isArray()) {
        QJsonArray jsonArray = jsonObject["archive"].toArray();
        int arraySize = jsonArray.count();
        for (int i = 0; i < arraySize; i++) {
            SingleNote *note = new SingleNote(jsonArray[i].toObject());
            this->archive.push_back(note);
        }
    }
    else {
        correctJSON = false;
    }


    // Get max ID
    if (jsonObject.contains("max_id") && jsonObject["max_id"].isDouble())
        this->maxID = jsonObject["max_id"].toInt();
    else {
        correctJSON = false;
    }

    // Get list of new tags
    if (jsonObject.contains("tags") && jsonObject["tags"].isArray()) {
        QJsonArray tagArray = jsonObject["tags"].toArray();
        int tagsSize = tagArray.size();
        for (int i = 0; i < tagsSize; i++)
            this->tags.push_back(tagArray[i].toString());
    }

    readNotesFile.close();
    return true;
}

bool MainWindow::writeJSON(QString filePath) {
    QFile writeNotesFile;
    writeNotesFile.setFileName(filePath);
    if (!writeNotesFile.open(QFile::WriteOnly))
        return false;

    // Add notes
    QJsonArray notesArray;
    int notesSize = notes.size();
    QJsonObject singleNoteObject;
    for (int i = 0; i < notesSize; i++) {
        notes[i]->writeJSON(singleNoteObject);
        notesArray.push_back(singleNoteObject);
    }

    // Add archived notes
    QJsonArray archiveArray;
    int archiveSize = archive.size();
    QJsonObject archiveObject;
    for (int i = 0; i < archiveSize; i++) {
        notes[i]->writeJSON(archiveObject);
        notesArray.push_back(archiveObject);
    }

    // Add tags
    QJsonObject finalObject;
    QJsonArray tagArray;
    int tagArraySize = this->tags.size();
    for (int i = 0; i < tagArraySize; i++) {
        tagArray.push_back(this->tags[i]);
    }

    finalObject["notes"] = notesArray;
    finalObject["max_id"] = this->maxID;
    finalObject["tags"] = tagArray;
    finalObject["archive"] = archiveObject;


    QJsonDocument document(finalObject);
    writeNotesFile.write(document.toJson());
    writeNotesFile.close();
    return true;
}

bool MainWindow::checkYN() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Add tags");
    msgBox.setText("Would you like to add tag?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes)
      return true;
    else
        return false;
}

void MainWindow::debugNote(SingleNote *sn) {
    qDebug() << "ID : " << sn->getID()
             << "\nCreation time : " << sn->getCreationTime()
             << "\nCreation date : " << sn->getCreationDate()
             << "\nEdited time : " << sn->getEditedTime()
             << "\nEdited date : " << sn->getEditedDate()
             << "\nText : " << sn->getText()
             << "\nTags : " << sn->getTags();
}

void MainWindow::updateTabel(QStringList tags) {
    // TODO : You know what to do
    if (tags.size() == 0) {
        // Display all notes
    }
    else {
        // Display selected notes
    }
}

bool MainWindow::moveToArchive(int ID) {
    int notesSize = notes.size();
    for (int i = 0; i < notesSize; i++) {
        if (notes[i]->getID() == ID) {
            archive.push_back(notes[i]);
            notes.erase(notes.begin() + i);
            return true;
        }
    }
    return false;
}

bool MainWindow::moveFromArchive(int ID) {
    int archiveSize = notes.size();
    for (int i = 0; i < archiveSize; i++) {
        if (archive[i]->getID() == ID) {
            notes.push_back(archive[i]);
            archive.erase(archive.begin() + i);
            return true;
        }
    }
    return false;
}

void MainWindow::deleteTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        if (ui->list_tags->selectedItems()[i]->text() == "uncategorized"
                || ui->list_tags->selectedItems()[i]->text() == "work"
                || ui->list_tags->selectedItems()[i]->text() == "university") {
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Error","You can't delete \"" + ui->list_tags->selectedItems()[i]->text() + "\" tag!");
            messageBox.setFixedSize(600,200);
        }
        else {
            QListWidgetItem *item = ui->list_tags->takeItem(ui->list_tags->currentRow());
            item->text();
            // TODO : Unlinck all notes from deleted tag
            delete item;
        }
    }
}
