#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>

class QLabel;
class QDateTime;
class QPropertyAnimation;


class ChatListItem final : public QWidget{
    Q_OBJECT
    Q_PROPERTY(int bgOpacity READ get_bgOpacity WRITE set_bgOpacity)
public:
    ChatListItem(QWidget* parent, const QString& from, const QString& content, const QDateTime& time);

    void SetContent(const QString& from, const QString& content, const QDateTime& time) const;

    void set_bgOpacity(int opacity);
    void set_Online(const bool online) { mb_online = online; }
    [[nodiscard]] int get_bgOpacity() const;
    [[nodiscard]] QByteArray get_UID() const;
    [[nodiscard]] QString From() const;
    [[nodiscard]] QDateTime LastTime() const;
    [[nodiscard]] bool Is_Online() const { return mb_online; }
    void Unselect();
    void Select();
Q_SIGNALS:
    void Selected();
private:
    QLabel* uid;
    QLabel* content;
    QLabel* time;

    int m_bgOpacity = 0;
    bool mb_hoverd = false;
    bool mb_selected = false;
    bool mb_online = false;
    QPropertyAnimation* m_anim_bgOpacity;
    void Set_anim_bgOpacity(int opacity);

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};



#endif //CHATITEM_H
