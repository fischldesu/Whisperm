#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "ui/Window.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class Client;
class QEventLoop;

class LoginDialog final : public Window {
Q_OBJECT

public:
    explicit LoginDialog(Client* client);
    ~LoginDialog() override;

    bool exec();

    void Accept();
    void Reject();
private:
    Client* client;
    Ui::LoginDialog *ui;
    int result = 0;
    QEventLoop* loop = nullptr;
    bool mb_online = false;
    void AUTH_FAILED(const QString& response);
    void closeEvent(QCloseEvent* event) override;
public Q_SLOTS:
    void Client_onConnected();
    void Client_onDisconnected();
    void UI_onTryLogin() const;
};


#endif //LOGINDIALOG_H
