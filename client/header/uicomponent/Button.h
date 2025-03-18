#ifndef BTNTEXT_H
#define BTNTEXT_H

#include <QPropertyAnimation>
#include <QPushButton>
#include "utils/Animation.h"
#include "utils/Painter.h"
#include "utils/AppLog.h"

class QLabel;
class QPropertyAnimation;


class Button : public QPushButton{

    Q_OBJECT
public:
    enum StyleState
    {
        Normal = 1 << 0,
        Hover = 1 << 1,
        Pressed = 1 << 2,
    };

    explicit Button(QWidget* parent);

    void SetContent(const QString& text = {}, const QIcon& icon = {});

    void set_bgMargin(int margin);
    int get_bgMargin() const;

    void set_bgRadius(int radius);
    void set_bgRadius(const BorderRadius& radius);
    BorderRadius get_bgRadius() const { return property_bgRadius; }

    void set_IconScale(const qreal scale) { property_iconScale = std::clamp(scale, 0.0, 1.0); }
    double get_IconScale() const { return property_iconScale; }

    void set_textColor(const QColor color) { auto pal = this->palette(); pal.setColor(QPalette::ButtonText, color); this->setPalette(pal);}
    QColor get_textColor() const { return this->palette().color(QPalette::ButtonText); }

    void set_bgColor(StyleState state, QColor color);
    QColor get_bgColor(const StyleState state) const { return property_bgColor.get_Value(state); }
private:
    int property_bgMargin = 0;
    qreal property_iconScale = 1;
    StyleState property_styleState = Normal;
    BorderRadius property_bgRadius{};
    Animation::Color<StyleState> property_bgColor;

    QIcon m_icon;
    bool mb_SameBorderRadius = true;
    bool mb_Hoverd = false;

    void Transition();

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
};



#endif //BTNTEXT_H
