#ifndef WHISPEM_H
#define WHISPEM_H

#include <QApplication>
#include "Client.h"
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"

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
