#include <QApplication>
#include "ChatClient.h"
#include "LoginWindow.h"
#include "RegisterWindow.h"
#include "ChatWindow.h"
#include <QtDebug>
#include <QFile> // 读取QSS文件
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 加载QSS文件以美化界面
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet); // 应用全局样式
        styleFile.close();
        qDebug() << "Stylesheet loaded successfully";
    }
    else
    {
        qDebug() << "Failed to load stylesheet";
    }

    qDebug() << "Client Start";

    // 命令行参数解析器
    QCommandLineParser parser;
    parser.setApplicationDescription("Chat Client");
    parser.addHelpOption();                                                                          // 添加支持
    QCommandLineOption portOption("port", "Specify the server port to connect to", "port", "12345"); // 默认端口 12345
    parser.addOption(portOption);
    parser.process(app); // 解析命令行参数

    // 获取用户指定的端口号
    bool ok;
    quint16 port = parser.value(portOption).toUInt(&ok);
    if (!ok || port == 0)
    {
        qDebug() << "Invalid port specified, using default 12345";
        port = 12345; // 无效输入时使用默认值
    }
    qDebug() << "Client starting with port:" << port;
    ChatClient client;
    client.connectToServer("localhost", port);

    LoginWindow login(&client);
    RegisterWindow registerWindow(&client);
    ChatWindow *chat = nullptr;
    // 用lambda表达式结合信号槽，很方便
    QObject::connect(&client, &ChatClient::connected, [&login]()
                     {
                         login.show(); // 客户端连接成功后显示登录窗口
                     });
    QObject::connect(&login, &LoginWindow::showRegister, [&registerWindow]()
                     {
                         registerWindow.show(); // 点击注册按钮显示注册窗口
                     });
    QObject::connect(&login, &LoginWindow::loginSuccess, [&](const QString &nickname)
                     {
        chat = new ChatWindow(&client, nickname);  // 登录成功后创建聊天窗口
        chat->show();
        login.hide(); });
    QObject::connect(&registerWindow, &RegisterWindow::registerSuccess, [&login, &registerWindow]()
                     {
        login.show();  // 注册成功后返回登录窗口
        registerWindow.hide(); });

    return app.exec();
}