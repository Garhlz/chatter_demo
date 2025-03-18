#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QtCore>
#include <QtNetwork>
#include <QJsonObject>
#include <QtConcurrent>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    void startServer(quint16 port);

private slots:
    void handleNewConnection();  // 新连接
    void processClientMessage(); // 处理消息
    void clientDisconnected();

private:
    QTcpServer *tcpServer;               // qt提供的TCP服务器类，监听客户端连接
    QMap<QString, QTcpSocket *> clients; // 用户名 -> 客户端套接字，跟踪已登录客户端
    // 每个用户对应一个tcp连接
    QJsonObject users; // 用户数据，从 JSON 文件加载
    QMutex usersMutex;
    QFuture<void> saveFuture;

    void loadUsers();
    void saveUsersAsync();
    void sendMessageToClient(QTcpSocket *client, const QJsonObject &message);
    void broadcastMessage(const QJsonObject &message, QTcpSocket *sender);
};

#endif