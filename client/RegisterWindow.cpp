#include "RegisterWindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>

RegisterWindow::RegisterWindow(ChatClient *client, QWidget *parent)
    : QWidget(parent), client(client)
{
    setWindowTitle("Register");
    resize(300, 200);

    auto *layout = new QVBoxLayout(this);
    auto *titleLabel = new QLabel("Register New User", this);
    titleLabel->setAlignment(Qt::AlignCenter); // 居中对齐
    layout->addWidget(titleLabel);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Enter Username");
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Enter Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    nicknameEdit = new QLineEdit(this);
    nicknameEdit->setPlaceholderText("Enter Nickname");
    layout->addWidget(nicknameEdit);

    registerButton = new QPushButton("Register", this);
    backButton = new QPushButton("Back to Login", this);
    layout->addWidget(registerButton);
    layout->addWidget(backButton);

    layout->addStretch(); // 弹性空间居中

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::attemptRegister);
    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::hide);
    connect(client, &ChatClient::messageReceived, this, &RegisterWindow::handleMessage);
}

void RegisterWindow::attemptRegister()
{
    if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty() || nicknameEdit->text().isEmpty())
    {
        usernameEdit->setPlaceholderText("All fields are required!");
        return;
    }

    QJsonObject msg{
        {"type", "register"},
        {"username", usernameEdit->text()},
        {"password", passwordEdit->text()},
        {"nickname", nicknameEdit->text()}};
    client->sendMessage(msg);
    registerButton->setEnabled(false); // 防止重复点击
}

void RegisterWindow::handleMessage(const QJsonObject &message)
{
    if (message["type"].toString() == "register")
    {
        if (message["status"].toString() == "success")
        {
            usernameEdit->clear();
            passwordEdit->clear();
            nicknameEdit->clear();
            registerButton->setEnabled(true);
            emit registerSuccess();
            hide();
        }
        else
        {
            usernameEdit->clear();
            passwordEdit->clear();
            nicknameEdit->clear();
            usernameEdit->setPlaceholderText(message["message"].toString());
            registerButton->setEnabled(true);
        }
    }
}