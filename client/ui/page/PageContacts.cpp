#include "ui/PageContacts.h"
#include "ui_PageContacts.h"

#include <QListWidgetItem>

#include "ui/LoginDialog.h"

PageContacts::PageContacts(QWidget *parent) :
    QWidget(parent), ui(new Ui::PageContacts) {
    ui->setupUi(this);
    connect(ui->list, &QListWidget::itemDoubleClicked, [this](const QListWidgetItem* item) {
        emit DouleClicked(item->whatsThis());
    });
}

PageContacts::~PageContacts() {
    delete ui;
}

void PageContacts::UpdateList(const QList<QByteArray>& list)
{
    ui->list->clear();
    for (const auto& name : list)
    {
        const auto item = new QListWidgetItem(ui->list);
        item->setText("🟩 "+ name);
        item->setWhatsThis(name);
        ui->list->addItem(item);
    }
}
