#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
class QLabel;
class Button;

class TitleBar: public QWidget
{
    Q_OBJECT
public:
    enum WindowBtn:int
    {
        Close,
        Maximize,
        Minimize,
        Count
    };
    explicit TitleBar(QWidget* parent = nullptr);
    ~TitleBar() override;

    void set_TitleText(const QString& text);
    void set_TitleIcon(const QIcon& icon);
    void set_TitleIcon(const QPixmap& icon);
private:
    QHash<QString, QWidget*> m_controls;
    Button* m_btns[Count];
    QLabel* m_title;
    void resizeEvent(QResizeEvent* event) override;
};



#endif //TITLEBAR_H
