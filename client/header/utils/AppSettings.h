#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QFuture>

class AppSettings {
    Q_GADGET
public:
    enum Options
    {
        QuitOnWindowClose,
        PoratableSettingsFile,
        CustomStoragePath,
        PreferredTheme
    }; Q_ENUM(Options);

    static AppSettings& UseInstance()
    { static AppSettings instance; return instance; }

    static QVariant Get(Options option, const QVariant& defaultValue);
    static QFuture<void> Set(Options option, const QVariant& value);
    static QFuture<void> Load();
    static void StopLoading()
    { UseInstance().m_quit.store(true); }
private:
    AppSettings();

    QMap<Options, QVariant> m_data;
    QMutex m_Settings_mutex;
    std::atomic<bool> m_quit = false;

    void LoadSettings();
};

#endif //APPSETTINGS_H
