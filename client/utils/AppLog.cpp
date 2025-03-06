#include "tools/AppLog.h"

#include <iostream>
#include <QtConcurrent/QtConcurrent>

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