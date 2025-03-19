#include "utils/AppSettings.h"

#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include "utils/Enum.h"
#include "utils/Task.h"

AppSettings::AppSettings() = default;

QVariant AppSettings::Get(const Options option, const QVariant& defaultValue)
{
    auto& ins = UseInstance();
    QMutexLocker locker(&ins.m_Settings_mutex);
    return ins.m_data.value(option, defaultValue);
}

QFuture<void> AppSettings::Set(const Options option, const QVariant& value)
{
    auto& ins = UseInstance();
    QMutexLocker locker(&ins.m_Settings_mutex);
    ins.m_data.insert(option, value);
    return Task(std::function{[option, value]
    {
        auto settings = std::make_unique<QSettings>(
            QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName());
        auto options_enum = Enum<Options>();
        if (settings->value("app/" + options_enum.Name(PoratableSettingsFile), false) .toBool())
            settings = std::make_unique<QSettings>("./config.ini", QSettings::IniFormat);
        const auto name = "app/" + options_enum.Name(option);
        settings->setValue(name, value);
    }}).AsyncCall();
}

QFuture<void> AppSettings::Load()
{
    auto& ins = UseInstance();
    ins.m_quit.store(false);
    return Task(&ins, &AppSettings::LoadSettings).AsyncCall();
}

void AppSettings::LoadSettings()
{
    QMutexLocker locker(&m_Settings_mutex);
    auto settings = std::make_unique<QSettings>(
        QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName());
    auto options_enum = Enum<Options>();
    auto pair = options_enum.Data();
    if (settings->value("app/" + options_enum.Name(PoratableSettingsFile), false) .toBool())
        settings = std::make_unique<QSettings>("./config.ini", QSettings::IniFormat);

    for (const auto& [name, option] : pair)
    {
        if (m_quit)
            break;
        m_data.insert(option, settings->value("app/" + name, 0));
    }

}

