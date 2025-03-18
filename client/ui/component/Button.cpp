#include "uicomponent/Button.h"

#include <QPaintEvent>

#include "utils/Painter.h"

Button::Button(QWidget* parent) : QPushButton(parent)
{
    this->setFlat(true);
    this->setAutoFillBackground(false);
    this->setMouseTracking(true);
    this->setCursor(Qt::CursorShape::PointingHandCursor);

    connect(property_bgColor.Transitor(), &ColorTransition::Changing, [this]()
        {
            this->update();
        });
    this->set_bgColor(Normal, Qt::transparent);
    this->set_bgColor(Hover, Qt::black);
    this->set_bgColor(Pressed, Qt::black);
}

void Button::SetContent(const QString& text, const QIcon& icon)
{
    m_icon = icon;
    this->setText(text);
}

void Button::set_bgMargin(const int margin)
{
    if (margin < this->width() / 2 && margin < this->height() /2)
    {
        property_bgMargin = margin;
        this->update();
    }
}

void Button::set_bgRadius(const int radius)
{
    if (radius >= 0)
    {
        property_bgRadius = BorderRadius{radius, radius, radius, radius};
        this->update();
    }
}

void Button::set_bgRadius(const BorderRadius& radius)
{
    property_bgRadius = radius;
    this->update();
}

void Button::set_bgColor(const StyleState state, const QColor color)
{
    property_bgColor.set_Value(state, color);
    if (state == Normal)
        property_bgColor.Transitor()->property_set(color);
}

int Button::get_bgMargin() const
{
    return property_bgMargin;
}

void Button::paintEvent(QPaintEvent *event)
{
    const auto& text = this->text();
    const auto w = width();
    const auto h = height();
    const auto maxSize = w > h ? h : w;
    const auto iconSize = maxSize * property_iconScale;
    const auto icon_y = (h - iconSize) / 2;
    auto icon_x = (w - iconSize) / 2;
    QRect bgRect = this->rect();
    if (property_bgMargin != 0)
        this->rect().adjust(property_bgMargin, property_bgMargin, -property_bgMargin, -property_bgMargin);
    Paint::Background(this, property_bgColor.Value(), bgRect, property_bgRadius);
    QPainter painter{this};
    if (!text.isEmpty())
    {
        int flag = Qt::AlignCenter;
        painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
        icon_x = (maxSize - iconSize) / 2 + property_bgMargin;
        if (!m_icon.isNull())
        {
            bgRect.adjust(maxSize + icon_x / 2, 0, 0, 0);
            flag = Qt::AlignVCenter | Qt::AlignLeft;
        }
        painter.setPen(this->palette().color(QPalette::ButtonText));
        painter.setFont(this->font());
        painter.drawText(bgRect, flag, text);
    }
    m_icon.paint(&painter, icon_x, icon_y, iconSize, iconSize);
}

void Button::mousePressEvent(QMouseEvent* event)
{
    if(property_styleState != Pressed)
    {
        property_styleState = Pressed;
        this->Transition();
    }
    QPushButton::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QMouseEvent* event)
{
    if(property_styleState == Pressed)
    {
        property_styleState = Hover;
        this->Transition();
    }
    QPushButton::mouseReleaseEvent(event);
}

void Button::mouseMoveEvent(QMouseEvent* event)
{
    QPushButton::mouseMoveEvent(event);
}

void Button::enterEvent(QEnterEvent* event)
{
    property_styleState = Hover;
    this->Transition();
    return QPushButton::enterEvent(event);
}

void Button::leaveEvent(QEvent* event)
{
    property_styleState = Normal;
    this->Transition();
    return QPushButton::leaveEvent(event);
}

void Button::Transition()
{
    property_bgColor.Animate(property_styleState);
}