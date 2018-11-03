#include "singlenote.h"

SingleNote::SingleNote() {

}

SingleNote::SingleNote(QTime creationTime, QString text, QVector<QString> tags) {
    this->creationTime = creationTime;
    this->text = text;
    this->tags = tags;
}

void SingleNote::readJSON(const QJsonObject &json) {
    if (json.contains("id") && json["id"].isDouble())
        this->id = json["id"].toInt();
    if (json.contains("creation_time") && json["creation_time"].isDouble())
        this->creationTime = QTime().addSecs(json["creation_time"].toInt());
    if (json.contains("edited_time") && json["edited_time"].isDouble())
        this->editedTime = QTime().addSecs(json["edited_time"].toInt());
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
    json["edited_time"] = QTime(0,0,0).secsTo(this->editedTime);
    QJsonArray tagsArray;
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize; i++)
        tagsArray.push_back(this->tags[i]);
    json["tags"] = tagsArray;
}

QTime SingleNote::getCreationTime() {
    return this->creationTime;
}

QTime SingleNote::getEditedTime() {
    return this->editedTime;
}

bool SingleNote::setEditedTime(QTime time) {
    this->editedTime = time;
    return true;
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