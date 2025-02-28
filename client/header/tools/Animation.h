#ifndef ANIMATION_H
#define ANIMATION_H

#include <QColor>
#include <QMap>

class TransitionColor {
public:
    TransitionColor()
    {

    }

private:
    QMap<QString, QColor> colors;
};



#endif //ANIMATION_H
