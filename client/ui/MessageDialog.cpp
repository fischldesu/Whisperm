#include "ui/MessageDialog.h"

#include <QApplication>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QLabel>

class MessageDialogTitlebar: public QWidget
{
public:
    explicit MessageDialogTitlebar(MessageDialog* parent);
    void set_Text(const QString& text) const;
private:
    QLabel* m_text;
};

MessageDialog::MessageDialog(const Type type, QWidget* parent)
: ::Window(parent), m_type(type), m_eventLoop(nullptr), m_CustomTitlebar(nullptr), m_ContentWidget(nullptr)
{
    this->InitilizeType();
}

void MessageDialog::set_WindowTitle(const QString& title)
{
    Window::set_WindowTitle(title);
    if (m_CustomTitlebar)
        m_CustomTitlebar->set_Text(title);
}

void MessageDialog::set_Content(const QString& content)
{

    if (m_ContentWidget)
    {
        m_ContentWidget->setParent(nullptr);
        delete m_ContentWidget;
    }
    const auto textContent = new QPlainTextEdit(content, this->centralWidget);
    m_ContentWidget = textContent;
    m_ContentWidget->setStyleSheet("background-color: transparent; border: none;");
    if (!this->centralWidget->layout())
        this->centralWidget->setLayout(new QGridLayout(this->centralWidget));
    this->centralWidget->layout()->addWidget(m_ContentWidget);
}

void MessageDialog::set_Content(QWidget* contentWidget)
{
    const auto old_Content = m_ContentWidget;
    m_ContentWidget = contentWidget;
    m_ContentWidget->setParent(this->centralWidget);
    if (old_Content)
    {
        old_Content->setParent(nullptr);
        delete old_Content;
    }
    if (!this->centralWidget->layout())
        this->centralWidget->setLayout(new QGridLayout(this->centralWidget));
    this->centralWidget->layout()->addWidget(m_ContentWidget);
}

int MessageDialog::exec(const bool modal)
{
    auto modal_ = Qt::NonModal;
    if (modal)
        modal_ = Qt::ApplicationModal;

    this->setWindowModality(modal_);
    this->show();
    QEventLoop eventLoop;
    // ReSharper disable once CppDFALocalValueEscapesFunction
    m_eventLoop = &eventLoop;
    const auto ret = eventLoop.exec();
    m_eventLoop = nullptr;
    return ret;
}

void MessageDialog::InitilizeType()
{
    if (m_type != Window)
    {
        set_CustomTitlebar(new MessageDialogTitlebar(this));
    }
}

void MessageDialog::QuitWithCode(const int code)
{
    if (m_eventLoop)
        m_eventLoop->exit(code);

    mb_Quit = true;
    emit Quit(code);
    this->close();
}

void MessageDialog::closeEvent(QCloseEvent* event)
{
    if(!mb_Quit)
        this->Reject();

    event->accept();
}

MessageDialogTitlebar::MessageDialogTitlebar(MessageDialog* parent)
:QWidget(parent), m_text(new QLabel(this))
{
    this->setFixedHeight(32);
    m_text->setGeometry(16, 0, 256, 32);
}

void MessageDialogTitlebar::set_Text(const QString& text) const
{ m_text->setText(text); }