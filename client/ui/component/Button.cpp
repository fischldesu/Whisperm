#include "uicomponent/Button.h"

#include <QPaintEvent>
#include <QPropertyAnimation>
#include "tools/Style.h"

Button::Button(QWidget* parent) : QPushButton(parent)
{
    m_Transiton_bgColor = new QPropertyAnimation(this, "bgColor", this);
    m_Transiton_bgColor->setParent(this);
    m_Transiton_bgColor->setDuration(200);
    this->setFlat(true);
    this->setAutoFillBackground(false);
    this->setMouseTracking(true);
    this->setCursor(Qt::CursorShape::PointingHandCursor);
}

void Button::SetContent(const QString& text, const QIcon& icon)
{
    m_icon = icon;
}

void Button::set_bgMargin(const int margin)
{
    if (margin < this->width() / 2 && margin < this->height() /2)
    {
        m_bgMargin = margin;
        this->update();
    }
}

void Button::set_bgRadius(const int radius)
{
    if (radius >= 0)
    {
        m_bgRadius = BorderRadius{radius, radius, radius, radius};
        this->update();
    }
}

void Button::set_bgRadius(const BorderRadius& radius)
{
    m_bgRadius = radius;
    this->update();
}

int Button::get_bgMargin() const
{
    return m_bgMargin;
}


void Button::Transition(const QColor new_color) const
{
    m_Transiton_bgColor->stop();
    m_Transiton_bgColor->setStartValue(property_bgColor);
    m_Transiton_bgColor->setEndValue(new_color);
    m_Transiton_bgColor->start();
}

void Button::paintEvent(QPaintEvent* event)
{
    const auto& text = this->text();
    const auto w = width();
    const auto h = height();
    const auto iconSize = (w > h ? h : w) * m_iconScale;
    const auto icon_y = (h - iconSize) / 2;
    auto icon_x = (w - iconSize) / 2;
    auto bgRect = this->rect().adjusted(m_bgMargin, m_bgMargin, -m_bgMargin, -m_bgMargin);
    Style::PaintBackground(this, property_bgColor, bgRect, m_bgRadius);
    QPainter painter{this};
    if (!text.isEmpty())
    {
        icon_x = 0;
        if (!m_icon.isNull())
            bgRect.adjust(w > h ? h : w, 0, 0, 0);
        painter.setPen(this->palette().color(QPalette::ButtonText));
        painter.setFont(this->font());
        painter.drawText(bgRect, Qt::AlignCenter, text);
    }
    m_icon.paint(&painter, icon_x, icon_y, iconSize, iconSize);
}

void Button::enterEvent(QEnterEvent* event)
{
    this->Transition(m_bgHoverColor);
    return QPushButton::enterEvent(event);
}

void Button::leaveEvent(QEvent* event)
{
    this->Transition(m_bgColor);
    return QPushButton::leaveEvent(event);
}
