#ifndef BTNTEXT_H
#define BTNTEXT_H

#include <QPropertyAnimation>
#include <QPushButton>

#include "tools/AppLog.h"
#include "tools/Style.h"
class QLabel;
class QPropertyAnimation;

class Button : public QPushButton{
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ property_get_bgColor WRITE property_set_bgColor)
public:
    explicit Button(QWidget* parent);

    void SetContent(const QString& text = {}, const QIcon& icon = {});
    //margin
    void set_bgMargin(int margin);
    [[nodiscard]]
    int get_bgMargin() const;
    //border-radius
    void set_bgRadius(int radius);
    void set_bgRadius(const BorderRadius& radius);
    [[nodiscard]]
    BorderRadius get_bgRadius() const { return m_bgRadius; }

    //bg-color
    void set_bgColor (const QColor color){ m_bgColor = color; this->property_set_bgColor(m_bgColor); }
    void set_bgHoverColor(const QColor color){ m_bgHoverColor = color; }
    [[nodiscard]]
    QColor get_bgColor() const { return m_bgColor; }
    [[nodiscard]]
    QColor get_bgHoverColor() const { return m_bgHoverColor; }
    //AnimationDuaration
    void set_TransitionTime(const int msec) const {m_Transiton_bgColor->setDuration(msec);}
    [[nodiscard]]
    int get_TransitionTime() const { return m_Transiton_bgColor->duration(); }
    //icon scale
    void set_IconScale(const qreal scale) { m_iconScale = std::clamp(scale, 0.0, 1.0); }
    [[nodiscard]] double get_IconScale() const { return m_iconScale; }

    void property_set_bgColor(const QColor color)
    {
        property_bgColor = color;
        this->update();
    }
    [[nodiscard]]
    QColor property_get_bgColor() const { return property_bgColor; }
private:
    QColor property_bgColor{Qt::transparent};

    int m_bgMargin = 0;
    BorderRadius m_bgRadius{};
    QColor m_bgColor{Qt::transparent};
    QColor m_bgHoverColor{Qt::black};

    qreal m_iconScale = 1;
    QPropertyAnimation* m_Transiton_bgColor;

    void Transition(QColor new_color) const;
    QIcon m_icon;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
};



#endif //BTNTEXT_H
