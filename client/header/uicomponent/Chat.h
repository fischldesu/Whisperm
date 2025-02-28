#ifndef CHAT_H
#define CHAT_H

#include "uicomponent/ChatMessage.h"
class ChatBox;
class QSpacerItem;
namespace Data
{
    struct Message;
}

class Chat:public QWidget{
    Q_OBJECT
public:
    virtual void AppendMessage(const Data::Message& message) = 0;
    [[nodiscard]] virtual bool Is_GroupChat() const {return mb_GroupChat;}
protected:
    explicit Chat(ChatBox* parent);

    bool mb_GroupChat;
    ChatBox* m_parent;
    QSpacerItem* m_spacer;
    QList<ChatMessage*> m_Messages;

};

class FriendChat:public Chat
{
    Q_OBJECT
public:
    FriendChat(ChatBox* chatBox, QString  target);

    void AppendMessage(const Data::Message& message) override;
private:
    QString m_target;

};

// public:
//     Chat(ChatBox* chatBox, const QString& target);
//     Chat(ChatBox* chatBox, const QStringList& targets);
//
//     void AppendMessage(const Data::Message& message);
//
//     [[nodiscard]] QString get_Target() const;
//     [[nodiscard]] QStringList get_GroupTargets() const;
//     [[nodiscard]] bool is_GroupChat() const { return mb_GroupChat; }
// private:
//     const ChatBox* m_parent;
//     const bool mb_GroupChat;
//     const QString m_target;
//     const QStringList* m_targets;
//     QList <ChatMessage*> m_Messages;
//     QSpacerItem* spacer;
//     void InitializeLayout();
//     void NewMessage(ChatMessage* widget);
//
//     void paintEvent(QPaintEvent* event) override;
#endif //CHAT_H
