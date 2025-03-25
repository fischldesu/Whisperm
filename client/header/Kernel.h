#ifndef KERNEL_H
#define KERNEL_H

#include <QtWebSockets/QtWebSockets>

class Kernel : public QObject{
    Q_OBJECT
public:
    enum WebSocketState
    {
        Open,
        Close,
        Opening,
        Closing
    };
    explicit Kernel(QObject *parent = nullptr);
    ~Kernel() override;

    bool SendText(const QString& data);
    bool SendBinary(const QByteArray& data);


    [[nodiscard]]
    QUrl get_WebSocketURL() const { return m_WebSocketURL; }
    bool set_WebSocketURL(const QUrl& url);
    [[nodiscard]]
    WebSocketState get_WebSocketState() const { return m_State; }
    [[nodiscard]]
    int get_WebSocket_TimeoutTime() const {return m_Timer_TimeoutSec; }
    bool set_WebSocket_TimeoutTime(const int seconds)
    { if (seconds > 0 && seconds < 60) { m_Timer_TimeoutSec = seconds; return true; } return false;  }
public Q_SLOTS:
    QFuture<void> WebSocket_Open();
    void WebSocket_Close();
    void WebSocket_Ping(const QByteArray& payload);

Q_SIGNALS:
    void WebSocketStateChanged(WebSocketState);
    void WebSocketReceivedText(QString);
    void WebSocketReceivedBinary(QByteArray);
    void WebSocketPong(quint64 elapsedTime, const QByteArray &payload);
    void WebSocketOpenTimeout();
private:
    QWebSocket m_WS;
    WebSocketState m_State = Close;
    QUrl  m_WebSocketURL;

    int m_Timer_Counter = 0;
    int m_Timer_TimeoutSec = 16;

    void timerEvent(QTimerEvent* event) override;
private Q_SLOTS:
    void On_WebSocket_Open();
    void On_WebSocket_Close();
    void On_WebSocket_Pong(quint64 elapsedTime, const QByteArray &payload);
    void On_WebSocket_RecivedText(const QString& data);
    void On_WebSocket_RecivedBinary(const QByteArray& data);
};


#endif //KERNEL_H
