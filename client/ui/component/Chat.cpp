#include "uicomponent/Chat.h"

#include <QVBoxLayout>
#include "uicomponent/ChatBox.h"
#include "tools/Data.h"

Chat::Chat(ChatBox* parent):QWidget(parent), mb_GroupChat(false), m_parent(parent)
{
    const auto layout = new QVBoxLayout(this);
    this->setLayout(layout);
    m_spacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);;
    layout->addSpacerItem(m_spacer);
    this->hide();
}

FriendChat::FriendChat(ChatBox* chatBox, QString target)
    :Chat(chatBox), m_target(std::move(target))
{
    mb_GroupChat = false;
}

void FriendChat::AppendMessage(const Data::Message& message)
{
    const auto self = m_parent->get_UID_Client() == message.from;
    const auto newChatMessage = new ChatMessage(this, self, message.text);
    const auto layout = dynamic_cast<QVBoxLayout*>(this->layout());

    layout->insertWidget(layout->indexOf(m_spacer), newChatMessage);
    m_Messages.append(newChatMessage);
}
