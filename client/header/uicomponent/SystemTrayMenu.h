#ifndef SYSTEMTRAYMENU_H
#define SYSTEMTRAYMENU_H

#include <QMenu>
class Button;
class QSystemTrayIcon;

class SystemTrayMenu:public QObject{
    Q_OBJECT
public:
    explicit SystemTrayMenu(QSystemTrayIcon* trayIcon);

    void AddAction(Button* button);
    void AddActions(QList<Button*> buttons);

    [[nodiscard]] QMenu* Menu() {return &m_menu;}
private:
    QSystemTrayIcon* m_trayIcon;
    QMenu m_menu;

    bool eventFilter(QObject* watched, QEvent* event) override;
    bool paintHandler(QPaintEvent* event);
};



#endif //SYSTEMTRAYMENU_H
