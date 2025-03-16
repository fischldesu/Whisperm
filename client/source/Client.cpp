#include "Client.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils/AppLog.h"

Client::Client(QObject* parent, const QUrl& proxy) :
    QObject(parent), server_url("wss://server.fischldesu.com/whisperm"), m_state(State::Disconnected),
    m_timeout_counter(0), timeout_time(10)
{
    this->set_ServerUrl(proxy);
    this->Initialize();
}

Client::~Client()
{
    m_ws.close(QWebSocketProtocol::CloseCodeNormal, "ClientApp quit");
}

Client::State Client::get_State() const { return m_state; }

QUrl Client::get_ServerUrl() const { return proxy_url; }

QByteArray Client::get_UID() const
{
    if (m_state == State::Idle || m_state == State::Requesting)
        return m_uid;
    return {};
}

void Client::set_ServerUrl(const QUrl& proxy)
{
    if (!proxy.isValid())proxy_url = server_url;
    else proxy_url = proxy;

    this->Server_CloseWebSocket();
    this->Server_OpenWebSocket();
}

void Client::Initialize()
{
    // WebSocket
    connect(&m_ws, &QWebSocket::connected, this, &Client::Server_onConnect);
    connect(&m_ws, &QWebSocket::disconnected, this, &Client::Server_onDisconnect);
    connect(&m_ws, &QWebSocket::textMessageReceived, this, &Client::Server_onRecviedText);
    connect(&m_ws, &QWebSocket::pong, this, &Client::Server_onPong);

    connect(this, &Client::ClientSignal_Timeout, this, &Client::Server_ConnectionTimeout);
    this->Server_OpenWebSocket();
    // Timer
    this->startTimer(1000);
    m_timeout_counter = timeout_time;
}
void Client::timerEvent(QTimerEvent* event)
{
    this->Server_Ping("a/check");
    m_timeout_counter --;
    if (m_timeout_counter <= 0)
    {
        emit ClientSignal_Timeout();
        Log::Warning("Retrying connect to server.");
        this->Server_CloseWebSocket();
        this->Server_OpenWebSocket();
        m_timeout_counter = this->timeout_time;
    }
    return QObject::timerEvent(event);
}

void Client::set_UID(const QByteArray& uid)
{
    m_uid = uid;
}

void Client::set_State(const State state)
{
    m_state = state;
}

void Client::Server_onConnect()
{
    Log::Info("Connected to" + proxy_url.toString());
    m_state = State::Connected;
    emit ServerSignal_Connected();
}

void Client::Server_onDisconnect()
{
    if (m_state == State::Connecting)
    {
        Log::Warning("Connection Failed:" + proxy_url.toString());
    }
    else if (m_state != State::Disconnected)
    {
        Log::Warning("Disconnected:" + proxy_url.toString());
        m_state = State::Disconnected;
        emit ServerSignal_Disconnected();
    }
}

void Client::Server_Ping(const QByteArray& text) { m_ws.ping(text); }
void Client::Server_OpenWebSocket()
{
    m_state = State::Connecting;
    m_ws.open(proxy_url);
}
void Client::Server_CloseWebSocket()
{
    m_ws.close();
}

void Client::Client_Login(const QString& uid, const QString& pwd)
{
    if (m_state == State::Authenticating)
        return;
    this->set_UID(uid.toLocal8Bit());
    this->set_State(State::Authenticating);
    const auto json = QJsonObject{
        {"type", "login"},
        {"data", QJsonObject{
            {"uid", uid},
            {"pwd", pwd},
            {"pubkey", RSAKeyPair.first}
        }}
    };
    QTimer::singleShot(10000, [this]()
    {
        if (m_state == State::Authenticating)
        {
            const QString response = "登录超时";
            Log::Info(response);
            m_state = State::Connected;
            emit this->ServerSignal_AuthResponse(false, response);
        }
    });
    const auto doc = QJsonDocument(json);
    m_ws.sendTextMessage(doc.toJson());
}

void Client::Client_Logout()
{

}

void Client::Client_ForwardMessage(const Data::Message& message)
{
    const auto& text = Data::RSA::EncryptMessage(message.text, onlineList[message.target.toLocal8Bit()]);
    const auto json = QJsonObject{
        {"type", "msg"},
        {"data", QJsonObject{
            {"msg", text},
            {"from", message.from},
            {"to", message.target},
            {"time", message.time.toSecsSinceEpoch()}
        }}
    };
    const auto doc = QJsonDocument(json);
    m_ws.sendTextMessage(doc.toJson());
}

void Client::Server_SendText(const QString& text) { m_ws.sendTextMessage(text); }

void Client::Server_ConnectionTimeout()
{
    Log::Warning("Connection Timeout.");
    if (m_state == State::Connected)
        this->Server_CloseWebSocket();
}

void Client::Server_onRecviedText(const QString& text)
{
    const auto doc = QJsonDocument::fromJson(text.toUtf8());
    if (doc.isNull())
    {
        Log::Warning("(WSRECV) Invalid JSON.");
        return;
    }

    const auto json = doc.object();

    const auto Recv_Message = [this, json]()
    {
        Data::Message message;
        const auto data = json["data"];
        const auto text = data["msg"].toString();
        const auto time = data["time"].toInt(0);
        const auto from = data["from"].toString();
        const auto to = data["to"].toString();
        message.text = Data::RSA::DecryptMessage(text, RSAKeyPair.second);
        message.time = QDateTime::fromSecsSinceEpoch(time);
        message.from = from;
        message.target = to;
        if (message.target != m_uid)
        {
            Log::Info("(WSRECV)Unkonwn: target:" + message.target);
            message.target = "";
        }

        emit this->ServerSignal_RecviedMessage(message);
    };
    const auto Recv_LoginAuth = [this, json]()
    {
        bool success = false;
        const auto response = json["data"];
        if (response["result"] == "AUTH_OK" && response["info"].toString() == m_uid)
            success = true;
        m_state = success ? State::Idle : State::Connected;
        emit this->ServerSignal_AuthResponse(success, response["result"].toString());
    };

    const auto Recv_OnlineList = [this, json]()
    {
        const auto online_list = json["data"].toArray();
        onlineList.clear();
        for (const QJsonValue &value : online_list) {
            const auto uid = value["uid"].toString().toLocal8Bit();
            const auto pbkey = value["publicKey"].toString().toLocal8Bit();
            if (uid != m_uid)
                onlineList.insert(uid, pbkey);
        }
        emit ServerSignal_OnlineListChanged();
    };

    const auto Recv_Logout = [this, json]()
    {
        const auto reason = json["data"];
        if (reason == "LOGIN_ELSEWHERE")
        {
            emit this->ServerSignal_LogoutResponse("Login elsewhere.");
        }
    };

    switch (m_state)
    {
    case State::Idle:
        {
            if (json["type"] == "msg") return Recv_Message();
            if (json["type"] == "online") return Recv_OnlineList();
            if (json["type"] == "logout") return Recv_Logout();
            break;
        }
    case State::Authenticating:
        {
            if (json["type"] == "login") return Recv_LoginAuth();
            break;
        }
    default:
        break;
    }
}

void Client::Server_onPong(const quint64 elapsedTime, const QByteArray& payload)
{
    m_timeout_counter = this->timeout_time;
    emit ServerSignal_Pong(elapsedTime, payload);
}
