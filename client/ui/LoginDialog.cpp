#include "ui/LoginDialog.h"
#include "ui_LoginDialog.h"

#include <QRegularExpressionValidator>
#include <QCloseEvent>
#include "Client.h"
#include "tools/AppLog.h"

LoginDialog::LoginDialog(Client* client) :
    Window(nullptr, nullptr, "Whisperm")
, client(client)
,ui(new Ui::LoginDialog) {
    ui->setupUi(this->centralWidget);
    const QRegularExpression regExp("[A-Za-z0-9]*");
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(this->centralWidget->size()+QSize(0, this->titlebarWidget->height()));
    connect(client, &Client::ServerSignal_Connected, this, &LoginDialog::Client_onConnected);
    connect(ui->btn_login, &QPushButton::clicked, this, &LoginDialog::UI_onTryLogin);
    ui->input_uid->setValidator(new QRegularExpressionValidator(regExp, this));
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
    this->setWindowTitle("🟩 " + this->windowTitle());
    ui->label_information->setText("");
    mb_online = true;
}

void LoginDialog::UI_onTryLogin() const
{
    if (!mb_online)
        return ui->label_information->setText("尚未连接到服务器");
    if (client->get_State() == Client::State::Authenticating)
        return ui->label_information->setText("正在登录中...");
    const auto uid = ui->input_uid->text();
    const auto pwd = ui->input_pwd->text();
    if(uid.isEmpty() || pwd.isEmpty())
        return ui->label_information->setText("请输入正确的用户名和密码");
    client->Client_Login(uid, pwd);
    ui->label_information->setText("正在登录...");
}

void LoginDialog::AUTH_FAILED(const QString& response)
{
    ui->label_information->setText("登录失败 " + response);
    Log("Login Failed:" + response, AppLog::LogLevel::Warning);
}
