#ifndef APPLOG_H
#define APPLOG_H

#include <QFuture>
#include <QMutex>
#include <QQueue>
#include <QString>

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


constexpr auto Log = [](const QString& logMessage, const AppLogger::LogLevel level = AppLogger::LogLevel::Info)
{
    if (const auto logger = AppLogger::get_Instance())
        logger->Log(logMessage, level);
};

#endif //APPLOG_H
