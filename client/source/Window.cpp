#include "ui/Window.h"

#include <QApplication>
#include <QCloseEvent>
#include <QPainter>
#include <QStyleOption>

#include "uicomponent/TitleBar.h"

Window::Window(QWidget* parent, QWidget* custom_titlebar, const QString& titleText) :
    QWidget(parent), m_helper(this),centralWidget(new QWidget(this)), titlebarWidget(custom_titlebar)
{
    this->InitializeWindow();
    this->Window::set_WindowTitle(titleText);
}

Window::~Window() = default;

void Window::set_WindowTitle(const QString& title)
{
    if (const auto titlebarWidget_ = qobject_cast<TitleBar*>(this->titlebarWidget))
        titlebarWidget_->set_TitleText(title);
    return setWindowTitle(title);
}

void Window::set_CustomTitlebar(QWidget* titlebar)
{
    const auto old_Titlbar = this->titlebarWidget;
    const auto resizeEvent = new QEvent(QEvent::Resize);
    this->titlebarWidget = titlebar;
    m_helper.set_titlebar(titlebar);
    old_Titlbar->setParent(nullptr);
    QApplication::sendEvent(this, resizeEvent);
    delete old_Titlbar;
    delete resizeEvent;
}

void Window::set_Margins(const QMargins& margins)
{
    m_margin = margins;
    QApplication::postEvent(this, new QEvent(QEvent::Resize));
}

void Window::InitializeWindow()
{
    auto titlebarHeight = 50;
    this->resize(800, 600);
    if (titlebarWidget)
    {
        titlebarWidget->setParent(this);
        titlebarHeight = titlebarWidget->height();
    }
    else
    {
        const auto titlebar = new TitleBar(this);
        titlebar->set_TitleText(this->windowTitle());
        titlebarWidget = titlebar;
    }
    const auto contentWidth = this->width() - m_margin.left() - m_margin.right();
    titlebarWidget->setGeometry(m_margin.left(), m_margin.top(), contentWidth, titlebarHeight);
    centralWidget->setGeometry(
        m_margin.left(),
        titlebarHeight + m_margin.top(),
        contentWidth,
        this->height() - titlebarHeight - m_margin.top() - m_margin.bottom());
    m_helper.set_titlebar(titlebarWidget);
    m_helper.InitWindowStyle(2);
    // m_currentTheme = GetCurrentTheme();
}


void Window::changeEvent(QEvent* event)
{
    return QWidget::changeEvent(event);
}

void Window::resizeEvent(QResizeEvent* event)
{
    const auto titlebarHeight = titlebarWidget->height();
    const auto contentWidth = this->width() - m_margin.left() - m_margin.right();
    titlebarWidget->setGeometry(m_margin.left(), m_margin.top(), contentWidth, titlebarHeight);
    centralWidget->setGeometry(
        m_margin.left(),
        titlebarHeight + m_margin.top(),
        contentWidth,
        this->height() - titlebarHeight - m_margin.top() - m_margin.bottom());
    return QWidget::resizeEvent(event);
}

void Window::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    QPainter painter(this);
    opt.initFrom(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    painter.fillRect(this->rect(), m_helper.get_bgColor());
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);
}

void Window::closeEvent(QCloseEvent* event)
{
    if (QApplication::quitOnLastWindowClosed())
    {
        event->accept();
    }
    else
    {
        this->hide();
        event->ignore();
    }
}

void Window::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    m_helper.InitWindowStyle(2);
    this->activateWindow();
}

bool Window::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    return m_helper.EventHandler_NativeEvent(eventType, message, result);
}
