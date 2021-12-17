#include "registerdialog.hpp"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    createDB();
    ui->new_username->setFocus();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::createDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(db.open()){
        qDebug() << "数据库打开成功";
    }
    else{
        qDebug() << db.lastError().text();
    }
}

void RegisterDialog::on_cancelButton_clicked()
{
    this->close();
}

void RegisterDialog::on_registerButton_clicked()
{
    QString input_username = ui->new_username->text();
    QString input_first_passwd = ui->newFirst_passwd->text();
    QString input_second_passwd = ui->newSec_passwd->text();
    QString input_nickname = ui->nicknameEdit->text();
    if(input_username == "" || input_nickname == "" || input_first_passwd == ""
            || input_second_passwd == ""){
        QMessageBox msg;
        msg.setText("信息请填写完整!");
        msg.exec();
    }
    else{
        if(input_first_passwd  == input_second_passwd){
            QSqlQuery query;
            // 判断是否有这个账号了
            QString search = QString("SELECT username FROM users "
                                     "WHERE username='%1'").arg(input_username);
            query.exec(search);
            query.next();
            // 找到了同名用户
            if(query.value(0).toString() == input_username){
                QMessageBox msg;
                msg.setText("该用户名已存在!");
                msg.exec();
            }
            else{
                QString str = QString("INSERT INTO users(username, password, nickname) VALUES('%1', '%2', '%3')")
                        .arg(input_username).arg(input_first_passwd)
                        .arg(input_nickname);
                if(query.exec(str)){
                    QMessageBox msg;
                    msg.setText("注册成功");
                    msg.exec();
                    this->close();
                }
                else{
                    qDebug() << str;
                }
            }
        }
        else{
            QMessageBox msg;
            msg.setText("两次密码输入不一致");
            msg.exec();
        }
    }


}

