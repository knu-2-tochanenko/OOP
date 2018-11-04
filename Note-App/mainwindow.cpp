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
    this->tags.push_back("uncategorized");
    this->tags.push_back("work");
    this->tags.push_back("university");
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
    QString noteText = QInputDialog::getMultiLineText(this, tr("New Note"), tr("Note text:"), "default", &ok);
    QTime creatingTime = QTime::currentTime();
    QDate creationDate = QDate::currentDate();
    QString text = noteText;
    QVector<QString> tags;
    int ID = this->maxID + 1;
    this->maxID++;

    // Get list of tags
    //*
    bool moreTags = true;
    QStringList stringList;
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize; i++)
        stringList.push_back(this->tags[i]);
    while (moreTags) {
        QString tag = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
                                            tr("Season:"), stringList, 0, false, &ok);
        moreTags = false;
    }
    //*/
    // TODO : Make adding new note to JSON tabel
    // TODO : Make new note function
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
    // TODO : Read all tags
    // TODO : Read maxID tag
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

    // Get array of tags

    readNotesFile.close();
    return true;
}

bool MainWindow::writeJSON(QString filePath) {
    // TODO : Write tags
    // TODO : Write maxID
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
