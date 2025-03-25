#include "Kernel.h"

Kernel::Kernel(QObject* parent)
    :QObject(parent)
{
    connect(&m_WS, &QWebSocket::connected, this, &On_WebSocket_Open);
    connect(&m_WS, &QWebSocket::disconnected, this, &On_WebSocket_Close);
    connect(&m_WS, &QWebSocket::pong, this, &On_WebSocket_Pong);
    connect(&m_WS, &QWebSocket::textMessageReceived, this, &On_WebSocket_RecivedText);
    connect(&m_WS, &QWebSocket::binaryMessageReceived, this, &On_WebSocket_RecivedBinary);

    m_Timer_Counter = 0;
    this->startTimer(1000);

}

Kernel::~Kernel()
{

}

bool Kernel::set_WebSocketURL(const QUrl& url)
{
    WebSocket_Close();
    if (const auto protocol = url.scheme(); url.isValid() && (protocol == "ws" || protocol == "wss"))
    {
        m_WebSocketURL = url;
        return true;
    }
    return false;
}

bool Kernel::SendText(const QString& data)
{
    if (const auto sentLength = m_WS.sendTextMessage(data); sentLength == data.length() && m_State == Open)
        return true;
    return false;
}

bool Kernel::SendBinary(const QByteArray& data)
{
    if (const auto sentLength = m_WS.sendBinaryMessage(data); sentLength == data.length()  && m_State == Open)
        return true;
    return false;
}

QFuture<void> Kernel::WebSocket_Open()
{
    QFutureInterface<void> futureInterface;
    if (m_State == Close || m_State == Closing)
    {
        m_State = Opening;
        emit WebSocketStateChanged(m_State);
        connect(this, &Kernel::WebSocketStateChanged,
            [futureInterface](const WebSocketState state) mutable {
                if (state == Open)
                    futureInterface.reportFinished();
            });
        connect(this, &Kernel::WebSocketOpenTimeout,
            [futureInterface] mutable{
            futureInterface.reportFinished();
        });
        m_WS.open(m_WebSocketURL);
    } else {
        futureInterface.reportFinished();
    }
    return futureInterface.future();
}

void Kernel::WebSocket_Close()
{
    if (m_State == Open || m_State == Opening)
    {
        m_State = Closing;
        emit WebSocketStateChanged(m_State);
        m_WS.close();
    }
}

void Kernel::WebSocket_Ping(const QByteArray& payload)
{
    if (m_State == Open)
        m_WS.ping(payload);
}

void Kernel::timerEvent(QTimerEvent* event)
{
    if (m_State == Opening)
    {
        m_Timer_Counter++;
        if (m_Timer_Counter >= m_Timer_TimeoutSec)
        {
            m_Timer_Counter = 0;
            WebSocket_Close();
            emit WebSocketOpenTimeout();
        }
    }
}

void Kernel::On_WebSocket_Open()
{
    m_State = Open;
    emit WebSocketStateChanged(m_State);
}

void Kernel::On_WebSocket_Close()
{
    m_State = Close;
    emit WebSocketStateChanged(m_State);
}

void Kernel::On_WebSocket_Pong(const quint64 elapsedTime, const QByteArray& payload)
{
    emit WebSocketPong(elapsedTime, payload);
}

void Kernel::On_WebSocket_RecivedText(const QString& data)
{
    emit WebSocketReceivedText(data);
}

void Kernel::On_WebSocket_RecivedBinary(const QByteArray& data)
{
    emit WebSocketReceivedBinary(data);
}


