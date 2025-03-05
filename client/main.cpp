#include  "whisperm.h"

int main(const int argc, char** argv)
{
    whisperm app(argc, argv);
    return app.run();
}

whisperm::whisperm(int argc, char** argv):app(argc, argv)
{
    QApplication::setApplicationName(_AppName_);
    QApplication::setApplicationVersion(_AppVersion_);

    this->client = new Client;
    this->InitTrayIcon();
}

whisperm::~whisperm()
{
    this->quit();
}

int whisperm::run()
{
    loginDialog = std::make_unique<LoginDialog>(this->client);
    if (loginDialog->exec())
    {
        loginDialog.reset();

        mainWindow = std::make_unique<MainWindow>(this->client);
        mainWindow->show();
        mainWindow->UpdateContentSize();
        if (m_trayIcon)
        {
            m_trayIcon->show();
            QApplication::setQuitOnLastWindowClosed(false);
        }
        return QApplication::exec();
    }
    return 0;
}

void whisperm::quit()
{
    delete client;
    delete m_trayIcon;
    delete m_trayMenu;
    client = nullptr;
    m_trayIcon = nullptr;
    m_trayMenu = nullptr;

    QApplication::closeAllWindows();
    QApplication::quit();
}

void whisperm::display()
{
    if (mainWindow)
        mainWindow->show();
}

void whisperm::InitTrayIcon()
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        m_trayIcon = new QSystemTrayIcon;
        m_trayMenu = new QMenu;

        m_trayIcon->setIcon(QIcon(":/svg/chat-icon_blue.svg"));
        m_trayIcon->setContextMenu(m_trayMenu);
        m_trayIcon->setToolTip(QApplication::applicationName());

        QObject::connect(m_trayMenu->addAction("打开"), &QAction::triggered, [this]{ this->display(); });
        QObject::connect(m_trayMenu->addAction("退出"), &QAction::triggered, [this]{ this->quit(); });
    }
}
