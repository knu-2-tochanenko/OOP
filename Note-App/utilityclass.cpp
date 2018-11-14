#include "utilityclass.h"

#include <QMessageBox>
#include <QDebug>

UtilityClass::UtilityClass() {

}

bool UtilityClass::checkYN(QString message, QString title) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes)
      return true;
    else
        return false;
}

void UtilityClass::debugNote(SingleNote *sn) {
    qDebug() << "ID : " << sn->getID()
             << "\nCreation time : " << sn->getCreationTime()
             << "\nCreation date : " << sn->getCreationDate()
             << "\nEdited time : " << sn->getEditedTime()
             << "\nEdited date : " << sn->getEditedDate()
             << "\nText : " << sn->getText()
             << "\nTags : " << sn->getTags();
}
