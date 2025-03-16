#ifndef PAGECHATS_H
#define PAGECHATS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class PageChats; }
QT_END_NAMESPACE
namespace Data
{
    struct Message;
}
class Chat;
class QLabel;
class ChatListItem;

class PageChats : public QWidget {
Q_OBJECT

public:
    explicit PageChats(QWidget *parent = nullptr);
    ~PageChats() override;

    void AppendChatItem(const Data::Message& message);
    void SortChatItems();
    void SelectChat(const ChatListItem* selected);

    void UpdateLabel();

    void set_UID_Client(const QString& uid);
    [[nodiscard]] QString get_UID_Client() const { return m_UID; }
    [[nodiscard]] QList<ChatListItem*>& ChatList() {return m_itemListContainer; }
public Q_SLOTS:
    void ReceivedMessage(const Data::Message& message);
Q_SIGNALS:
    void ForwardMessage(const Data::Message& message);
private:
    QString m_UID;
    QList<ChatListItem*> m_itemListContainer;
    QLabel* m_UIDLabel;
    Ui::PageChats *ui;

    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
private Q_SLOTS:
    void onSendMessage_Clicked();
};


#endif //PAGECHATS_H
