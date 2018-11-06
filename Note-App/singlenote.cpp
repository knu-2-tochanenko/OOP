#include "singlenote.h"

SingleNote::SingleNote() {
    // Empty constructor. Nothig special. Please, use writeJSOM fucntion!
}

SingleNote::SingleNote(int ID, QTime creationTime, QDate creationDate, QString text, QStringList tags) {
    this->ID = ID;
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
        this->ID = json["id"].toInt();

    if (json.contains("creation_time") && json["creation_time"].isString())
        this->creationTime = QTime().fromString(json["creation_time"].toString(), Qt::TextDate);
    if (json.contains("creation_date") && json["creation_date"].isString())
        this->creationDate = QDate().fromString(json["creation_date"].toString(), Qt::TextDate);

    if (json.contains("edited_time") && json["edited_time"].isString())
        this->editedTime = QTime().fromString(json["edited_time"].toString(), Qt::TextDate);
    if (json.contains("edited_date") && json["edited_date"].isString())
        this->editedDate = QDate().fromString(json["edited_date"].toString(), Qt::TextDate);

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
    json["id"] = this->ID;
    json["creation_time"] = this->creationTime.toString(Qt::TextDate);
    json["creation_date"] = this->creationDate.toString(Qt::TextDate);
    json["edited_time"] = this->editedTime.toString(Qt::TextDate);
    json["edited_date"] = this->editedDate.toString(Qt::TextDate);
    json["text"] = this->text;
    QJsonArray tagsArray;
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize; i++)
        tagsArray.push_back(this->tags[i]);
    json["tags"] = tagsArray;
}

int SingleNote::getID() {
    return this->ID;
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

QString SingleNote::getTags() {
    QString res = "";
    for (int i = 0; i < this->tags.size(); i++)
        res += this->tags[i] + " ";
    return res;
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
