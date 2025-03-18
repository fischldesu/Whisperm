#include "utils/Animation.h"

ColorTransition::ColorTransition(QObject* parent)
    :QObject(parent), m_Animation(this, "property_color", this)
{
    m_Animation.setDuration(300);
    connect(&m_Animation, &QPropertyAnimation::finished, [this]{ emit Finished(); });
}

void ColorTransition::Transit(const QColor& from, const QColor& to)
{
    m_Animation.stop();
    m_Animation.setStartValue(from);
    m_Animation.setEndValue(to);
    m_Animation.start();
}
