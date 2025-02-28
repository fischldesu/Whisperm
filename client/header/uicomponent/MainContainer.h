#ifndef MAINCONTAINER_H
#define MAINCONTAINER_H

#include <QWidget>

class MainContainer: public QWidget{
    Q_OBJECT
public:
    explicit MainContainer(QWidget* parent);
    [[nodiscard]] int GetPageCount() const;
private:
    void resizeEvent(QResizeEvent* event) override;
};



#endif //MAINCONTAINER_H
