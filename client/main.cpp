#include  "whisperm.h"

int main(const int argc, char** argv)
{
    whisperm app(argc, argv);
    return app.run();
}

whisperm::whisperm(int argc, char** argv)
    :app(argc, argv), client(new Client)
{
    QApplication::setOrganizationName(OrgName());
    QApplication::setApplicationName(AppName());
    QApplication::setApplicationVersion(AppVersion());

    this->InitTrayIcon();
}

whisperm::~whisperm()
{
    delete client;
    delete m_trayIcon;
    delete m_trayMenu;
}

int whisperm::run()
{
    auto loginDialog = std::make_unique<LoginDialog>(this->client);
    AppSettings::Load().then(QThread::currentThread(), [&loginDialog]
    { loginDialog->DisplayComponents(); });
    if (loginDialog->exec())
    {
        mainWindow = std::make_unique<MainWindow>(this->client);

        if (m_trayIcon)
        {
            m_trayIcon->setToolTip(QApplication::applicationName()+ "\nUID: " + client->get_UID());
            m_trayIcon->show();
        }
        QApplication::setQuitOnLastWindowClosed(AppSettings::Get(AppSettings::QuitOnWindowClose, false).toBool());
        mainWindow->show();
    }
    else 
        QTimer::singleShot(0, []{ whisperm::quit(); });
        
    return QApplication::exec();
}

void whisperm::quit()
{
    QApplication::closeAllWindows();
    QApplication::quit();
}

void whisperm::display() const
{
    if (mainWindow)
    {
        mainWindow->show();
        if (mainWindow->isMinimized())
            mainWindow->showNormal();
        mainWindow->activateWindow();
        mainWindow->raise();
    }
}

void whisperm::InitTrayIcon()
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        m_trayIcon = new QSystemTrayIcon;
        m_trayMenu = new SystemTrayMenu(m_trayIcon);
        m_trayIcon->setIcon(QIcon(":/tray/icon.svg"));
        m_trayMenu->set_Tray_DoubleClick([this]{ this->display(); });
        QHash<QString, QPair< QIcon, std::function<void()>>> buttons;

        buttons.insert("打开窗口",{QIcon(":/tray/menu_open.svg"), [this]
        {this->display();}});
        buttons.insert("退出", {QIcon(":/tray/menu_close.svg"), [this]
        { quit();}});

        const auto menu = m_trayMenu->Menu();
        for (auto it = buttons.constBegin(); it != buttons.constEnd(); ++it)
        {
            const auto& data = it.value();
            const auto btn = new Button(menu);
            btn->SetContent(it.key(), data.first);
            QObject::connect(btn, &QPushButton::clicked, [data]{ data.second();});
            m_trayMenu->AddAction(btn);
        }
    }
}
