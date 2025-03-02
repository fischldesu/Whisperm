#include "tools/AppLog.h"

#include <QDateTime>

void AppLog::Log(const QString& logMessage, LogLevel level) const
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
    puts(log.toUtf8().data());
}

AppLog::AppLog():m_printYear(false)
{

}

AppLog AppLog::Instance()
{
    static AppLog instance;
    return instance;
}
