#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPropertyAnimation>
#include <QColor>

#include "utils/Enum.h"

class ColorTransition:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor property_color_ READ property_get WRITE property_set)
public:
    explicit ColorTransition(QObject* parent = nullptr);

    void Transit(const QColor& from, const QColor& to);
    void Stop() { m_Animation.stop(); }
    QColor Value() const { return property_get(); }

    void set_Duration(const int msec)
    { m_Animation.setDuration(msec); }
    [[nodiscard]]
    int get_Duration() const
    { return m_Animation.duration(); }

    void set_TargetProperty(const QByteArray& propertyName)
    { m_Animation.setPropertyName(propertyName); }

    void property_set(const QColor& value) { property_value = value; emit Changing(); }
    QColor property_get() const { return property_value; }
Q_SIGNALS:
    void Finished();
    void Changing();
private:
    QColor property_value;
    QPropertyAnimation m_Animation;
};

namespace Animation
{
    template <EnumTypename E>
    class Color
    {
    public:
        explicit Color(::ColorTransition* color):m_Animation(color)
        {
        }
        Color():m_Animation(new ::ColorTransition)
        {
        }
        ~Color() { m_Animation->Stop(); m_Animation->deleteLater(); }

        void set_Value(const E key, QColor color)
        { m_ColorMap.insert(key, color); }
        [[nodiscard]]
        QColor get_Value(const E key) const
        { return m_ColorMap.value(key); }

        [[nodiscard]]
        ColorTransition* Transitor() const
        { return m_Animation; }

        [[nodiscard]]
        QColor Value() const
        { return m_Animation->Value(); }

        void Animate(E to);
        void Animate(const QColor& to) const;
    private:
        QMap<E, QColor> m_ColorMap;
        ::ColorTransition* m_Animation;
    };

    template <EnumTypename E>
    void Color<E>::Animate(E to)
    {
        this->Animate(this->get_Value(to));
    }

    template <EnumTypename E>
    void Color<E>::Animate(const QColor& to) const
    {
        m_Animation->Transit(m_Animation->property_get(), to);
    }

}


#endif //ANIMATION_H
