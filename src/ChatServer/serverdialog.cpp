#include "serverdialog.hpp"
#include "ui_serverdialog.h"

//QString global_onlineNum;
ServerDialog::ServerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerDialog)
{
    ui->setupUi(this);
    // 当客户端向服务器发送连接请求，发送信号:newConnection
    connect(&tcpServer, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
    connect(&tcpServer, SIGNAL(newConnection()),this, SLOT(changeOnlineNum()));
    connect(&timer, SIGNAL(timeout()),this, SLOT(onTimerout()));
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

// 创建服务器按钮对应的槽函数
void ServerDialog::on_createButton_clicked()
{
    if(ui->createButton->text() == "创建服务器")
    {
        // 获取服务器端口
        port = ui->portEdit->text().toShort();
        // 设置服务器IP和端口
        if(tcpServer.listen(QHostAddress::Any,port) == true){
            qDebug() << "创建服务器成功";
            // 禁用按钮和端口
            ui->portEdit->setReadOnly(true);
            // ui->createButton->setEnabled(false);
            ui->createButton->setText("关闭服务器");
            QMessageBox msg;
            msg.setText("服务器开启成功");
            msg.exec();
            // 开启定时器
            timer.start(1000);
        }
        else{
            qDebug() << "创建失败";
        }
    }
    // 关闭服务器操作
    else{
        timer.stop();
        ui->portEdit->setEnabled(true);
        ui->createButton->setText("创建服务器");
        QMessageBox msg;
        msg.setText("服务器关闭成功");
        msg.exec();
        tcpServer.close();
        for(int i=0;i < tcpClientList.size();++i)
            tcpClientList.removeAt(i);
    }
}

// 相应客户端连接请求的槽函数
void ServerDialog::onNewConnection()
{
    // 获取和客户端通信的套接字
    QTcpSocket* tcpClient = tcpServer.nextPendingConnection();
    // 保存套接字到容器
    tcpClientList.append(tcpClient);
    // 当客户端给服务器发送消息时，通信套接字发送信号:readyRead
    connect(tcpClient, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
}

// 接受客户端聊天消息的槽函数
void ServerDialog::onReadyRead()
{
    // 遍历容器哪个客户端给服务器发送了消息
    for(int i = 0;i<tcpClientList.size();++i){
        // bytesAvailable:获取当前套接字等待读取消息字节数
        // 返回0表示没有消息
        // 返回>0，说明当前套接字有消息
        if(tcpClientList.at(i)->bytesAvailable() > 0){
            // 读取消息并保存
            QByteArray buf = tcpClientList.at(i)->readAll();
            // 显示聊天消息
            ui->listWidget->addItem(buf);
            ui->listWidget->scrollToBottom();
            // 转发消息给所有在线客户端
            sendMessage(buf);
        }
    }
}

// 转发聊天消息给其他客户端
void ServerDialog::sendMessage(const QByteArray& buf)
{
    for(int i = 0;i < tcpClientList.size();++i){
        if(tcpClientList.at(i)->write(buf)==-1){
            qDebug() << "message send error";
        }
    }
}

// 定时器到期后将执行的槽函数
void ServerDialog::onTimerout(){
    // 遍历检查容器中保存的客户端通信套接字是否已经断开连接,如果是则删除
    for(int i=0;i < tcpClientList.size();++i){
        if(tcpClientList.at(i)->state() == QAbstractSocket::UnconnectedState){
            tcpClientList.removeAt(i);
            --i;
        }
    }
    changeOnlineNum();
}

void ServerDialog::changeOnlineNum()
{
    QString str = QString::number(tcpClientList.size());
    //global_onlineNum = str;
    ui->onlineLabel->setText(str);
}

