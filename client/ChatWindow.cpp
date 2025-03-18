#include "ChatWindow.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

ChatWindow::ChatWindow(ChatClient *client, const QString &nickname, QWidget *parent)
    : QWidget(parent), client(client), nickname(nickname)
{
    auto *layout = new QVBoxLayout(this); // 垂直布局管理器

    nicknameLabel = new QLabel("Nickname: " + nickname, this);
    nicknameLabel->setObjectName("nicknameLabel");
    layout->addWidget(nicknameLabel);

    chatDisplay = new QTextEdit(this); // 只读聊天框
    chatDisplay->setReadOnly(true);
    layout->addWidget(chatDisplay);

    messageInput = new QLineEdit(this);               // 输入框
    auto *sendButton = new QPushButton("Send", this); // 发送按钮
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);      // 点击发送消息
    connect(client, &ChatClient::messageReceived, this, &ChatWindow::handleMessage); // 处理服务器消息
}

void ChatWindow::sendMessage()
{
    QString text = messageInput->text();
    if (!text.isEmpty())
    {
        QJsonObject msg{
            {"type", "message"},
            {"nickname", nickname},
            {"text", text}};
        client->sendMessage(msg);
        chatDisplay->append(QString("%1: %2").arg(nickname, text)); // 本地显示消息
        messageInput->clear();
    }
}

void ChatWindow::handleMessage(const QJsonObject &message)
{
    if (message["type"].toString() == "message")
    {
        chatDisplay->append(QString("%1: %2").arg(message["nickname"].toString(), message["text"].toString()));
    }
}