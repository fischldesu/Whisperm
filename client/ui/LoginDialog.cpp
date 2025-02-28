#include "ui/LoginDialog.h"
#include "ui_LoginDialog.h"

#include <QCloseEvent>
#include "Client.h"
#include "tools/AppLog.h"

LoginDialog::LoginDialog(const Client* client) :
    Window(nullptr, nullptr, "Whisperm")
, client(client)
,ui(new Ui::LoginDialog) {
    this->setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this->centralWidget);
    this->setFixedSize(this->centralWidget->size());
    connect(client, &Client::ServerSignal_Connected, this, &LoginDialog::Client_onConnected);
    connect(ui->btn_login, &QPushButton::clicked, this, &LoginDialog::UI_onTryLogin);
}

LoginDialog::~LoginDialog() {
    delete ui;
}

bool LoginDialog::exec()
{
    const auto auth = [this](bool sucess, const QString& response)
    {
        if (sucess)
        {
            this->Accept();
        }
        else
        {
            this->AUTH_FAILED(response);
        }
    };
    connect(client, &Client::ServerSignal_AuthResponse, auth);

    this->show();
    QEventLoop eventLoop;
    // ReSharper disable once CppDFALocalValueEscapesFunction
    this->loop = &eventLoop;
    this->result = this->loop->exec();
    this->loop = nullptr;
    this->close();
    return this->result;
}

void LoginDialog::Accept() const
{
    if (this->loop) {
        this->loop->exit(true);
    }
}

void LoginDialog::Reject() const
{
    if (this->loop) {
        this->loop->exit(false);
    }
}

void LoginDialog::closeEvent(QCloseEvent* event)
{
    Window::closeEvent(event);
    this->Reject();
    event->accept();
}

void LoginDialog::Client_onConnected()
{
    this->setWindowTitle("🟩 Whisperm");
}

void LoginDialog::UI_onTryLogin() const
{
    const auto uid = ui->input_uid->text();
    const auto pwd = ui->input_pwd->text();
    const_cast<Client*>(client)->Client_Login(uid, pwd);
}

void LoginDialog::AUTH_FAILED(const QString& response)
{
    Log("Login Failed:" + response, AppLog::LogLevel::Warning);
}
