#include "tools/AppLog.h"

#include <iostream>
#include <QtConcurrent/QtConcurrent>

#include "tools/Task.h"
#include "ui/MessageDialog.h"

AppLogger* AppLogger::s_instance;

void AppLogger::Log(const QString& logMessage, LogLevel level)
{
    QString timeFormat{"MM-dd hh:mm:ss"};
    if (m_printYear)
        timeFormat = "YYYY MM-dd hh:mm:ss";
    const auto time = QDateTime::currentDateTime().toString(timeFormat);
    QString _level = "[LOG]";
    switch (level)
    {
    case LogLevel::Info: _level = "[INFO]"; break;
    case LogLevel::Warning: _level = "[WARNING]"; break;
    case LogLevel::Error: _level = "[ERROR]"; break;
    }
    const QString log = time + " " + _level + " " + logMessage;

    QMutexLocker locker(&m_logQueue_Mutex);
    m_logQueue.enqueue(log);
    locker.unlock();
}

AppLogger::AppLogger(bool printYear):m_printYear(printYear)
{
    m_running = true;
    m_logThreadFuture = QtConcurrent::run([=]
    {
        while (m_running)
        { this->Locked_Log();}
    });
}

AppLogger::~AppLogger()
{
    m_running = false;
    if (m_logThreadFuture.isRunning())
        m_logThreadFuture.waitForFinished();
}

AppLogger* AppLogger::get_Instance()
{
    return s_instance;
}

void AppLogger::set_Instance(AppLogger* instance)
{
    s_instance = instance;
}

void AppLogger::Locked_Log()
{
    QMutexLocker locker(&m_logQueue_Mutex);
    while (!m_logQueue.isEmpty())
    {
        const auto log = m_logQueue.dequeue();
        std::clog << log.toStdString() << std::endl;
    }
}

AppLog::AppLog():m_AsyncRunning{false}
{
    auto Task_FileW = Task(this, &AppLog::AsyncLogFile);
    Task_FileW.AsyncCall();
}

void AppLog::AsyncLogFile()
{
    m_AsyncRunning = true;
    while (m_AsyncRunning)
    {
        QMutexLocker locker(&m_Mutex_LogQueue);
        while (!m_AsyncLogQueue.isEmpty())
        {
            const auto log = m_AsyncLogQueue.dequeue();

        }
    }
}

void AppLog::ConsoelWrite(const QString& log)
{
    std::clog << log.toStdString() << std::endl;
}

void AppLog::Log_Async(const QString& log, bool noFileW)
{
    ConsoelWrite(log);
    QMutexLocker locker(&m_Mutex_LogQueue);
    m_AsyncLogQueue.enqueue(log);
}

AppLog& AppLog::UseInstance()
{
    static AppLog instance;
    return instance;
}

void AppLog::Log(const QString &log, Level level,  bool window, bool nofilew, bool async)
{
    const auto datetime = QDateTime::currentDateTime().toLocalTime().toString("MM-dd hh:mm:ss");
    const auto level_ = levelEnumertor.Name(level);

    const auto log_ = datetime + " [" + level_ + "] " + log;
    
    auto type = MessageDialog::Info;
    auto modal = false;

    switch (level)
    {
    case WARN:
        type = MessageDialog::Warning;
        break;
    case FATAL:[[fallthrough]]
        modal = true;
        m_AsyncRunning = false;
        async = false;
    case ERROR:
        type = MessageDialog::Error;
        break;
    default:
        break;
    }

    if (window)
    {
        MessageDialog logDialog(type);
        logDialog.set_Content(log_);
        logDialog.exec(modal);
    }

    if(async) Log_Async(log_, nofilew);
    else ConsoelWrite(log_);
}

void Log::Log(const QString& log, const bool window, const bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::LOG, window, nofilew);
}

void Log::Info(const QString& log, bool window, bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::INFO, window, nofilew);
}

void Log::Warning(const QString& log, bool window, bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::WARN, window, nofilew);
}

void Log::Error(const QString& log, bool window, bool nofilew)
{
    auto& logger = AppLog::UseInstance();
#pragma push_macro("ERROR")
#undef ERROR
    logger.Log(log, AppLog::ERROR, window, nofilew);
#pragma pop_macro("ERROR")
}

void Log::Fatal(const QString& log, bool window, bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::FATAL, window, nofilew);
}