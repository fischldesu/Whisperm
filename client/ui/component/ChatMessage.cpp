#include "uicomponent/ChatMessage.h"

#include <QTextEdit>
#include <QTextBlock>
#include <QMenu>

ChatMessage::ChatMessage(QWidget* parent, const bool self, const QString& text)
    :QWidget(parent), m_TextEdit(new QTextEdit(this)), m_menu(new QMenu(m_TextEdit)),mb_self(self)
{
    this->setMinimumHeight(64);

    QString color[2] = {"white", "black"};
    if (self)
    {
        color[0] = "#0099ff";
        color[1] = "white";
    }
    const auto textStyle = QString
        {"QTextEdit{background-color:%1;color:%2;padding:4px;border-radius:8px}"}
        .arg(color[0])
        .arg(color[1]);
    m_TextEdit->resize(192, 58);
    m_TextEdit->setText(text);
    m_TextEdit->setStyleSheet(textStyle);
    m_TextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_TextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    m_TextEdit->setReadOnly(true);
    m_TextEdit->installEventFilter(this);
    this->RelocateText();
    const QAction* action_copy = m_menu->addAction("Copy");
    connect(action_copy, &QAction::triggered, this, &ChatMessage::onContextMenu_Copy);
    m_menu->setStyleSheet(
    "QMenu {background-color: #FAFAFC;}"
        "QMenu::item:selected{background-color:gray;}");
}

void ChatMessage::RelocateText()
{
    auto xpos = 8;
    if (mb_self)
        xpos = this->width() - xpos - m_TextEdit->width();
    m_TextEdit->move(xpos, 3);
}

void ChatMessage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    this->RelocateText();
}

bool ChatMessage::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_TextEdit && event->type() == QEvent::MouseButtonPress)
    {
        m_menu->exec(QCursor::pos());
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void ChatMessage::onContextMenu_Copy()
{
    if (m_TextEdit->textCursor().hasSelection()) m_TextEdit->copy();
    else
    {
        m_TextEdit->selectAll();
        m_TextEdit->copy();
    }
}
