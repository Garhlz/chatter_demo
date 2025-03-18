#include "ChatServer.h"
#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 和客户端完全相同的逻辑，通过参数动态获取端口号
    QCommandLineParser parser;
    parser.setApplicationDescription("Chat Server");
    parser.addHelpOption();                                                                  // 添加 -h 或 --help 支持
    QCommandLineOption portOption("port", "Specify the port to listen on", "port", "12345"); // 默认端口 12345
    parser.addOption(portOption);
    parser.process(app); // 解析命令行参数

    // 用户指定的端口号
    bool ok;
    quint16 port = parser.value(portOption).toUInt(&ok);
    if (!ok || port == 0)
    {
        qDebug() << "Invalid port specified, using default 12345";
        port = 12345;
    }

    qDebug() << "Server starting on port:" << port;
    ChatServer server;
    server.startServer(port);

    return app.exec();
}