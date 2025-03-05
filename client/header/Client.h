#ifndef CLIENT_H
#define CLIENT_H

#include <QSystemTrayIcon>
#include <QtWebSockets/QtWebSockets>
#include <QUrl>

#include "tools/Data.h"

#endif // CLIENT_H
class Client final :public QObject
{
    Q_OBJECT
public:
    enum class State
    {
        Disconnected,
        Connecting,
        Connected,
        Authenticating,
        Idle,
        Requesting
    };
    explicit Client(QObject* parent = nullptr, const QUrl& proxy = {});
    ~Client() override;


    [[nodiscard]] bool Is_Online(const QByteArray& uid) const { return onlineList.contains(uid); }
    [[nodiscard]] State get_State() const;
    [[nodiscard]] QUrl get_ServerUrl() const;
    [[nodiscard]] QByteArray get_UID() const;
    [[nodiscard]] QList<QByteArray> get_OnlineList() const { return onlineList.keys(); }
    void set_ServerUrl(const QUrl& proxy = {});

private:
    const QUrl server_url;
    QUrl proxy_url;
    QWebSocket m_ws;
    State m_state;
    QByteArray m_uid;
    QMap<QByteArray, QByteArray> onlineList;
    int m_timeout_counter;
    int timeout_time;

    QPair<QString, QString> RSAKeyPair;

    void Initialize();
    void set_UID(const QByteArray& uid);
    void set_State(State state);

    void timerEvent(QTimerEvent* event) override;

public Q_SLOTS:

    void Server_Ping(const QByteArray &text);
    void Server_OpenWebSocket();
    void Server_CloseWebSocket();

    void Client_Login(const QString& uid, const QString& pwd);
    void Client_Logout();
    void Client_ForwardMessage(const Data::Message& message);
private Q_SLOTS:
    void Server_onConnect();
    void Server_onDisconnect();
    void Server_onRecviedText(const QString& text);
    void Server_onPong(quint64 elapsedTime, const QByteArray& payload);
    void Server_SendText(const QString& text);
    void Server_ConnectionTimeout();
Q_SIGNALS:
    void ServerSignal_Connected();
    void ServerSignal_Disconnected();
    void ServerSignal_Pong(quint64 elapsedTime, const QByteArray& payload);
    void ServerSignal_AuthResponse(bool success, const QString& response);
    void ServerSignal_LogoutResponse(const QString& reason);
    void ServerSignal_OnlineListChanged();
    void ServerSignal_RecviedMessage(const Data::Message& message);
    void ClientSignal_SysTrayTrigger(int signal);
    void ClientSignal_Timeout();
};
