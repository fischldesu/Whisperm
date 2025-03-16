#ifndef DATA_H
#define DATA_H

#include <QDateTime>

namespace Data
{
    struct Message
    {
        QString text;
        QDateTime time;
        QString from;
        QString target;
    };

} // namespace Data

#endif //DATA_H
