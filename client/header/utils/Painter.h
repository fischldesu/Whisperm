#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QPainter>

struct BorderRadius
{
    int TopLeft = 0;
    int TopRight = 0;
    int BottomLeft = 0;
    int BottomRight = 0;
};

namespace Paint {
    void Background(QWidget* widget, QColor color, QRect rect, qreal radius = 0);
    void Background(QWidget* widget, QColor color, QRect rect, const BorderRadius& radius);

    void Border(QWidget* widget, QColor color, QRect rect, int strokWidth, qreal radius = 0);
    void Border(QWidget* widget, QColor color, QRect rect, int strokWidth, const BorderRadius& radius);
};


#endif //PAINTER_H
