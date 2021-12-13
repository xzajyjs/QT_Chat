#include "clientdialog.hpp"
#include "ui_clientdialog.h"
#include "menuwidget.hpp"
#include "logindialog.hpp"
#include "myprofiledialog.hpp"
extern QString extern_nickname;
extern QString extern_username;
//extern QString global_onlineNum;
ClientDialog::ClientDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    status = false; // 离线
    ui->connectButton->setFocus();
    ui->usernameEdit->setText(extern_nickname);
    connect(&tcpsocket, SIGNAL(connected()),this,SLOT(onConnected()));
    connect(&tcpsocket, SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(&tcpsocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError()));
    if(extern_username == "admin")
        ui->myButton->setText("管理");



}

ClientDialog::~ClientDialog()
{
    delete ui;
}

// 发送按钮
void ClientDialog::on_sendButton_clicked()
{
    // 获取用户输入聊天消息
    QString msg = ui->messageEdit->text();
    if(msg==""){
        QMessageBox ms;
        ms.setText("输入消息为空");
        ms.exec();
        return;
    }
    msg = username + "说: " + msg;
    // 发送聊天消息
    tcpsocket.write(msg.toUtf8());
    // 清空输入框
    ui->messageEdit->clear();
}

// 连接服务器按钮
void ClientDialog::on_connectButton_clicked()
{
    if(ui->connectButton->text() == "连接")
        ui->myButton->setEnabled(false);
    else
        ui->myButton->setEnabled(true);

    // 如果是离线状态，则建立和服务器连接
    if(status == false){
        // 获取服务器IP
        serverIP.setAddress(ui->serverIPEdit->text());
        // 获取端口
        serverPort = ui->serverPortEdit->text().toShort();
        // 获取聊天室昵称
        username = extern_nickname;
        // 向服务器发送连接请求
        // 成功发送信号：connected
        // 失败发送信号：error
        tcpsocket.connectToHost(serverIP, serverPort);
    }
    // 如果当前是在线状态，则断开和服务器连接
    else{
        // 向服务器发送离开聊天室的提示消息
        QString msg = username + "离开了聊天室";
        tcpsocket.write(msg.toUtf8());
        // 关闭和服务器的连接,同时发送信号:disconnected
        tcpsocket.disconnectFromHost();
    }
}

// 和服务器连接成功时执行的槽函数
void ClientDialog::onConnected(){
    // 在线状态
    status = true;
    ui->statusRadio->setChecked(true);
    ui->sendButton->setEnabled(true);
    ui->serverIPEdit->setEnabled(false);
    ui->serverPortEdit->setEnabled(false);
    ui->connectButton->setText("断开");
    ui->messageEdit->setFocus();
    // 向服务器发送进入聊天室提示消息
    QString str = username + "进入了聊天室";
    // toUtf8 QString转换为QByteArray
    tcpsocket.write(str.toUtf8());
}

// 和服务器断开连接执行的槽函数
void ClientDialog::onDisconnected(){
    status = false;
    ui->statusRadio->setChecked(false);
    ui->sendButton->setEnabled(false);
    ui->serverIPEdit->setEnabled(true);
    ui->serverPortEdit->setEnabled(true);
    ui->connectButton->setText("连接");
}

// 接收聊天消息的槽函数
void ClientDialog::onReadyRead(){
    if(tcpsocket.bytesAvailable()){
        // 接收消息
        QByteArray buf = tcpsocket.readAll();
        // 显示消息
        ui->listWidget->addItem(buf);
        ui->listWidget->scrollToBottom();
    }
}

// 网络异常执行的槽函数
void ClientDialog::onError(){
    // errorString获取网络异常原因
    QMessageBox::critical(this, "ERROR", "连接失败!\nError log: "+tcpsocket.errorString());
}

// 我的 | 管理
void ClientDialog::on_myButton_clicked()
{
    if(extern_username == "admin"){
        MenuWidget * control = new MenuWidget;
        control->show();
    }
    else{
        myProfileDialog *profile = new myProfileDialog("xx");
        profile->show();
    }
}

// 退出登录
void ClientDialog::on_pushButton_clicked()
{
    this->close();
    LoginDialog *login = new LoginDialog;
    login->show();
}

