#include "ChatClient.h"

ChatClient::ChatClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead); // 似乎是第二次握手的内容
    // 信号槽连接定义在类构造函数中
    // 信号是一种特殊的声明的函数类型，emit发送
    // 槽是私有的反应函数
    // 信号和槽的类型相同
}

void ChatClient::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "Attempting to connect to" << host << "on port" << port;
    socket->connectToHost(host, port); // 发起TCP连接
}

void ChatClient::sendMessage(const QJsonObject &message)
{
    QJsonDocument doc(message);
    socket->write(doc.toJson()); // 直接调用socket的写即可，向服务器发送数据
}

// 以下都是槽函数，每个对应一个信号
void ChatClient::onConnected()
{
    qDebug() << "Connected to server!";
    emit connected();
}

void ChatClient::onDisconnected()
{
    emit disconnected();
}

void ChatClient::onReadyRead()
{
    QByteArray data = socket->readAll(); // 非阻塞读取所有可用数据
    QJsonDocument doc = QJsonDocument::fromJson(data);
    emit messageReceived(doc.object()); // 通过信号槽机制将消息转发给给ChatWindow, 展示所有服务器广播记录
}