#include "utils/Cryptology.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "utils/AppLog.h"

SafeTransport::SafeTransport(QObject* parent)
    :QObject(parent)
{
    const auto [publicKey, privateKey] = Crypto::RSA::Generate();
    m_key_public = publicKey;
    m_key_private = privateKey;
}

QString SafeTransport::EncryptMessage(const QString& plainText) const
{
    if (mb_valid)
        return Crypto::AES::EncryptMessage("bytes/" + plainText + "/endbytes", m_key_session);
    return {};
}

QString SafeTransport::DecryptMessage(const QString& cipherText) const
{
    if (mb_valid)
    {
        const auto result = Crypto::AES::DecryptMessage(cipherText, m_key_session);
        if (result.startsWith("bytes/") && result.endsWith("/endbytes"))
            return result.mid(6, result.size() - 15);
    }
    return {};
}

QByteArray SafeTransport::Request_HandShake(const QByteArray& key_public)
{
    mb_initiator = true;
    m_key_session = Crypto::AES::Generate();
    const auto ecrypted_session = Crypto::RSA::EncryptMessage(m_key_session, key_public);
    return
    "AES/"
    + QByteArray::number(QDateTime::currentDateTime().addDays(2).toSecsSinceEpoch())
    + '/'
    + ecrypted_session;
}

bool SafeTransport::HandShake(const QByteArray& encrypted)
{
    const auto data = encrypted.split('/');
    if (data.size() != 3 || data[0] != "AES")
        return false;

    bool ok = false;
    const auto valid_until = QDateTime::fromSecsSinceEpoch(data[1].toLongLong(&ok));
    if (!ok || !valid_until.isValid() || valid_until < QDateTime::currentDateTime())
        return false;

    const auto session_key = Crypto::RSA::DecryptMessage(data[2], m_key_private);

    if (session_key.isEmpty())
        return false;

    if (mb_initiator)
        if (session_key != m_key_session)
            return false;

    m_key_session = session_key;
    mb_valid = true;
    return true;
}

namespace Crypto
{
    std::pair<QByteArray, QByteArray> RSA::Generate()
    {
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();

        EVP_PKEY* pkey = EVP_PKEY_new();
        if (!pkey)
        {

        }

        EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        if (!pctx || EVP_PKEY_keygen_init(pctx) <= 0)
        {

        }

        if (EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, 2048) <= 0)
        {

        }

        if (EVP_PKEY_keygen(pctx, &pkey) <= 0)
        {

        }

        BIO* privateBio = BIO_new(BIO_s_mem());
        if (!privateBio || !PEM_write_bio_PrivateKey(privateBio, pkey, nullptr, nullptr, 0, nullptr, nullptr))
        {

        }
        BUF_MEM* privateBuf;
        BIO_get_mem_ptr(privateBio, &privateBuf);
        const QByteArray private_key(privateBuf->data, static_cast<qsizetype>(privateBuf->length));
        BIO_free_all(privateBio);

        BIO* publicBio = BIO_new(BIO_s_mem());
        if (!publicBio || !PEM_write_bio_PUBKEY(publicBio, pkey)) {

        }
        BUF_MEM* publicBuf;
        BIO_get_mem_ptr(publicBio, &publicBuf);
        const QByteArray public_key(publicBuf->data, static_cast<qsizetype>(publicBuf->length));
        BIO_free_all(publicBio);

        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(pctx);

        return {public_key, private_key};
    }

    QByteArray RSA::EncryptMessage(const QByteArray& plainText, const QByteArray& publicKey)
    {
        const auto result = plainText;//模拟过程
        return result;
    }

    QByteArray RSA::DecryptMessage(const QByteArray& cipherText, const QByteArray& privateKey)
    {
        const auto result = cipherText;//模拟过程
        return result;
    }

    QByteArray AES::Generate()
    {
        return {};
    }

    QString AES::EncryptMessage(const QString& plainText, const QByteArray& key)
    {
        const auto result = plainText;//模拟过程
        return result;
    }

    QString AES::DecryptMessage(const QString& cipherText, const QByteArray& key)
    {
        const auto result = cipherText;//模拟过程
        return result;
    }

}
