#include "ui/MainWindow.h"
#include "ui_MainWindow.h"

#include <QPainter>
#include <QPainterPath>

#include "Client.h"
#include "utils/AppLog.h"
#include "utils/Data.h"
#include "uicomponent/ChatListItem.h"

MainWindow::MainWindow(Client* client) :
    Window(nullptr, nullptr, "Whisperm"), m_client(client),ui(new Ui::MainWindow),m_currentPage(Main) {
    InitializeComponents();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::InitializeComponents()
{
    ui->setupUi(this->centralWidget);

    m_pages[Main] = ui->page_1;
    m_pages[Contacts] = ui->page_2;
    m_pages[More] = ui->page_3;
    m_pages[Settings] = ui->page_4;

    m_menubarBtns[Main] = ui->menubar_btnMain;
    m_menubarBtns[Contacts] = ui->menubar_btnContacts;
    m_menubarBtns[More] = ui->menubar_btnMore;
    m_menubarBtns[Settings] = ui->menubar_btnSettings;
    //Initialize menubar buttons
    for (MainWindowPage i = Main; i < Count; ++reinterpret_cast<int&>(i))
    {
        m_menubarBtns[i]->set_bgMargin(4);
        m_menubarBtns[i]->set_bgRadius(10);
        m_menubarBtns[i]->set_IconScale(0.5);
        m_menubarBtns[i]->set_bgColor(Button::Hover, {64,64,64, 64});
        connect(m_menubarBtns[i], &QPushButton::clicked, [this, i] { this->SwitchPage(i); });
    }
    ui->menubar_btnMain->SetContent("", QIcon(":/menu/chat.svg"));
    ui->menubar_btnContacts->SetContent("", QIcon(":/menu/contact.svg"));
    ui->menubar_btnMore->SetContent("", QIcon(":/menu/more.svg"));
    ui->menubar_btnSettings->SetContent("", QIcon(":/menu/navigate.svg"));

    //message receive/forward
    connect(m_client, &Client::ServerSignal_RecviedMessage, ui->page_1, &PageChats::ReceivedMessage);
    connect(ui->page_1, &PageChats::ForwardMessage, m_client, &Client::Client_ForwardMessage);

    const auto uid = m_client->get_UID();
    connect(m_client, &Client::ServerSignal_OnlineListChanged, this, &MainWindow::onOnlineList_Changed);
    connect(m_client, &Client::ServerSignal_LogoutResponse, [this](const QString& reason)
    {
        Log::Info("Logout: " + reason);
        this->close();
    });
    connect(ui->page_2, &PageContacts::DouleClicked, [this, uid](const QString& name)
    {
        bool already_exist = false;
        for (const auto item: ui->page_1->ChatList())
            if (item->get_UID() == name)
            {
                ui->page_1->SelectChat(item);
                this->SwitchPage(Main);
                already_exist = true;
                break;
            }
        if (!already_exist) ui->page_1->AppendChatItem({"Message", QDateTime::currentDateTime(), name, uid});

        onOnlineList_Changed();
    });
    ui->mainContent->installEventFilter(this);

    ui->page_1->set_UID_Client(m_client->get_UID());

    ui->page_1->AppendChatItem({"Message", QDateTime::currentDateTime(), uid, uid});

    this->onOnlineList_Changed();
    this->setMinimumSize(600, 450);
    this->UpdateContentSize();
}

void MainWindow::showEvent(QShowEvent* event)
{
    Window::showEvent(event);
    this->UpdateContentSize();
}

void MainWindow::UpdateContentSize() const
{
    const auto height = ui->mainContent->height();
    const auto width = ui->mainContent->width();
    ui->mainContainer->setGeometry(0, -m_currentPage * height, width, height * Count);
    for (int page = 0; page < Count; ++page)
        m_pages[page]->setGeometry(0, height * page, width, height);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Window::resizeEvent(event);
    this->UpdateContentSize();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->mainContent)
    {
        if (event->type() == QEvent::Paint)
        {
            QPainter painter(ui->mainContent);
            painter.setRenderHint(QPainter::Antialiasing);
            constexpr int radius = 8;
            constexpr int margin = 1;
            constexpr float width = 0.5;
            const QPen pen(Qt::gray, width);
            QPainterPath path;
            painter.setPen(pen);
            path.moveTo(margin, margin + radius);
            path.lineTo(margin, ui->mainContent->height() - margin);
            path.moveTo(margin + radius, margin);
            path.lineTo(ui->mainContent->width() - margin, margin);
            path.moveTo(margin + radius, margin);
            path.arcTo(QRectF(margin, margin, radius * 2, radius * 2), 90, 90);
            path.moveTo(ui->mainContent->width() - margin, ui->mainContent->height());
            painter.drawPath(path);
        }
    }
    return Window::eventFilter(watched, event);
}

void MainWindow::SwitchPage(const MainWindowPage page)
{
    if (page >= 0 && page< MainWindowPage::Count)
    {
        m_currentPage = page;
        this->UpdateContentSize();
    }
}

void MainWindow::onOnlineList_Changed() const
{
    for (const auto itemList = ui->page_1->ChatList(); const auto item : itemList)
        item->set_Online(m_client->Is_Online(item->get_UID()));
    ui->page_1->UpdateLabel();
    ui->page_2->UpdateList(m_client->get_OnlineList());
}
