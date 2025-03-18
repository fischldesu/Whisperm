#include "utils/Painter.h"

#include <QPainterPath>

QPainterPath RoundedRectPath(const QRect rect, const BorderRadius& radius)
{
    QPainterPath path;
    if (radius.TopLeft > 0) {
        path.moveTo(rect.left(), rect.top() + radius.TopLeft);
        path.arcTo(rect.left(), rect.top(), 2 * radius.TopLeft, 2 * radius.TopLeft, 180, -90);
    } else {
        path.moveTo(rect.topLeft());
    }
    if (radius.TopRight > 0) {
        path.lineTo(rect.right() - radius.TopRight, rect.top());
        path.arcTo(rect.right() - 2 * radius.TopRight, rect.top(), 2 * radius.TopRight, 2 * radius.TopRight, 90, -90);
    } else {
        path.lineTo(rect.topRight());
    }
    if (radius.BottomRight > 0) {
        path.lineTo(rect.right(), rect.bottom() - radius.BottomRight);
        path.arcTo(rect.right() - 2 * radius.BottomRight, rect.bottom() - 2 * radius.BottomRight, 2 * radius.BottomRight, 2 * radius.BottomRight, 0, -90);
    } else {
        path.lineTo(rect.bottomRight());
    }
    if (radius.BottomLeft > 0) {
        path.lineTo(rect.left() + radius.BottomLeft, rect.bottom());
        path.arcTo(rect.left(), rect.bottom() - 2 * radius.BottomLeft, 2 * radius.BottomLeft, 2 * radius.BottomLeft, 270, -90);
    } else {
        path.lineTo(rect.bottomLeft());
    }
    path.closeSubpath();
    return path;
}

void Paint::Background(QWidget* widget, const QColor color, const QRect rect, const qreal radius)
{
    QPainter painter(widget);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, radius, radius);
}

void Paint::Background(QWidget* widget, const QColor color, const QRect rect, const BorderRadius& radius)
{
    QPainter painter(widget);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawPath(RoundedRectPath(rect, radius));
}

void Paint::Border(QWidget* widget, QColor color, QRect rect, int lineWidth, qreal radius)
{
    const auto adj = lineWidth/2;
    QPainter painter(widget);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(color, lineWidth));
    painter.drawRoundedRect(rect, radius, radius);
}

void Paint::Border(QWidget* widget, QColor color, QRect rect, int lineWidth, const BorderRadius& radius)
{
    QPainter painter(widget);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(color, lineWidth));
    painter.drawPath(RoundedRectPath(rect, radius));
}

