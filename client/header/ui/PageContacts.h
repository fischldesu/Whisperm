#ifndef PAGECONTACTS_H
#define PAGECONTACTS_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class PageContacts; }
QT_END_NAMESPACE

class PageContacts : public QWidget {
Q_OBJECT

public:
    explicit PageContacts(QWidget *parent = nullptr);
    ~PageContacts() override;
    void UpdateList(const QList<QByteArray>& list);
Q_SIGNALS:
    void DouleClicked(const QString& name);
private:
    Ui::PageContacts *ui;
};


#endif //PAGECONTACTS_H
