#include "ui/Window.h"

#include <QApplication>
#include <QEvent>
#include <QOperatingSystemVersion>
#include <QPainter>
#include <QStyleOption>

#include "uicomponent/TitleBar.h"

Window::Window(QWidget* parent, QWidget* custom_titlebar, const QString& titleText) :
    QWidget(parent), m_helper(this),centralWidget(new QWidget(this)), titlebarWidget(custom_titlebar)
{
    this->setWindowTitle(titleText);
    this->InitializeWindow();
}

Window::~Window() = default;

void Window::setWindowTitle(const QString& title)
{
    const auto titleWidget = qobject_cast<TitleBar*>(this->titlebarWidget);
    if (titleWidget)
    {
        titleWidget->set_TitleText(title);
    }
    return QWidget::setWindowTitle(title);
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

    painter.fillRect(this->rect(), m_helper.bgColor());
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);
}

bool Window::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    return m_helper.NativeEventHandler(eventType, message, result);
}
