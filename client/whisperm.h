#ifndef WHISPEM_H
#define WHISPEM_H

#include <QApplication>

#include "Client.h"
#include "header/utils/AppSettings.h"
#include "utils/AppLog.h"
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"
#include "uicomponent/Button.h"
#include "uicomponent/SystemTrayMenu.h"

consteval const char* OrgName()
{ return "fischldesu"; }
consteval const char* AppName()
{ return "Whisperm"; }
consteval const char* AppVersion()
{ return "0.0.1(alpha)p1"; }


class whisperm {
public:
    whisperm(int argc, char** argv);
    ~whisperm();

    int run();
    void display() const;

    static void quit();
private:
    void InitTrayIcon();

    QApplication app;
    Client* client;

    QSystemTrayIcon* m_trayIcon = nullptr;
    SystemTrayMenu* m_trayMenu = nullptr;

    std::unique_ptr<MainWindow> mainWindow;
};



#endif //WHISPEM_H
