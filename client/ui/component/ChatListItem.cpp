#include "uicomponent/ChatListItem.h"

#include <QDateTime>
#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "tools/Style.h"

ChatListItem::ChatListItem(QWidget* parent, const QString& from, const QString& content, const QDateTime& time)
:QWidget(parent), uid(new QLabel(this)), content(new QLabel(this)), time(new QLabel(this)),m_anim_bgOpacity(nullptr)
{
    QPalette palette = this->content->palette();
    palette.setColor(QPalette::WindowText, Qt::gray);
    this->content->setPalette(palette);
    this->time->setAlignment(Qt::AlignRight);
    this->setCursor(Qt::CursorShape::PointingHandCursor);

    m_anim_bgOpacity = new QPropertyAnimation(this, "bgOpacity");
    m_anim_bgOpacity->setParent(this);
    m_anim_bgOpacity->setDuration(200);

    this->setMouseTracking(true);
    this->setFixedHeight(60);
    this->SetContent(from, content, time);
}

void ChatListItem::SetContent(const QString& from, const QString& content, const QDateTime& time) const
{
    QString shortContent = content;
    shortContent = shortContent.remove('\n').left(8);
    this->uid->setText(from);
    this->content->setText(shortContent);
    this->time->setText(time.toLocalTime().toString("HH:mm"));
    this->uid->setGeometry(10,8, 100, 20);
    this->content->setGeometry(10, 32, 100, 20);
    this->time->setGeometry(82, 8, 36, 20);
}

QDateTime ChatListItem::LastTime() const
{
    return QDateTime{};
}

void ChatListItem::Unselect()
{
    if (mb_selected)
    {
        mb_selected = false;
        int opacity = 0;
        if (mb_hoverd) opacity = 64;
        this->Set_anim_bgOpacity(opacity);
    }
}

void ChatListItem::Select()
{
    if (!mb_selected)
    {
        mb_selected = true;
        int opacity = 96;
        if (mb_hoverd) opacity = 128;
        this->Set_anim_bgOpacity(opacity);
        emit Selected();
    }
}

QString ChatListItem::From() const
{
    return uid->text();
}

void ChatListItem::set_bgOpacity(const int opacity)
{
    m_bgOpacity = std::clamp(opacity, 0, 255);
    update();
}

int ChatListItem::get_bgOpacity() const
{
    return m_bgOpacity;
}

QByteArray ChatListItem::get_UID() const
{
    return this->uid->text().toLocal8Bit();
}

void ChatListItem::Set_anim_bgOpacity(const int opacity)
{
    m_anim_bgOpacity->stop();
    m_anim_bgOpacity->setStartValue(m_bgOpacity);
    m_anim_bgOpacity->setEndValue(opacity);
    m_anim_bgOpacity->start();
}

void ChatListItem::resizeEvent(QResizeEvent* event)
{
    this->uid->resize( this->width() - 54, 20);
    this->content->resize( this->width() - 16, 20);
    this->time->setGeometry(this->width() - 48, 8, 36, 20);

    QWidget::resizeEvent(event);
}

void ChatListItem::enterEvent(QEnterEvent* event)
{
    mb_hoverd = true;
    return QWidget::enterEvent(event);
}

void ChatListItem::leaveEvent(QEvent* event)
{
    mb_hoverd = false;
    int opacity = 0;
    if (mb_selected)  opacity = 96;
    this->Set_anim_bgOpacity(opacity);
    QWidget::leaveEvent(event);
}

void ChatListItem::mouseMoveEvent(QMouseEvent* event)
{
    mb_hoverd = true;
    int opacity = 64;
    if (mb_selected) opacity = 128;
    this->Set_anim_bgOpacity(opacity);
    QWidget::mouseMoveEvent(event);
}

void ChatListItem::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->Select();
    }
    else if (event->button() == Qt::RightButton)
    {

    }
    QWidget::mousePressEvent(event);
}

void ChatListItem::paintEvent(QPaintEvent* event)
{
    const auto color = QColor(128,128,128, m_bgOpacity);
    QPainter painter{this};
    Style::PaintBackground(this, color, this->rect(), 0);
    QWidget::paintEvent(event);
}

