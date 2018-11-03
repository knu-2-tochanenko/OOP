#ifndef SINGLENOTE_H
#define SINGLENOTE_H

#include <QTime>
#include <QString>
#include <QVector>

class SingleNote {
private:
    QTime creationTime;
    QTime editedTime;
    QString text;
    QVector<QString> tags;
public:
    SingleNote();
    SingleNote(QTime creationTime, QString text, QVector<QString> tags);

    QTime getCreationTime();
    QTime getEditedTime();
    bool setEditedTime(QTime time);

    QString getText();
    bool setText(QString string);

    bool checkForTag(QString tag);
    bool deleteTag(QString tag);

};

#endif // SINGLENOTE_H
