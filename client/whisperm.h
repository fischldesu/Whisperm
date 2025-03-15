#ifndef WHISPEM_H
#define WHISPEM_H

#include <QApplication>
#include "Client.h"
#include "tools/AppLog.h"
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"
#include "uicomponent/Button.h"
#include "uicomponent/SystemTrayMenu.h"

#ifndef _AppName_
#define _AppName_ "Whisperm"
#endif
#ifndef _AppVersion_
#define _AppVersion_ "0.0.1(alpha)p1"
#endif

class whisperm {
public:
    whisperm(int argc, char** argv);
    ~whisperm();

    int run();
    
    void display() const;
    static void quit();
private:
    void InitTrayIcon();
    QSystemTrayIcon* m_trayIcon = nullptr;
    SystemTrayMenu* m_trayMenu = nullptr;
    Client* client;
//window
    std::unique_ptr<LoginDialog> loginDialog;
    std::unique_ptr<MainWindow> mainWindow;

    QApplication app;
};



#endif //WHISPEM_H
