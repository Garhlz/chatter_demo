#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <QtDebug>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QJsonObject &message);

signals:
    void connected(); // 针对连接的不同状态，发送不同信号
    void disconnected();
    void messageReceived(const QJsonObject &message);
    // 收到服务器消息

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    // 每个信号直接对应一个槽函数

private:
    QTcpSocket *socket; // socket指针作为一个成员字段
};

#endif // CHATCLIENT_H