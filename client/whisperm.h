#ifndef WHISPEM_H
#define WHISPEM_H

#include <QApplication>
#include "Client.h"
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"

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

    int run() const;
private:
    QApplication app;
    Client* client;

};



#endif //WHISPEM_H
