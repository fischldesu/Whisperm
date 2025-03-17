#ifndef PAGESETTINGS_H
#define PAGESETTINGS_H

#include <QWidget>
class Panel;

QT_BEGIN_NAMESPACE
namespace Ui { class PageSettings; }
QT_END_NAMESPACE

class PageSettings : public QWidget {
Q_OBJECT

public:
    explicit PageSettings(QWidget *parent = nullptr);
    ~PageSettings() override;

    void InitialzieComponents();
private:
    Ui::PageSettings *ui;

    void AddComponent(Panel* component);
};

#endif //PAGESETTINGS_H
