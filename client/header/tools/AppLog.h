#ifndef APPLOG_H
#define APPLOG_H

#include <QMutex>
#include <QQueue>
#include <QString>
#include "tools/Enum.h"

class AppLogger {
public:
    enum class LogLevel {
        Info,
        Warning,
        Error
    };
    explicit AppLogger(bool printYear = false);
    ~AppLogger();

    static AppLogger* get_Instance();
    static void set_Instance(AppLogger* instance);

    void Log(const QString& logMessage, LogLevel level);
private:
    static AppLogger* s_instance;
    bool m_printYear;

    void Locked_Log();
    std::atomic<bool> m_running;
    QFuture<void> m_logThreadFuture;
    QMutex m_logQueue_Mutex;
    QQueue<QString> m_logQueue;
};


constexpr auto Logger = [](const QString& logMessage, const AppLogger::LogLevel level = AppLogger::LogLevel::Info)
{
    if (const auto logger = AppLogger::get_Instance())
        logger->Log(logMessage, level);
};

class AppLog{
    Q_OBJECT
public:
    enum Level{
        LOG, INFO, WARN, ERROR, FATAL
    }; Q_ENUM(Level)

    ~AppLog()
    { m_AsyncRunning.store(false); }

    static AppLog& UseInstance();
    void Log(const QString& log, Level level, bool window, bool nofilew, bool async = true);
    void Log_Async(const QString& log, bool noFileW);
    static void ConsoelWrite(const QString& log);

private:
    std::atomic<bool> m_AsyncRunning;
    QQueue<QString> m_AsyncLogQueue;
    QMutex m_Mutex_LogQueue;

    Enum<Level> levelEnumertor;

    AppLog();
    void AsyncLogFile();
};

namespace Log
{
    void Log(const QString& log,     bool window = false, bool nofilew = false);
    void Info(const QString& log,    bool window = false, bool nofilew = false);
    void Error(const QString& log,   bool window = false, bool nofilew = false);
    void Warning(const QString& log, bool window = false, bool nofilew = false);
    void Fatal(const QString& log,   bool window = false, bool nofilew = false);
} // namespace Log


#endif //APPLOG_H
