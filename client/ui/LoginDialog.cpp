#include "ui/LoginDialog.h"
#include "ui_LoginDialog.h"

#include <QRegularExpressionValidator>
#include <QCloseEvent>

#include "Client.h"
#include "utils/AppLog.h"
#include "ui/PageSettings.h"
#include "uicomponent/Button.h"

LoginDialog::LoginDialog(Client* client) :
    MessageDialog(Window, nullptr)
, client(client),ui(new Ui::LoginDialog), m_titleText(new QLabel(this))
{
    ui->setupUi(this->centralWidget);
    this->set_WindowTitle(QApplication::applicationName());
    this->setFixedSize(this->centralWidget->size()+QSize(0, this->titlebarWidget->height()));
    connect(client, &Client::ServerSignal_Connected, this, &LoginDialog::Client_onConnected);
    connect(client, &Client::ServerSignal_Disconnected, this, &LoginDialog::Client_onDisconnected);
    connect(client, &Client::ServerSignal_AuthResponse, this, &LoginDialog::AUTH);

    connect(ui->btn_login, &QPushButton::clicked, this, &LoginDialog::UI_onTryLogin);

    ui->input_uid->setValidator(new QRegularExpressionValidator(QRegularExpression{"[A-Za-z0-9]*"}, this));
    InitTitlebar();

}

LoginDialog::~LoginDialog() {
    delete ui;
}

bool LoginDialog::exec()
{
    return MessageDialog::exec(true);
}

void LoginDialog::InitTitlebar()
{
    const auto titlebar = new QWidget(this);
    const auto layout = new QGridLayout(titlebar);
    const auto H_spacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    const auto V_spacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    const auto btn_close = new Button(titlebar);
    const auto btn_settings = new Button(titlebar);
    const auto btn_minimize = new Button(titlebar);
    this->set_CustomTitlebar(titlebar);

    btn_close->setFixedSize(42, 32);
    btn_minimize->setFixedSize(42, 32);
    btn_settings->setFixedSize(42, 32);
    btn_close->SetContent({}, QIcon(":/titlebar/close.svg"));
    btn_minimize->SetContent({}, QIcon(":/titlebar/minimize.svg"));
    btn_settings->SetContent({}, QIcon(":/menu/navigate.svg"));
    btn_close->set_IconScale(0.43);
    btn_minimize->set_IconScale(0.43);
    btn_settings->set_IconScale(0.53);
    btn_close->set_bgHoverColor(Qt::red);
    btn_minimize->set_bgHoverColor({64,64,64,64});
    btn_settings->set_bgHoverColor({64,64,64,64});
    titlebar->setLayout(layout);
    titlebar->setFixedHeight(48);
    layout->setContentsMargins(16, 0, 0, 0);
    layout->setSpacing(0);
    
    layout->addWidget(btn_close, 0, 4, 1, 1);
    layout->addWidget(btn_minimize, 0, 3, 1, 1);
    layout->addWidget(btn_settings, 0, 2, 1, 1);
    layout->addWidget(m_titleText, 0, 0, 2, 1);
    layout->addItem(H_spacer, 0, 1, 1, 1);
    layout->addItem(V_spacer, 1, 2, 1, 3);

    connect(btn_close, &QPushButton::clicked, this, &LoginDialog::Reject);
    connect(btn_minimize, &QPushButton::clicked, this, &LoginDialog::showMinimized);
    connect(btn_settings, &QPushButton::clicked, this, &LoginDialog::UI_onSettingsClicked);
}

void LoginDialog::Client_onConnected()
{
    this->set_WindowTitle("🟩 " + QApplication::applicationName());
    ui->label_information->setText("");
    mb_online = true;
}

void LoginDialog::Client_onDisconnected()
{
    this->set_WindowTitle(QApplication::applicationName());
    ui->label_information->setText("已离线");
    mb_online = false;
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

void LoginDialog::UI_onSettingsClicked()
{
    MessageDialog dialog(MessageDialog::Window);
    dialog.set_Content(new PageSettings(this));
    dialog.set_WindowTitle("设置 " + QApplication::applicationName());
    this->hide();
    dialog.exec(true);
    this->show();
}

void LoginDialog::AUTH(const bool sucess, const QString& failed_response)
{
    if (sucess)
        return this->Accept();

    ui->label_information->setText("登录失败 " + failed_response);
}
