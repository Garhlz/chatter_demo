#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "ChatClient.h"

class QTextEdit;
class QLineEdit;
class QLabel;

class ChatWindow : public QWidget // 继承 QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(ChatClient *client, const QString &nickname, QWidget *parent = nullptr);

private slots:
    void sendMessage();
    void handleMessage(const QJsonObject &message);

private:
    ChatClient *client;      // 服务器通信
    QTextEdit *chatDisplay;  // 只读文本框
    QLineEdit *messageInput; // 输入框
    QString nickname;        // 用户昵称
    QLabel *nicknameLabel;
};

#endif