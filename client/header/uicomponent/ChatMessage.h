#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QWidget>

class QTextEdit;


class ChatMessage final : public QWidget{
    Q_OBJECT
public:
    ChatMessage(QWidget* parent, bool self, const QString& text);
private:
    QTextEdit* m_TextEdit;
    QMenu* m_menu;
    bool mb_self;
    QString m_source;

    void RelocateText();

    void resizeEvent(QResizeEvent* event) override;
    // void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
private Q_SLOTS:
    void onContextMenu_Copy();
};


#endif //CHATMESSAGE_H
