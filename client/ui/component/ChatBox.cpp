#include "uicomponent/ChatBox.h"

#include <QLabel>
#include <QLayout>
#include <QPainter>
#include "Client.h"
#include "tools/Data.h"
#include "tools/AppLog.h"

ChatBox::ChatBox(QWidget* parent):QWidget(parent),m_current(nullptr),m_splashScreen(new QWidget(this))
{
    const auto layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_splashScreen);

}

void ChatBox::AppendMessage(const Data::Message& message)
{
    const auto key = (message.from == m_UID_client) ? message.towhom : message.from;
    Chat* chat = m_ChatMap.value(key, nullptr);
    if (!chat)
    {
        chat = new FriendChat(this, key);
        this->layout()->addWidget(chat);
        m_ChatMap.insert(key, chat);
    }
    chat->AppendMessage(message);
}

void ChatBox::AppendHistory(const QString& text, bool self)
{

}

void ChatBox::set_UID_Client(const QString& uid)
{
    m_UID_client = uid;
}

void ChatBox::set_UID_Target(const QString& uid)
{
    if (m_splashScreen)
    {
        m_splashScreen->setParent(nullptr);
        m_splashScreen->hide();
        m_splashScreen->deleteLater();
        m_splashScreen = nullptr;
    }

    if (m_current) m_current->hide();

    auto chat = m_ChatMap.value(uid, nullptr);
    if (!chat)
        chat = NewChat(uid);
    m_current = chat;
    m_current->show();
    m_UID_target = uid;
}

QString ChatBox::get_UID_Target() const
{
    return m_UID_target;
}

QString ChatBox::get_UID_Client() const
{
    return m_UID_client;
}

Chat* ChatBox::NewChat(const QString& target)
{
    const auto chat = new FriendChat{this, target};
    this->layout()->addWidget(chat);
    m_ChatMap.insert(target, chat);
    return chat;
}

Chat* ChatBox::NewChat(const QStringList& targets)
{
    // const auto chat = new GroupChat{this, targets};
    // this->layout()->addWidget(chat);
    // m_ChatMap.insert(message.from, chat);
    return nullptr;
}

void ChatBox::resizeEvent(QResizeEvent* event)
{
    if (m_current)
    {
        const auto contentMargin = layout()->contentsMargins();
        m_current->resize(this->width() - contentMargin.right() - contentMargin.left(), m_current->height());
    }
    QWidget::resizeEvent(event);
}


