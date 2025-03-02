#include "ui/PageChats.h"
#include "ui_PageChats.h"

#include <QPushButton>
#include <QPainter>
#include <QLabel>
#include "tools/Data.h"
#include "uicomponent/ChatListItem.h"

PageChats::PageChats(QWidget *parent) :
    QWidget(parent), ui(new Ui::PageChats) {

    ui->setupUi(this);

    constexpr QColor bgColor{0, 103, 192};
    constexpr QColor hoverColor{0,73,166};
    const auto UIDLabelConatiner = new QWidget{this};

    m_UIDLabel = new QLabel(UIDLabelConatiner);
    m_UIDLabel->setGeometry(16, 9, 256, 30);
    ui->chatInput->hide();
    ui->chatInput->setFixedHeight(128);
    ui->chatInputDragger->installEventFilter(this);
    ui->widgetLayout->insertWidget(0, UIDLabelConatiner);
    UIDLabelConatiner->setFixedHeight(50);
    UIDLabelConatiner->installEventFilter(this);
    UIDLabelConatiner->hide();

    ui->chatbox_btnSend->set_fgColor(Qt::white);
    ui->chatbox_btnMore->set_fgColor(Qt::white);
    ui->chatbox_btnSend->set_bgColor(bgColor);
    ui->chatbox_btnMore->set_bgColor(bgColor);
    ui->chatbox_btnSend->set_bgHoverColor(hoverColor);
    ui->chatbox_btnMore->set_bgHoverColor(hoverColor);
    ui->chatbox_btnSend->set_bgRadius({8, 0, 8, 0});
    ui->chatbox_btnMore->set_bgRadius({0, 8, 0, 8});

    connect(ui->chatbox_btnSend, &QPushButton::clicked, this, &PageChats::onSendMessage_Clicked);
}

PageChats::~PageChats() {
    delete ui;
}

void PageChats::AppendChatItem(const Data::Message& message)
{
    const auto new_Item = new ChatListItem{ui->chatList, message.from, message.text, message.time};
    ui->verticalLayout->insertWidget(0, new_Item);
    m_itemListContainer.append(new_Item);
    connect(new_Item, &ChatListItem::Selected, [this, new_Item](){ this->SelectChat(new_Item); });
}

void PageChats::SortChatItems()
{
    std::ranges::sort(m_itemListContainer, [](const ChatListItem* a, const ChatListItem* b) {
        return a->LastTime() > b->LastTime();
    });

    for (const auto item : m_itemListContainer)
    {
        const auto spacerIndex = ui->verticalLayout->indexOf(ui->verticalSpacer);
        ui->verticalLayout->insertWidget(spacerIndex, item);
    }
}

void PageChats::SelectChat(const ChatListItem* selected)
{
    for (const auto item: m_itemListContainer)
    {
        if (item == selected)
        {
            const auto uid = item->get_UID();
            QString prefix = "🟥 ";
            if (item->Is_Online() || uid == m_UID)
                prefix = "🟩 ";
            item->Select();
            ui->chatInput->show();
            ui->chatBox->set_UID_Target(uid);
            m_UIDLabel->setText(prefix + uid);
            m_UIDLabel->parentWidget()->show();
        }
        else item->Unselect();
    }
}

void PageChats::ReceivedMessage(const Data::Message& message)
{
    bool alreay_exist_in_ChatList = false;
    for (const auto chatListItem : m_itemListContainer)
        if (chatListItem->From() == message.from)
        {
            alreay_exist_in_ChatList = true;
            chatListItem->SetContent(message.from, message.text, message.time);
            ui->verticalLayout->insertWidget(0, chatListItem);
            break;
        }
    if (!alreay_exist_in_ChatList)
        this->AppendChatItem(message);

    ui->chatBox->AppendMessage(message);
}

void PageChats::set_UID_Client(const QString& uid)
{
    m_UID = uid;
    ui->chatBox->set_UID_Client(uid);
}

void PageChats::UpdateLabel()
{
    for (const auto item : m_itemListContainer)
    {
        if (ui->chatBox->get_UID_Target() == item->get_UID())
        {
            QString prefix = "🟥 ";
            if (item->Is_Online() || ui->chatBox->get_UID_Target() == m_UID)
                prefix = "🟩 ";
            m_UIDLabel->setText(prefix + ui->chatBox->get_UID_Target());
            break;
        }
    }

}

void PageChats::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen;
    const auto xpos = ui->scrollArea->width() + ui->scrollArea->x() -1;
    pen.setWidthF(0.5f);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    painter.drawLine(xpos, 1, xpos, this->height());

    QWidget::paintEvent(event);
}

bool PageChats::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->chatInputDragger && event->type() == QEvent::Type::MouseMove)
    {
        if (const auto mouseEvent = dynamic_cast<QMouseEvent*>(event))
        {
            const auto buttons = mouseEvent->buttons();
            if (buttons & Qt::MouseButton::LeftButton)
            {
                constexpr auto topRange = 128;
                constexpr auto bottomRange = 128;
                const auto value = ui->chatInput->height() - static_cast<int>(mouseEvent->position().y());
                const int h = std::clamp(value, bottomRange, this->height() - topRange);
                ui->chatInput->setFixedHeight(h);
                return true;
            }
        }
    }
    else if (watched == m_UIDLabel->parentWidget() && event->type() == QEvent::Type::Paint)
    {
        const auto widget = m_UIDLabel->parentWidget();
        const auto height = widget->height();
        QPainter painter(widget);
        QPen pen;
        pen.setWidthF(0.5f);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        painter.drawLine(0, height - 1, widget->width(), height - 1);
    }
    return QWidget::eventFilter(watched, event);
}

void PageChats::onSendMessage_Clicked()
{
    if (const auto target = ui->chatBox->get_UID_Target(); !target.isEmpty())
    {
        const auto text = ui->input->toPlainText();
        const Data::Message msg = {
            ui->input->toPlainText(),
            QDateTime::currentDateTime(),
            m_UID,
            target
        };
        ui->input->setPlainText({});
        emit ForwardMessage(msg);
        for (const auto item: m_itemListContainer)
        {
            if (item->get_UID() == target)
            {
                item->SetContent(target, text, QDateTime::currentDateTime());
                break;
            }
        }
        if (target != m_UID)
            ui->chatBox->AppendMessage(msg);
    }
}
