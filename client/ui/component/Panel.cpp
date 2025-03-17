#include "uicomponent/Panel.h"

#include <QBoxLayout>
#include <QEvent>

#include "utils/Style.h"

Panel::Panel(QWidget *parent)
    :QWidget(parent), m_IconContainer(new QWidget(this)),m_contentContainer(new QWidget(this))
{
    const auto vLayout = new QVBoxLayout(m_contentContainer);
    const auto hLayout = new QHBoxLayout(this);
    m_contentText = new QLabel(m_contentContainer);
    m_contentDetail = new QLabel(m_contentContainer);
    this->setLayout(hLayout);
    m_contentContainer->setLayout(vLayout);

    hLayout->addWidget(m_IconContainer);
    hLayout->addWidget(m_contentContainer);
    vLayout->addWidget(m_contentText);
    vLayout->addWidget(m_contentDetail);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(2);

    m_contentText->setStyleSheet("font-weight: bold;");
    m_contentDetail->setStyleSheet("color: gray;");
    m_contentDetail->hide();
    m_contentContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
}

void Panel::set_Controler(QWidget* controler)
{
    if (controler)
    {
        if (controler == m_IconContainer)
            return;

        if (m_Controler)
        {
            this->layout()->removeWidget(m_Controler);
            m_Controler->hide();
            m_Controler->deleteLater();
        }

        this->layout()->addWidget(controler);
        m_Controler = controler;
        m_Controler->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    }
}

void Panel::set_ShowDetail(const bool showDetail)
{
    if (showDetail)
    {
        m_contentDetail->show();
        this->setToolTip({});
    }
    else
    {
        m_contentDetail->hide();
        this->setToolTip(m_contentDetail->text());
    }
}

void Panel::paintEvent(QPaintEvent* event)
{
    Style::PaintBackground(this, {128, 128, 128, 32}, this->rect(), 8);
}

void Panel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_IconContainer->setFixedWidth(m_IconContainer->height());
}

bool Panel::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_IconContainer && event->type() == QEvent::Paint && !m_Icon.isNull())
    {
        const auto size = m_IconContainer->height() - 2;
        QPainter painter(m_IconContainer);
        m_Icon.paint(&painter, 1, 1, size, size);
    }
    return QWidget::eventFilter(watched, event);
}
