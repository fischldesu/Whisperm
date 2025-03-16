#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include "ui/Window.h"

class QEventLoop;
class MessageDialogTitlebar;

class MessageDialog: public Window
{
    Q_OBJECT
public:
    enum Type
    {
        Window,
        Info,
        Warning,
        Error
    };
    explicit MessageDialog(Type type, QWidget* parent = nullptr);

    void set_WindowTitle(const QString& title) override;
    void set_Content(const QString& content);
    void set_Content(QWidget* contentWidget);

    virtual int exec(bool modal);
public Q_SLOTS:
    void Accept(const int code = 1) { this->QuitWithCode(code); }
    void Reject(const int code = 0) { this->QuitWithCode(code); }
Q_SIGNALS:
    void Quit(int code);
protected:
    const Type m_type;
    bool mb_Quit = false;
    QEventLoop* m_eventLoop;

    MessageDialogTitlebar* m_CustomTitlebar;
    QWidget* m_ContentWidget;

    void InitilizeType();
    void QuitWithCode(int code);

    void closeEvent(QCloseEvent* event) override;
};

#endif //MESSAGEDIALOG_H
