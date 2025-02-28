#ifndef APPLOG_H
#define APPLOG_H

#include <QString>

class AppLog {
public:
    enum class LogLevel {
        Info,
        Warning,
        Error
    };
    AppLog();
    static AppLog Instance();
    void Log(const QString& logMessage, LogLevel level) const;
private:
    bool m_printYear;
};


constexpr auto Log = [](const QString& logMessage, const AppLog::LogLevel level = AppLog::LogLevel::Info){AppLog::Instance().Log(logMessage, level);};

#endif //APPLOG_H
