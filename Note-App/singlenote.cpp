#include "singlenote.h"

SingleNote::SingleNote() {

}

SingleNote::SingleNote(QTime creationTime, QString text, QVector<QString> tags) {
    this->creationTime = creationTime;
    this->text = text;
    this->tags = tags;
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
