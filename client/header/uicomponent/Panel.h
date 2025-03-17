#ifndef PANEL_H
#define PANEL_H

#include <QLabel>

class Panel: public QWidget{
    Q_OBJECT
public:
    explicit Panel(QWidget* parent);

    void set_Text(const QString& text) const
    { m_contentText->setText(text); }
    [[nodiscard]] QString get_Text() const
    { return m_contentText->text(); }

    void set_Detail(const QString& detail) const
    { m_contentDetail->setText(detail); m_contentDetail->setVisible(!detail.isEmpty()); }
    [[nodiscard]]
    QString get_Detail() const
    { return m_contentDetail->text(); }

    void set_Content(const QPair<QString, QString>& content) const
    { this->set_Text(content.first); this->set_Detail(content.second); }
    [[nodiscard]]
    QPair<QString, QString> get_Content() const
    { return {this->get_Text(), this->get_Detail()}; }

    void set_Controler(QWidget* controler);
    void set_ShowDetail(bool showDetail);
    [[nodiscard]]
    bool get_Is_ShowDetail() const
    { return m_contentDetail->isVisible(); }

    void set_Icon(const QIcon& icon)
    { m_Icon = icon; }
    [[nodiscard]]
    QIcon get_Icon() const
    { return m_Icon; }
protected:
    QIcon m_Icon;

    QWidget* m_Controler = nullptr;
    QWidget* m_IconContainer;
    QWidget* m_contentContainer;
    QLabel* m_contentText;
    QLabel* m_contentDetail;

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif //PANEL_H
