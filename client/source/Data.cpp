#include "utils/Data.h"

#include "utils/Cryptology.h"

QString Data::RSA::EncryptMessage(const QString& plainText, const QString& publicKey)
{
    const auto result = plainText;

    return "txt/" + result + "/endtxt";
}

QString Data::RSA::DecryptMessage(const QString& cipherText, const QString& privateKey)
{
    const auto result = cipherText;

    if (result.startsWith("txt/") && result.endsWith("/endtxt"))
        return result.mid(4, result.size() - 11);
    return {};
}
