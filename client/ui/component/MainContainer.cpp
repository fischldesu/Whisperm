#include "uicomponent/MainContainer.h"

#include "tools/AppLog.h"

MainContainer::MainContainer(QWidget* parent):QWidget(parent)
{

}

int MainContainer::GetPageCount() const
{
    int count = 0;
    for (const auto child : this->children())
    {
        if (child->isWidgetType())
            count++;
    }
    return count;
}

void MainContainer::resizeEvent(QResizeEvent* event)
{
    static int pageCount = this->GetPageCount();
    for (int i = 0; i < pageCount; i++)
    {
        if (const auto child = this->children().at(i); child->isWidgetType())
        {
            const auto widget = dynamic_cast<QWidget*>(child);
            widget->setGeometry(0, this->parentWidget()->height() * i, this->width(), this->height());
        }
    }
    QWidget::resizeEvent(event);
}
