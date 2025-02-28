#include "tools/AppLog.h"
#include <iostream>
#include <QDateTime>

void AppLog::Log(const QString& logMessage, LogLevel level) const
{
    QString timeFormat{"MM-dd hh:mm:ss"};
    if (m_printYear)
        timeFormat = "YYYY MM-dd hh:mm:ss";
    const auto qt_time = QDateTime::currentDateTime().toString(timeFormat);
    const auto time = qt_time.toStdString();
    switch (level)
    {
    case LogLevel::Info:
        std::cout << time << " [INFO] " << logMessage.toStdString() << std::endl;
        break;
    case LogLevel::Warning:
        std::cout << time << " [WARNING] " << logMessage.toStdString() << std::endl;
        break;
    case LogLevel::Error:
        std::cerr << time << " [ERROR] " << logMessage.toStdString() << std::endl;
        break;
    default:
        std::cout << time << " [INFO?] " << logMessage.toStdString() << std::endl;
        break;
    }
}

AppLog::AppLog():m_printYear(false)
{

}

AppLog AppLog::Instance()
{
    static AppLog instance;
    return instance;
}
