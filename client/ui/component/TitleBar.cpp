#include "uicomponent/TitleBar.h"

#include <QPushButton>
#include <QApplication>
#include <QResizeEvent>
#include <QLabel>
#include "uicomponent//Button.h"

TitleBar::TitleBar(QWidget* parent):QWidget(parent), m_title(new QLabel(this))
{
    for (WindowBtn i = Close; i < Count; ++reinterpret_cast<int&>(i))
    {
        m_btns[i] = new Button(this);
        m_btns[i]->resize(50, 32);
        m_btns[i]->set_bgHoverColor({64,64,64, 64});
        m_btns[i]->set_IconScale(0.5);
    }
    m_btns[Close]->set_bgHoverColor({Qt::red});
    m_btns[Close]->SetContent("", QIcon{":/titlebar/close.svg"});
    m_btns[Minimize]->SetContent("", QIcon{":/titlebar/minimize.svg"});
    m_btns[Maximize]->SetContent("", QIcon{":/titlebar/maximize.svg"});
    connect(m_btns[Close], &QPushButton::clicked, this->parentWidget(), &QWidget::close);
    connect(m_btns[Minimize], &QPushButton::clicked, this->parentWidget(), &QWidget::showMinimized);
    connect(m_btns[Maximize], &QPushButton::clicked, [this]()
    {
        if (const auto parentWidget = this->parentWidget(); parentWidget->isMaximized())
        {
            parentWidget->showNormal();

        }
        else
        {
            parentWidget->showMaximized();

        }
    });
}

TitleBar::~TitleBar() = default;

void TitleBar::set_TitleText(const QString& text)
{
    m_title->setText(text);
}

void TitleBar::set_TitleIcon(const QIcon& icon)
{

}

void TitleBar::set_TitleIcon(const QPixmap& icon)
{

}

void TitleBar::resizeEvent(QResizeEvent* event)
{
    m_title->setGeometry(30, this->height() / 2 - 15, 256, 30);
    for (WindowBtn i = Close; i < Count; ++reinterpret_cast<int&>(i))
    {
        m_btns[i]->move(this->width() - 50 * (i + 1), 0);
    }
    QWidget::resizeEvent(event);
}
