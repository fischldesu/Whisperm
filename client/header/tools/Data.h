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
    class ConfigHelper
    {
    public:
        explicit ConfigHelper(QString  path);
        bool SetConfig(const QString& key, const QString& value) const; // NOLINT(*-use-nodiscard)
        QString GetSingleConfig(const QString& key) const;
        QHash<QString,QString> GetConfig() const;
        static QStringList ConfigKeys();
    private:
        QString m_path;
    };

} // namespace Data

#endif //DATA_H
