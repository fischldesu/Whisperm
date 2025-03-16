#ifndef CHATBOX_H
#define CHATBOX_H

#include <QWidget>
#include "uicomponent/Chat.h"

namespace Data
{
    struct Message;
}
class QLabel;
class Chat;
class GroupChat;


template<typename T>
concept ChatType = std::is_same_v<T, FriendChat> || std::is_same_v<T, GroupChat>;

class ChatBox final :public QWidget {
public:
    explicit ChatBox(QWidget* parent = nullptr);

    void AppendMessage(const Data::Message& message);
    void AppendHistory(const QString& text, bool self = false);

    void set_UID_Client(const QString& uid);
    void set_UID_Target(const QString& uid);
    [[nodiscard]] QString get_UID_Target() const;
    [[nodiscard]] QString get_UID_Client() const;
private:
    QMap<QString, Chat*> m_ChatMap;
    Chat* m_current;
    QString m_UID_target;
    QString m_UID_client;
    QWidget* m_splashScreen;

    Chat* NewChat(const QString& target);
    Chat* NewChat(const QStringList& targets);

    void resizeEvent(QResizeEvent* event) override;

};



#endif //CHATBOX_H
