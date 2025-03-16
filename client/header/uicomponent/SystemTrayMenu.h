#ifndef SYSTEMTRAYMENU_H
#define SYSTEMTRAYMENU_H

#include <QMenu>
#include <QSystemTrayIcon>

class Button;
class QSystemTrayIcon;


class SystemTrayMenu:public QObject{
    Q_OBJECT
public:
    explicit SystemTrayMenu(QSystemTrayIcon* trayIcon);

    void AddAction(Button* button);
    void AddActions(const QList<Button*>& buttons);

    [[nodiscard]] QMenu* Menu() {return &m_menu;}
    void set_Tray_DoubleClick(std::function<void()> f)
    { mf_Tray_DoubleClick = std::move(f);}
public Q_SLOTS:
    void onTrayIconTrigger(QSystemTrayIcon::ActivationReason reason);
private:
    QSystemTrayIcon* m_trayIcon;
    QMenu m_menu;

    std::function<void()> mf_Tray_DoubleClick;

    bool paintHandler(QPaintEvent* event);
    bool eventFilter(QObject* watched, QEvent* event) override;
};



#endif //SYSTEMTRAYMENU_H
