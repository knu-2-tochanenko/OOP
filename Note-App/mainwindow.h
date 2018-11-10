#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "singlenote.h"
#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QMap>

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
    // Button actions
    void on_button_newTag_clicked();
    void on_button_newNote_clicked();

    // Context menu actions
    void showContextMenu(const QPoint&);
    void showListMenu(const QPoint&);

    // Actions with tags
    void deleteTagItem();
    void addTagItem();
    void editTagItem();
    void addTagToFilter();

    // Actions with notes
    void editNote();
    void deleteNote();
    void moveToArchive();

    // Toolbar ations
    void on_actionOpen_Archive_triggered();
    void on_actionToggle_Theme_triggered();
    void on_actionExport_to_triggered();

private:
    Ui::MainWindow *ui;
    // Vector of notes
    QVector<SingleNote*> notes;
    QVector<SingleNote*> archive;
    QStringList tags;
    QStringList filter;

    int maxID;

    bool readJSON(QString filePath);
    bool writeJSON(QString filePath);

    bool checkYN(QString message, QString title);

    void debugNote(SingleNote *sn);

    void updateList();

    bool moveToArchive(int ID);
    bool moveFromArchive(int ID);

    void runInterface();

    void setTheme(int number);
};

#endif // MAINWINDOW_H
