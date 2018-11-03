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

    // TODO : Add reading from JSON file
}

MainWindow::~MainWindow() {
    // TODO : Add writing to JSON file
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
        if (!hasSame)
            ui->list_tags->addItem(tagText);
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

    QInputDialog qid;
    // TODO : Find a way to resize this window
    QString tagText = qid.getText(this, tr("New Note"), tr("Note text:"), QLineEdit::Normal, "default", &ok);
    QTime creatingTime = QTime::currentTime();
    QDate creationDate = QDate::currentDate();
    // TODO : Make adding new note to JSON 7 tabel
    // TODO : Make new note funcion
}

void MainWindow::on_button_openArchive_clicked() {
    // TODO : Make opening archive window
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = ui->list_tags->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("New tag", this, SLOT(addTagItem()));
    myMenu.addAction("Delete",  this, SLOT(deleteTagItem()));

    myMenu.exec(globalPos);
}

void MainWindow::addTagItem() {
    addTagFunction();
}

bool MainWindow::readJSON(QString filePath) {
    // TODO : Read all tags from another file
    QFile readNotesFile;
    readNotesFile.setFileName(filePath);
    if (!readNotesFile.open(QFile::ReadOnly | QFile::Text))
        return false;

    QString jsonData = readNotesFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObject = document.object();

    // Get array of notes
    QJsonArray jsonArray = jsonObject["notes"].toArray();
    int arraySize = jsonArray.count();
    for (int i = 0; i < arraySize; i++) {
        SingleNote *note = new SingleNote(jsonArray[i].toObject());
        this->notes.push_back(note);
    }
    readNotesFile.close();
    return true;
}

bool MainWindow::writeJSON(QString filePath) {
    // TODO : Write tags to another file
    QFile writeNotesFile;
    writeNotesFile.setFileName(filePath);
    if (!writeNotesFile.open(QFile::WriteOnly))
        return false;

    QJsonArray jsonArray;
    int notesSize = notes.size();
    QJsonObject jsonObject;
    for (int i = 0; i < notesSize; i++) {
        notes[i]->writeJSON(jsonObject);
        jsonArray.push_back(jsonObject);
    }

    QJsonObject finalObject;
    finalObject["notes"] = jsonArray;
    QJsonDocument document(finalObject);
    writeNotesFile.write(document.toJson());
    writeNotesFile.close();
    return true;
}

void MainWindow::deleteTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        QListWidgetItem *item = ui->list_tags->takeItem(ui->list_tags->currentRow());
        // Unlinck all notes from deleted tag
        delete item;
    }
}
