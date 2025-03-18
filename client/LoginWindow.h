#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "ChatClient.h"

class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(ChatClient *client, QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &nickname);
    void showRegister(); // 切换到注册窗口

private slots:
    void attemptLogin();                            // 尝试登录
    void handleMessage(const QJsonObject &message); // 处理服务器响应

private:                // 都是指针
    ChatClient *client; // 当前client实例传入作为私有字段
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit; // 输入框
    QPushButton *loginButton;
    QPushButton *registerButton; // 按钮
};

#endif // LOGINWINDOW_H