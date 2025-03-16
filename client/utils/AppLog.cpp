#include "utils/AppLog.h"

#include <iostream>
#include <QApplication>
#include <QDateTime>
#include <QFile>

#include "utils/Task.h"
#include "ui/MessageDialog.h"

AppLog::AppLog():m_AsyncRunning{false}
{
    auto Task_FileW = Task(this, &AppLog::AsyncLogFile);
    Task_FileW.AsyncCall();
    QObject::connect(QApplication::instance(), &QApplication::aboutToQuit, [this]
    {
        m_AsyncRunning.store(false);
    });
}

void AppLog::AsyncLogFile()
{
    m_AsyncRunning.store(true);
    while (m_AsyncRunning)
    {
        QMutexLocker locker(&m_Mutex_LogQueue);
        while (!m_AsyncLogQueue.isEmpty())
        {
            const auto log = m_AsyncLogQueue.dequeue();
            locker.unlock();
            QFile logFile("log-" + QDate::currentDate().toString() + ".log");
            if (logFile.open(QIODevice::Append | QIODevice::Text))
            {
                QTextStream{&logFile} << log << "\n";
            }
            logFile.close();
            locker.relock();
        }
    }
}

void AppLog::ConsoelWrite(const QString& log)
{
    std::clog << log.toStdString() << std::endl;
}

void AppLog::PushFileWriteQueue(const QString& log)
{
    QMutexLocker locker(&m_Mutex_LogQueue);
    m_AsyncLogQueue.enqueue(log);
}

AppLog& AppLog::UseInstance()
{
    static AppLog instance;
    return instance;
}

int AppLog::Log(const QString& log, const Level level, const bool window, const bool nofilew)
{
    const auto datetime = QDateTime::currentDateTime().toLocalTime().toString("MM-dd hh:mm:ss");
    const auto level_ = levelEnumertor.Name(level);

    const auto log_ = datetime + " [" + level_.left(level_.size() - 1) + "] " + log;
    
    auto type = MessageDialog::Info;
    auto modal = false;
    auto noFileWirte = nofilew;

    switch (level)
    {
    case WARN_:
        type = MessageDialog::Warning;
        break;
    case FATAL_:
        modal = true;
        m_AsyncRunning = false;
        noFileWirte = false;
        [[fallthrough]];
    case ERROR_:
        type = MessageDialog::Error;
        break;
    default:
        break;
    }

    ConsoelWrite(log_);

    if(!noFileWirte)
        PushFileWriteQueue(log_);

    if (window)
    {
        MessageDialog logDialog(type);
        logDialog.set_Content(log_);
        return logDialog.exec(modal);
    }
    return 0;
}

void Log::Log(const QString& log, const bool window, const bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::LOG_, window, nofilew);
}

void Log::Info(const QString& log, const bool window, const bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::INFO_, window, nofilew);
}

void Log::Warning(const QString& log, const bool window, const bool nofilew)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::WARN_, window, nofilew);
}

void Log::Error(const QString& log)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::ERROR_, true, false);
}

void Log::Fatal(const QString& log)
{
    auto& logger = AppLog::UseInstance();
    logger.Log(log, AppLog::FATAL_, true, false);
}