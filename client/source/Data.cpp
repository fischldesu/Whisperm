#include "tools/Data.h"

#include "tools/AppLog.h"

QString Data::RSA::EncryptMessage(const QString& plainText, const QString& publicKey)
{
    Log("解密消息：" + plainText);
    return plainText;
}

QString Data::RSA::DecryptMessage(const QString& cipherText, const QString& privateKey)
{
    Log("加密消息：" + cipherText);
    return cipherText;
}

Data::ConfigHelper::ConfigHelper(QString path):m_path(std::move(path))
{

}
bool Data::ConfigHelper::SetConfig(const QString& key, const QString& value) const
{
    if (QSettings settings(m_path, QSettings::IniFormat); settings.isWritable())
    {
        settings.setValue(key, value);
        if (GetSingleConfig(key) == value) return true;
        return false;
    }
    return false;
}
QString Data::ConfigHelper::GetSingleConfig(const QString& key) const
{
    const QSettings settings(m_path, QSettings::IniFormat);
    return settings.value(key).toString();
}
QHash<QString, QString> Data::ConfigHelper::GetConfig() const
{
    QHash<QString, QString> config;
    for (const auto& key : ConfigKeys())
    {
        config[key] = GetSingleConfig(key);
    }
    return config;
}
QStringList Data::ConfigHelper::ConfigKeys()
{
    return {
        "storage/user",
        "storage/logfile",
        "client/close_to_tray",
    };
}
