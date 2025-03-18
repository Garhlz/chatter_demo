#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

LoginWindow::LoginWindow(ChatClient *client, QWidget *parent) : QWidget(parent), client(client)
{
    auto *layout = new QVBoxLayout(this);         // 垂直布局管理器
    layout->addWidget(new QLabel("Login", this)); // 添加标题标签

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password); // 设置密码输入模式
    layout->addWidget(passwordEdit);

    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    // 连接信号和槽，逻辑不变，样式由 QSS 控制
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::showRegister);
    connect(client, &ChatClient::messageReceived, this, &LoginWindow::handleMessage);

    qDebug() << "LoginWindow created";
    show(); // 显示窗口
}

void LoginWindow::attemptLogin()
{
    QJsonObject msg{
        {"type", "login"},
        {"username", usernameEdit->text()},
        {"password", passwordEdit->text()}};
    client->sendMessage(msg); // 发送登录请求
}

void LoginWindow::handleMessage(const QJsonObject &message)
{
    if (message["type"].toString() == "login")
    {
        if (message["status"].toString() == "success")
        {
            emit loginSuccess(message["nickname"].toString()); // 登录成功
        }
        else
        {
            usernameEdit->clear();
            passwordEdit->clear();
            usernameEdit->setPlaceholderText(message["message"].toString()); // 显示错误信息
        }
    }
}