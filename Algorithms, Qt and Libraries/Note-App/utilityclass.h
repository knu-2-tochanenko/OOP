#ifndef UTILITYCLASS_H
#define UTILITYCLASS_H

#include "singlenote.h"
#include <QString>

class UtilityClass
{
public:
    UtilityClass();

    static bool checkYN(QString message, QString title);
    static void debugNote(SingleNote *sn);
    static void error(QString message);
};

#endif // UTILITYCLASS_H
