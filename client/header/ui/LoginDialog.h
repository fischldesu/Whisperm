#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QLabel>
#include "ui/MessageDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE
class Client;
class QEventLoop;


class LoginDialog final : public MessageDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(Client* client);
    ~LoginDialog() override;

    bool exec();
private:
    Client* client;
    Ui::LoginDialog *ui;
    bool mb_online = false;
    QLabel* m_titleText;
    void InitTitlebar();

    void set_WindowTitle(const QString& title) override
    {   m_titleText->setText(title); MessageDialog::set_WindowTitle(title); }
private Q_SLOTS:
    void AUTH(bool sucess, const QString& failed_response = {});
public Q_SLOTS:
    void Client_onConnected();
    void Client_onDisconnected();
    void UI_onTryLogin() const;
    void UI_onSettingsClicked();
};


#endif //LOGINDIALOG_H
