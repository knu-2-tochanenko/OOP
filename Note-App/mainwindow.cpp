#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "notedialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QTime>
#include <QDate>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //Theme
    QFile f("../qdarkstyle/style.qss");
    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    //Theme

    // Right button for list
    ui->list_tags->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->list_tags, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    ui->table_notes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->table_notes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenu(QPoint)));

    ui->list_tags->addItem("uncategorized");
    ui->list_tags->addItem("work");
    ui->list_tags->addItem("personal");
    this->tags.push_back("work");
    this->tags.push_back("personal");

    if (!readJSON("backup.json")) {
        // If the is no safe file
        this->maxID = 0;
        qDebug() << "There is no file\n";
    }
    ui->table_notes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_notes->horizontalHeader()->setStretchLastSection(true);
    runInterface();
}

MainWindow::~MainWindow() {
    if (!writeJSON("backup.json")) {
        qDebug()<<"Backup was corrapted!\n";
    }
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
    // Choose backup folder
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    dir += "backup.txt";

    QFile backupFile(dir);
    backupFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&backupFile);

    // Write all content:
    out << "Main Notes:\n";
    int notesSize = this->notes.size();
    for (int i = 0; i < notesSize; i++) {
        out << this->notes[i]->getText() << "\n";
        out << "ID : " << this->notes[i]->getID() << "\n";
        out << "Tags : " << this->notes[i]->getTags() << "\n";
        out << "Edited on : " << this->notes[i]->getEditedTime().toString(Qt::TextDate)
            << " " << this->notes[i]->getEditedDate().toString(Qt::TextDate) << "\n";
        out << "Created on : " << this->notes[i]->getCreationTime().toString(Qt::TextDate)
            << " " << this->notes[i]->getCreationDate().toString(Qt::TextDate) << "\n";
        out << "----------------\n\n";
    }

    out << "Archived Notes:\n";
    int archiveSize = this->archive.size();
    for (int i = 0; i < archiveSize; i++) {
        out << this->archive[i]->getText() << "\n";
        out << "ID : " << this->archive[i]->getID() << "\n";
        out << "Tags : " << this->archive[i]->getTags() << "\n";
        out << "Edited on : " << this->archive[i]->getEditedTime().toString(Qt::TextDate)
            << " " << this->archive[i]->getEditedDate().toString(Qt::TextDate) << "\n";
        out << "Created on : " << this->archive[i]->getCreationTime().toString(Qt::TextDate)
            << " " << this->archive[i]->getCreationDate().toString(Qt::TextDate) << "\n";
        out << "----------------\n\n";
    }

    out << "All tags : uncategorized, work, personal";
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize; i++) {
        out << ", " << this->tags[i];
    }

    backupFile.flush();
    backupFile.close();
}

void MainWindow::on_button_newNote_clicked() {
    QTime creatingTime = QTime::currentTime();
    QDate creationDate = QDate::currentDate();
    QString text = "";
    QStringList noteTags;
    int ID = this->maxID + 1;
    this->maxID++;
    SingleNote *sn = new SingleNote(ID, creatingTime, creationDate, text, noteTags);
    noteDialog *nd = new noteDialog(this);
    nd->addNote(sn);
    nd->addtags(this->tags);
    nd->exec();
    if (sn->getText() == "") {
        // Don't need to do smth
    }
    else {
        this->notes.push_back(sn);
    }
    //}
    debugNote(sn);
    runInterface();
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

void MainWindow::showTableMenu(const QPoint &pos) {
    QPoint globalPos = ui->table_notes->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Edit Note", this, SLOT(editNote()));
    myMenu.addAction("Move to archive", this, SLOT(moveToArchive()));
    myMenu.addAction("Delete Note",  this, SLOT(deleteNote()));

    myMenu.exec(globalPos);
}

void MainWindow::addTagItem() {
    addTagFunction();
}

void MainWindow::editTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int k = 0; k < listSize; ++k) {
        if (ui->list_tags->selectedItems()[k]->text() == "uncategorized"
                || ui->list_tags->selectedItems()[k]->text() == "work"
                || ui->list_tags->selectedItems()[k]->text() == "personal") {
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Error","You can't edit \"" + ui->list_tags->selectedItems()[k]->text() + "\" tag!");
            messageBox.setFixedSize(600,200);
        }
        else {
            QString tagToEdit = ui->list_tags->selectedItems()[k]->text();
            int tagSize = tags.size();
            for (int i = 0; i < tagSize; i++)
                if (tags[i] == tagToEdit) {
                    bool ok;
                    QString tagText = QInputDialog::getText(this, tr("Create New Tag"), tr("Tag name:"), QLineEdit::Normal, "default", &ok);
                    tagText = tagText.toLower();

                    if (ok && !tagText.isEmpty()) {
                        // Chek for similar tag
                        int numberOfTags = ui->list_tags->count();
                        bool hasSame = false;
                        for (int i = 0; i < numberOfTags; i++)
                            if (ui->list_tags->item(k)->text() == tagText)
                                hasSame = true;
                        if (!hasSame) {
                            for (int j = 0; j < tagSize; j++) {
                                if (tags[j] == tagToEdit) {
                                    tags[j] = tagText;
                                    ui->list_tags->selectedItems()[k]->setText(tagText);
                                }
                            }
                        }
                        else {
                            // If tag is already available
                            QMessageBox messageBox;
                            messageBox.critical(nullptr,"Error","There is already a \"" + tagText + "\" tag!");
                            messageBox.setFixedSize(600,200);
                        }
                    }
                    break;
                }
        }
    }
}

void MainWindow::addTagToFilter() {

    if (ui->list_tags->selectedItems()[0]->text() == "uncategorized" && filter.size() > 0)
        filter.clear();
    else {
        int listSize = ui->list_tags->selectedItems().size();
        for (int i = 0; i < listSize; ++i) {
            if (!filter.contains(ui->list_tags->selectedItems()[i]->text()))
                filter.push_back(ui->list_tags->selectedItems()[i]->text());
            else {
                QMessageBox messageBox;
                messageBox.critical(nullptr,"Error","There is already \"" + ui->list_tags->selectedItems()[i]->text() + "\" tag!");
                messageBox.setFixedSize(600,200);
            }
        }
    }
    runInterface();
}

void MainWindow::editNote() {
    int listSize = ui->table_notes->selectedItems().size();
    int currentRow = ui->table_notes->selectedItems()[0]->row();
    int notesSize = notes.size();
    SingleNote *noteToEdit;

    int noteID = ui->table_notes->item(currentRow, 0)->text().toInt();
    for (int i = 0; i < notesSize; i++)
        if (notes[i]->getID() == noteID) {
            noteToEdit = notes[i];
            break;
        }
    QTime creatingTime = noteToEdit->getCreationTime();
    QDate creationDate = noteToEdit->getCreationDate();

    QString text = noteToEdit->getText();

    QStringList noteTags = noteToEdit->getTagsList();
    int ID = noteToEdit->getID();
    SingleNote *sn = new SingleNote(ID, creatingTime, creationDate, text, noteTags);
    noteDialog *nd = new noteDialog(this);
    nd->addNote(sn);
    nd->addtags(this->tags);
    nd->exec();
    sn = nd->getExecData();
    if (sn->getText() == "") {
        int notesSize = notes.size();
        for (int i = 0; i < notesSize; i++)
            if (this->notes[i]->getID() == sn->getID()) {
                // Remove item
                this->notes.erase(this->notes.begin() + i);
                break;
            }
    }
    else {
        noteToEdit->setText(sn->getText());
        QStringList tagsListNote = sn->getTagsList();
        for (int i = 0; i < tagsListNote.size(); i++)
            if (!noteToEdit->checkForTag(tagsListNote[i]))
                noteToEdit->addTag(tagsListNote[i]);
        noteToEdit->setEditedTime(sn->getEditedTime());
        noteToEdit->setEditedDate(sn->getEditedDate());
    }
    debugNote(sn);
    runInterface();
//*/
}

void MainWindow::deleteNote() {
    int currentRow = ui->table_notes->selectedItems()[0]->row();
    int noteID = ui->table_notes->item(currentRow, 0)->text().toInt();
    int notesSize = notes.size();
    for (int i = 0; i < notesSize; i++)
        if (this->notes[i]->getID() == noteID) {
            // Remove item
            this->notes.erase(this->notes.begin() + i);
            break;
        }
    runInterface();
}

void MainWindow::moveToArchive() {
    int currentRow = ui->table_notes->selectedItems()[0]->row();
    int noteID = ui->table_notes->item(currentRow, 0)->text().toInt();
    int notesSize = notes.size();
    for (int i = 0; i < notesSize; i++)
        if (this->notes[i]->getID() == noteID) {
            // Remove item
            this->archive.push_back(this->notes[i]);
            this->notes.erase(this->notes.begin() + i);
            break;
        }
    runInterface();
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
        for (int i = 0; i < tagsSize; i++) {
            this->tags.push_back(tagArray[i].toString());
            ui->list_tags->addItem(tagArray[i].toString());
        }
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
        if (this->tags[i] != "work" && this->tags[i] != "personal" && this->tags[i] != "uncategorized")
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

void MainWindow::updateTable() {
    if (tags.size() == 0) {
        // Display all notes
        int notesSize = this->notes.size();
        ui->table_notes->setRowCount(0);
        ui->table_notes->setRowCount(notesSize);
        // Insert all notes
        for (int i = 0; i < notesSize; i++) {
            // text time date tags
            ui->table_notes->setItem(i, 4, new QTableWidgetItem(this->notes[i]->getText()));
            ui->table_notes->setItem(i, 1, new QTableWidgetItem(this->notes[i]->getEditedTime().toString(Qt::TextDate)));
            ui->table_notes->setItem(i, 2, new QTableWidgetItem(this->notes[i]->getEditedDate().toString(Qt::TextDate)));
            ui->table_notes->setItem(i, 3, new QTableWidgetItem(this->notes[i]->getTags()));
            ui->table_notes->setItem(i, 0, new QTableWidgetItem(QString::number(this->notes[i]->getID())));
        }
    }
    else {
        // Display selected notes
        int notesSize = this->notes.size();
        int filterSize = filter.size();
        QVector<SingleNote*> noteList;

        bool hasAllTags;
        for (int i = 0; i < notesSize; i++) {
            hasAllTags = true;
            for (int j = 0; j < filterSize; j++)
                if (!notes[i]->checkForTag(filter[j]))
                    hasAllTags = false;
            if (hasAllTags)
                noteList.push_back(notes[i]);
        }

        int noteListSize = noteList.size();
        ui->table_notes->setRowCount(0);
        ui->table_notes->setRowCount(noteListSize);
        // Insert all notes
        for (int i = 0; i < noteListSize; i++) {
            // text time date tags
            ui->table_notes->setItem(i, 4, new QTableWidgetItem(noteList[i]->getText()));
            ui->table_notes->setItem(i, 1, new QTableWidgetItem(noteList[i]->getEditedTime().toString(Qt::TextDate)));
            ui->table_notes->setItem(i, 2, new QTableWidgetItem(noteList[i]->getEditedDate().toString(Qt::TextDate)));
            ui->table_notes->setItem(i, 3, new QTableWidgetItem(noteList[i]->getTags()));
            ui->table_notes->setItem(i, 0, new QTableWidgetItem(QString::number(this->notes[i]->getID())));
        }
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

void MainWindow::runInterface() {
    updateTable();
    QString labelText = "Notes :";
    if (filter.size() != 0) {
        for (int i = 0; i < filter.size(); i++) {
            labelText += " " + filter[i];
        }
    }
    ui->label_notes->setText(labelText);
}

void MainWindow::deleteTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        if (ui->list_tags->selectedItems()[i]->text() == "uncategorized"
                || ui->list_tags->selectedItems()[i]->text() == "work"
                || ui->list_tags->selectedItems()[i]->text() == "personal") {
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Error","You can't delete \"" + ui->list_tags->selectedItems()[i]->text() + "\" tag!");
            messageBox.setFixedSize(600,200);
        }
        else {
            QListWidgetItem *item = ui->list_tags->takeItem(ui->list_tags->currentRow());
            int tagSize = tags.size();
            for (int j = 0; j < tagSize; j++)
                if (tags[j] == item->text()) {
                    tags.erase(tags.begin() + j);
                    break;
                }
            // Unlinck all notes from deleted tag

            int notesSize = notes.size();
            int archiveSize = archive.size();
            for (int j = 0; j < notesSize; j++)
                notes[j]->deleteTag(item->text());
            for (int j = 0; j < archiveSize; j++)
                archive[j]->deleteTag(item->text());

            delete item;
        }
    }
}
