#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QString>
#include "utilityclass.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();

private slots:
    void on_buttonCancel_clicked();
    void on_buttonCreate_clicked();
    void on_buttonPath_clicked();

    void on_userPassword_textChanged();
    void on_ownerPassword_textChanged();

private:
    Ui::ExportDialog *ui;

    bool checkPassword(QString const & password) const;
    bool checkFileName(QString const & fileName) const;
    bool checkPathName(QString const & pathName) const;

    bool doesExist(QString const & fullPath);

    void generatePDF(QString const & path,
        QString const & ownerPassword = "", QString const & userPassword = "");

    void update();

};

#endif // EXPORTDIALOG_H
