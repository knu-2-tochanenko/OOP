#include "singlenote.h"

SingleNote::SingleNote() {
    // Empty constructor. Nothig special. Please, use writeJSOM fucntion!
}

SingleNote::SingleNote(QTime creationTime, QDate creationDate, QString text, QVector<QString> tags) {
    this->creationTime = creationTime;
    this->creationDate = creationDate;
    this->editedTime = creationTime;
    this->editedDate = creationDate;
    this->text = text;
    this->tags = tags;
}

SingleNote::SingleNote(const QJsonObject &json) {
    readJSON(json);
}

void SingleNote::readJSON(const QJsonObject &json) {
    if (json.contains("id") && json["id"].isDouble())
        this->id = json["id"].toInt();

    if (json.contains("creation_time") && json["creation_time"].isDouble())
        this->creationTime = QTime().addSecs(json["creation_time"].toInt());
    if (json.contains("creation_date") && json["creation_date"].isDouble())
        this->creationDate = QDate().addDays((json["creation_date"].toInt()));

    if (json.contains("edited_time") && json["edited_time"].isDouble())
        this->editedTime = QTime().addSecs(json["edited_time"].toInt());
    if (json.contains("edited_date") && json["edited_date"].isDouble())
        this->editedDate = QDate().addDays((json["edited_date"].toInt()));

    if (json.contains("note_text") && json["note_text"].isString())
        this->text = json["note_text"].toString();

    if (json.contains("tags") && json["tags"].isArray()) {
        QJsonArray tagsArray = json["tags"].toArray();
        int numberOfTags = tagsArray.count();
        for (int i = 0; i < numberOfTags; i++) {
            this->tags.push_back(tagsArray[i].toString());
        }
    }
}

void SingleNote::writeJSON(QJsonObject &json) const {
    json["id"] = this->id;
    json["creation_time"] = QTime(0,0,0).secsTo(this->creationTime);
    json["creation_date"] = QDate(0,0,0).daysTo(this->creationDate);
    json["edited_time"] = QTime(0,0,0).secsTo(this->editedTime);
    json["edited_date"] = QDate(0,0,0).daysTo(this->editedDate);
    QJsonArray tagsArray;
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize; i++)
        tagsArray.push_back(this->tags[i]);
    json["tags"] = tagsArray;
}

QTime SingleNote::getCreationTime() {
    return this->creationTime;
}

QDate SingleNote::getCreationDate() {
    return this->creationDate;
}

QTime SingleNote::getEditedTime() {
    return this->editedTime;
}

QDate SingleNote::getEditedDate() {
    return this->editedDate;
}

void SingleNote::setEditedTime(QTime time) {
    this->editedTime = time;
}

void SingleNote::setEditedDate(QDate date) {
    this->editedDate = date;
}

QString SingleNote::getText() {
    return this->text;
}

bool SingleNote::setText(QString string) {
    this->text = string;
    return true;
}

bool SingleNote::checkForTag(QString tag) {
    int vectorSize = this->tags.size();
    for (int i = 0; i < vectorSize; i++) {
        if (tag == tags[i])
            return true;
    }
    return false;
}

bool SingleNote::deleteTag(QString tag) {
    int vectorSize = this->tags.size();
    for (int i = 0; i < vectorSize; i++)
        if (tag == tags[i]) {
            tags.erase(tags.begin() + i);
            return true;
        }
    return false;
}
