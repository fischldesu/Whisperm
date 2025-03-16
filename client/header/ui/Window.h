#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "utils/Multiplatform.hpp"

class Window : public QWidget {
Q_OBJECT
    friend NativeWindowHelper;
public:
    explicit Window(QWidget *parent = nullptr, QWidget* custom_titlebar = nullptr, const QString& titleText = "");
    ~Window() override;

    virtual void set_WindowTitle(const QString& title);
    void set_CustomTitlebar(QWidget* titlebar);

    void set_Margins(const QMargins& margins);
    [[nodiscard]] QMargins get_Margins() const {return m_margin;}
private:
    void InitializeWindow();
protected:
    NativeWindowHelper m_helper;

    QWidget* centralWidget;
    QWidget* titlebarWidget;

    QMargins m_margin;

    void changeEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
};


#endif //WINDOW_H
