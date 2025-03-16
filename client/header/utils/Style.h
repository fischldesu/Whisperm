#ifndef STYLE_H
#define STYLE_H

#include <QWidget>
#include <QPainter>

struct BorderRadius
{
    int TopLeft = 0;
    int TopRight = 0;
    int BottomLeft = 0;
    int BottomRight = 0;
};

namespace  Style {
    void PaintBackground(QWidget* widget, QColor color, QRect rect, qreal radius = 0);
    void PaintBackground(QWidget* widget, QColor color, QRect rect, const BorderRadius& radius);


};



#endif //STYLE_H
