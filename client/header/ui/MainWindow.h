#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Window.h"

class Button;
class Client;
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
namespace Data
{
    struct Message;
}
QT_END_NAMESPACE

class MainWindow final : public Window
{
    Q_OBJECT

public:
    explicit MainWindow(Client* client);
    ~MainWindow() override;
    enum MainWindowPage:int
    {
        Main,
        Contacts,
        More,
        Settings,
        Count
    };
    void UpdateContentSize() const;
private:
    Client* m_client;
    Ui::MainWindow* ui;
    MainWindowPage m_currentPage;
    QWidget* m_pages[Count];
    Button* m_menubarBtns[Count];
    void InitializeComponents();

    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
public Q_SLOTS:
    void SwitchPage(MainWindowPage page);
    void onOnlineList_Changed() const;
};


#endif // MAINWINDOW_H
