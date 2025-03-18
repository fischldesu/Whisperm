#include "uicomponent/SystemTrayMenu.h"

#include <QApplication>
#include <QPaintEvent>
#include <QWidgetAction>
#include <QSystemTrayIcon>

#include "utils/Painter.h"
#include "uicomponent/Button.h"

SystemTrayMenu::SystemTrayMenu(QSystemTrayIcon* trayIcon):m_trayIcon(trayIcon)
{
    m_trayIcon->setContextMenu(&m_menu);
    m_trayIcon->setToolTip(QApplication::applicationName());
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &SystemTrayMenu::onTrayIconTrigger);
    m_menu.installEventFilter(this);
}

void SystemTrayMenu::AddAction(Button* button)
{
    const auto action = new QWidgetAction(&m_menu);
    action->setDefaultWidget(button);
    m_menu.addAction(action);
    button->set_bgColor(Button::Hover, {64, 64, 64, 64});
    button->set_bgRadius(7);
    button->set_IconScale(0.8);
    connect(button, &QPushButton::clicked, [this, button]
            { m_menu.close(); QApplication::postEvent(button, new QEvent(QEvent::Leave)); });
}

void SystemTrayMenu::AddActions(const QList<Button*>& buttons)
{
    for (const auto button: buttons)
        this->AddAction(button);
}

void SystemTrayMenu::onTrayIconTrigger(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        if (mf_Tray_DoubleClick)
            mf_Tray_DoubleClick();
    }
}

bool SystemTrayMenu::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == &m_menu)
    {
        switch (event->type())
        {
        case QEvent::Paint:
            return paintHandler(dynamic_cast<QPaintEvent*>(event));
        default: break;
        }
    }
    return QObject::eventFilter(watched, event);
}

bool SystemTrayMenu::paintHandler(QPaintEvent* event)
{
    //
    return false;
}

