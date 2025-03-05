#include "tools/Data.h"

#include "tools/AppLog.h"

QString Data::RSA::EncryptMessage(const QString& plainText, const QString& publicKey)
{
    return "txt/" + plainText + "/endtxt";
}

QString Data::RSA::DecryptMessage(const QString& cipherText, const QString& privateKey)
{
    if (cipherText.startsWith("txt/") && cipherText.endsWith("/endtxt"))
        return cipherText.mid(4, cipherText.size() - 11);
    return {};
}
