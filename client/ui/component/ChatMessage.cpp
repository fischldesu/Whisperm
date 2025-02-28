#include "uicomponent/ChatMessage.h"

#include <QTextEdit>
#include <QTextBlock>

ChatMessage::ChatMessage(QWidget* parent, const bool self, const QString& text)
    :QWidget(parent), m_TextEdit(new QTextEdit(this)), mb_self(self)
{
    this->setMinimumHeight(64);
    m_TextEdit->resize(160, 58);
    m_TextEdit->setText(text);
    m_TextEdit->setStyleSheet("QTextEdit{background-color:#00b2ff;padding:4px;}");
    m_TextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_TextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    m_TextEdit->setReadOnly(true);
    this->RelocateText();
}

void ChatMessage::RelocateText()
{
    auto xpos = 8;
    if (mb_self)
        xpos = this->width() - xpos - m_TextEdit->width();
    m_TextEdit->move(xpos, 8);
}

void ChatMessage::resizeEvent(QResizeEvent* event)
{
    this->RelocateText();
    return QWidget::resizeEvent(event);
}
