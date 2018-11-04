#ifndef SINGLENOTE_H
#define SINGLENOTE_H

#include <QTime>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class SingleNote {
private:
    int id;
    QTime creationTime;
    QDate creationDate;
    QTime editedTime;
    QDate editedDate;
    QString text;
    QVector<QString> tags;
public:
    SingleNote();
    SingleNote(QTime creationTime, QDate creationDate, QString text, QVector<QString> tags);
    SingleNote(const QJsonObject &json);

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;

    QTime getCreationTime();
    QDate getCreationDate();
    QTime getEditedTime();
    QDate getEditedDate();
    void setEditedTime(QTime time);
    void setEditedDate(QDate date);

    QString getText();
    bool setText(QString string);

    bool checkForTag(QString tag);
    bool deleteTag(QString tag);

};

#endif // SINGLENOTE_H
