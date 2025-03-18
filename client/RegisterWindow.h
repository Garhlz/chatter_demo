#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include "ChatClient.h" // 包含 ChatClient 以与服务器通信

class QLineEdit;
class QPushButton;

class RegisterWindow : public QWidget
{
Q_OBJECT // Qt 的宏，启用信号和槽机制

    public : explicit RegisterWindow(ChatClient *client, QWidget *parent = nullptr);
    ~RegisterWindow() override = default;

signals:
    void registerSuccess(); // 注册成功时发出信号，通知主程序切换到登录窗口

private slots:
    void attemptRegister();                         // 点击注册按钮时尝试注册
    void handleMessage(const QJsonObject &message); // 处理从服务器接收到的消息

private:
    ChatClient *client;          // 指向 ChatClient 的指针，用于发送注册请求
    QLineEdit *usernameEdit;     // 用户名输入框
    QLineEdit *passwordEdit;     // 密码输入框
    QLineEdit *nicknameEdit;     // 昵称输入框
    QPushButton *registerButton; // 注册按钮
    QPushButton *backButton;     // 返回登录页面的按钮
};

#endif // REGISTERWINDOW_H