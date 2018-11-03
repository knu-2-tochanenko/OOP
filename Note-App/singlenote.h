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
    QTime editedTime;
    QString text;
    QVector<QString> tags;
public:
    SingleNote();
    SingleNote(QTime creationTime, QString text, QVector<QString> tags);

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;

    QTime getCreationTime();
    QTime getEditedTime();
    bool setEditedTime(QTime time);

    QString getText();
    bool setText(QString string);

    bool checkForTag(QString tag);
    bool deleteTag(QString tag);

};

#endif // SINGLENOTE_H
