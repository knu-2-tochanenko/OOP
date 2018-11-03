#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "singlenote.h"
#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QJsonObject>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addTagFunction();
private slots:
    void on_button_newTag_clicked();
    void on_button_newNote_clicked();
    void on_button_openArchive_clicked();
    void on_button_backup_clicked();

    void showContextMenu(const QPoint&);

    void deleteTagItem();
    void addTagItem();

private:
    Ui::MainWindow *ui;
    // Vector of notes
    QVector<SingleNote> notes;
};

#endif // MAINWINDOW_H
