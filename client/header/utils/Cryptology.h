#ifndef CRYPOTOLOGY_H
#define CRYPOTOLOGY_H

#include <QObject>
#include <QMap>
#include <QCryptographicHash>
#include <QException>

#include "Data.h"

class Client;

namespace Crypto
{
    namespace RSA
    {
        std::pair<QByteArray, QByteArray> Generate();
        QByteArray EncryptMessage(const QByteArray& plainText, const QByteArray& publicKey);
        QByteArray DecryptMessage(const QByteArray& cipherText, const QByteArray& privateKey);
    }
    namespace AES
    {
        QByteArray Generate();
        QString EncryptMessage(const QString& plainText, const QByteArray& key);
        QString DecryptMessage(const QString& cipherText, const QByteArray& key);
    }
}

class SafeTransport final : QObject
{
    Q_OBJECT
public:
    explicit SafeTransport(QObject* parent = nullptr);

    [[nodiscard]]
    QString EncryptMessage(const QString& plainText) const;
    [[nodiscard]]
    QString DecryptMessage(const QString& cipherText) const;

    [[nodiscard]]
    QPair<QByteArray, QByteArray> get_KeyPair_RSA() const
    { return { m_key_public, m_key_private }; }

    QByteArray Request_HandShake(const QByteArray& key_public);
    bool HandShake(const QByteArray& encrypted);

private:
    bool mb_valid = false;
    bool mb_initiator = false;

    QByteArray m_key_private;
    QByteArray m_key_public;
    QByteArray m_key_session;
};

#endif //CRYPOTOLOGY_H
