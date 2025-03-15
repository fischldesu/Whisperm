#ifndef APPLOG_H
#define APPLOG_H

#include <QMutex>
#include <QQueue>
#include <QString>
#include "tools/Enum.h"

class AppLog{
    Q_GADGET
public:
    enum Level{
        LOG_, INFO_, WARN_, ERROR_, FATAL_
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
    void Warning(const QString& log, bool window = false, bool nofilew = false);
    void Error(const QString& log);
    void Fatal(const QString& log);
} // namespace Log


#endif //APPLOG_H
