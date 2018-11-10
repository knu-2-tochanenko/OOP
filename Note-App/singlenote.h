#ifndef SINGLENOTE_H
#define SINGLENOTE_H

#include <QTime>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

class SingleNote {
private:
    int ID;
    QTime creationTime;
    QDate creationDate;
    QTime editedTime;
    QDate editedDate;
    QString text;
    QStringList tags;
public:
    SingleNote();
    SingleNote(int ID, QTime creationTime, QDate creationDate, QString text, QStringList tags);
    SingleNote(const QJsonObject &json);

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;

    int getID();
    QTime getCreationTime();
    QDate getCreationDate();
    QTime getEditedTime();
    QDate getEditedDate();
    void setEditedTime(QTime time);
    void setEditedDate(QDate date);

    QString getText();
    QString getTags();
    bool setText(QString string);

    bool checkForTag(QString tag);
    bool addTag(QString tag);
    bool deleteTag(QString tag);
    void clearTags();

    QStringList getTagsList();
};

#endif // SINGLENOTE_H
