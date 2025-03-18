#include "ChatServer.h"
#include <QJsonDocument>
#include <QFile>
// 新增：引入 QtConcurrent 模块以支持异步任务

ChatServer::ChatServer(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this))
{
    // 连接 QTcpServer 的 newConnection 信号到 handleNewConnection 槽
    // 当有新客户端连接时触发
    connect(tcpServer, &QTcpServer::newConnection, this, &ChatServer::handleNewConnection);
    loadUsers(); // 启动时加载用户数据
}

void ChatServer::startServer(quint16 port)
{
    // 监听所有网络接口的指定端口
    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server failed to start:" << tcpServer->errorString();
        return;
    }
    qDebug() << "Server started on port" << port; // 输出服务器启动成功的调试信息
}

void ChatServer::handleNewConnection()
{
    // 获取新连接的客户端 socket
    QTcpSocket *client = tcpServer->nextPendingConnection();

    // 绑定 readyRead 信号到 processClientMessage 槽，处理客户端消息
    connect(client, &QTcpSocket::readyRead, this, &ChatServer::processClientMessage);
    // 绑定 disconnected 信号到 clientDisconnected 槽，处理客户端断开

    connect(client, &QTcpSocket::disconnected, this, &ChatServer::clientDisconnected);

    qDebug() << "New connection from" << client->peerAddress().toString(); // 记录客户端 IP
}

void ChatServer::processClientMessage()
{
    // 从信号发送者中获取客户端 socket，使用 qobject_cast 确保类型安全
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_6_0); // 设置数据流版本

    // 循环读取客户端发送的所有可用数据
    while (client->bytesAvailable())
    {
        QByteArray data = client->readAll();               // 非阻塞读取数据
        QJsonDocument doc = QJsonDocument::fromJson(data); // 解析为 JSON 文档
        QJsonObject msg = doc.object();                    // 获取 JSON 对象

        QString type = msg["type"].toString(); // 提取消息类型
        if (type == "register")
        {
            // 解析注册请求中的用户名、密码和昵称
            QString username = msg["username"].toString();
            QString password = msg["password"].toString();
            QString nickname = msg["nickname"].toString();

            if (users.contains(username))
            {
                // 用户名已存在，返回错误信息
                sendMessageToClient(client, {{"type", "register"}, {"status", "error"}, {"message", "Username exists"}});
            }
            else
            {
                // 添加新用户信息到 users 对象
                users[username] = QJsonObject{{"password", password}, {"nickname", nickname}};
                saveUsersAsync(); // 异步保存用户信息，不阻塞主线程
                // 立即通知客户端注册成功，无需等待文件操作完成
                sendMessageToClient(client, {{"type", "register"}, {"status", "success"}});
            }
        }
        else if (type == "login")
        {
            // 解析登录请求中的用户名和密码
            QString username = msg["username"].toString();
            QString password = msg["password"].toString();

            if (clients.contains(username))
            {
                // 用户已登录，返回错误信息
                sendMessageToClient(client, {{"type", "login"}, {"status", "error"}, {"message", "Already logged in"}});
            }
            else if (users.contains(username) && users[username].toObject()["password"].toString() == password)
            {
                // 登录验证成功，将客户端加入在线列表
                clients[username] = client;
                sendMessageToClient(client, {{"type", "login"}, {"status", "success"}, {"nickname", users[username].toObject()["nickname"].toString()}});
            }
            else
            {
                // 登录失败，返回错误信息
                sendMessageToClient(client, {{"type", "login"}, {"status", "error"}, {"message", "Invalid credentials"}});
            }
        }
        else if (type == "message")
        {
            // 处理聊天消息，广播给其他客户端
            broadcastMessage(msg, client);
        }
    }
}

void ChatServer::clientDisconnected()
{
    // 获取断开连接的客户端 socket
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString username = clients.key(client); // 查找对应的用户名
    if (!username.isEmpty())
    {
        clients.remove(username);               // 从在线客户端列表中移除
        qDebug() << username << "disconnected"; // 记录断开信息
    }
    client->deleteLater(); // 延迟删除 socket 对象，交给事件循环处理
}

void ChatServer::loadUsers()
{
    // 从 users.json 加载用户数据
    QFile file("users.json");
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        users = doc.object(); // 将文件内容赋值给 users
        file.close();
    }
}

void ChatServer::saveUsersAsync()
{
    // 异步保存用户数据到文件，使用 QtConcurrent::run在线程池中执行
    saveFuture = QtConcurrent::run([this]()
                                   {
        QMutexLocker locker(&usersMutex);
        QFile file("users.json");
        if (file.open(QIODevice::WriteOnly))
        {
            QJsonDocument doc(users); // 将 users 对象序列化为 JSON
            file.write(doc.toJson()); // 写入文件
            file.close();
            qDebug() << "Users saved asynchronously in thread:" << QThread::currentThreadId(); // 记录保存完成
        } });
}

void ChatServer::sendMessageToClient(QTcpSocket *client, const QJsonObject &message)
{
    // 将JSON对象转为字节数组并发送给客户端
    QJsonDocument doc(message);
    client->write(doc.toJson());
}

void ChatServer::broadcastMessage(const QJsonObject &message, QTcpSocket *sender)
{
    // 将消息广播给除发送者外的所有在线客户端
    QJsonDocument doc(message);
    QByteArray data = doc.toJson();
    for (QTcpSocket *client : clients.values())
    {
        if (client != sender)
        {
            client->write(data); // 异步发送消息
        }
    }
}