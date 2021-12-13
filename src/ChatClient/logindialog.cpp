#include "logindialog.hpp"
#include "ui_logindialog.h"
#include "QMessageBox"
#include "registerdialog.hpp"
#include "forgetpasswddialog.hpp"
#include "clientdialog.hpp"

QString extern_nickname;
QString extern_username;
QString extern_password;
int my_extern_id;

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(false);
    createDB();
    createTable();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::createDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(db.open()){
        qDebug() << "数据库打开成功";
        ui->radioButton->setChecked(true);
    }
    else{
        qDebug() << db.lastError().text();
        ui->radioButton->setChecked(false);
    }
}

// id(PK), username(TEXT), password(TEXT), nickname(TEXT)
void LoginDialog::createTable(){
    QSqlQuery query;
    QString str = QString("CREATE TABLE users("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "username TEXT NOT NULL,"
                          "password TEXT NOT NULL,"
                          "nickname TEXT);");
    if(query.exec(str)){
        qDebug() << "数据表创建成功";
    }
    else{
        qDebug() << str;
    }

    // 创建初始管理员用户: admin / 123456
    QString search = QString("SELECT username FROM users "
                             "WHERE username='admin'");
    query.exec(search);query.next();
    // 找到了admin
    if(query.value(0).toString() == "admin"){
        qDebug() << "admin已存在";
    }
    else{
        str = QString("INSERT INTO users(username, password, nickname) VALUES("
                      "'admin', '123456', 'Administrator');");
        if(query.exec(str)){
            qDebug() << "初始数据创建完成";
        }
        else{
            qDebug() << str;
        }
    }

    // 创建初始普通用户: test / 123456
    search = QString("SELECT username FROM users "
                             "WHERE username='test'");
    query.exec(search);query.next();
    // 找到了test
    if(query.value(0).toString() == "test"){
        qDebug() << "test已存在";
    }
    else{
        str = QString("INSERT INTO users(username, password, nickname) VALUES("
                      "'test', '123456', 'testMan');");
    }
    query.exec(str);
}

void LoginDialog::on_loginButton_clicked()
{
    QString input_password = ui->password->text();
    QString input_username = ui->username->text();
    if(input_username == ""){
        QMessageBox msg;
        msg.setText("账号为空");
        msg.exec();
    }
    else if(input_password == ""){
        QMessageBox msg;
        msg.setText("密码为空");
        msg.exec();
    }
    // 账号密码都非空
    else{
        QSqlQuery query;
        QString str = QString("SELECT password FROM users WHERE username='%1'").arg(input_username);
        if(!query.exec(str)){
            qDebug() << str;
        }
        query.next();
        qDebug() << "密码是:" << query.value(0).toString();
        // 账号密码正确
        if(input_password == query.value(0).toString()){
            str = QString("SELECT nickname FROM users WHERE username='%1'").arg(input_username);
            if(query.exec(str)){
                qDebug() << "获取昵称成功";
                query.next();
                extern_nickname = query.value(0).toString();
                extern_username = input_username;
                extern_password = input_password;

                query.exec(tr("SELECT id FROM users WHERE username='%1'").arg(input_username));
                query.next();
                my_extern_id = query.value(0).toInt();
            }
            else
                qDebug() << "昵称获取失败:" << str;
            ClientDialog *client = new ClientDialog;
            this->close();
            client->show();
        }
        // 账号或密码不正确
        else{
            QMessageBox msg;
            msg.setText("账号或密码错误");
            msg.exec();
        }
    }
}
void LoginDialog::on_registerButton_clicked()
{
    RegisterDialog *registerDialog = new RegisterDialog;
    registerDialog->show();
}

void LoginDialog::on_forgetButton_clicked()
{
    forgetPasswdDialog *forget = new forgetPasswdDialog;
    forget->show();
}

