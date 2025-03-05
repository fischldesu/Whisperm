#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QDateTime>
#include <QSettings>

namespace Data
{
    struct Message
    {
        QString text;
        QDateTime time;
        QString from;
        QString towhom;
    };

    namespace RSA
    {
        QString EncryptMessage(const QString& plainText, const QString& publicKey);
        QString DecryptMessage(const QString& cipherText, const QString& privateKey);
    }

} // namespace Data

#endif //DATA_H
