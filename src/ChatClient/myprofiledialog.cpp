#include "myprofiledialog.hpp"
#include "ui_myprofiledialog.h"
#include "menuwidget.hpp"

// 管理员要修改的用户
extern QString change_extern_username;
extern QString change_extern_nickname;
extern QString change_extern_password;
// 我的信息
extern QString extern_username;
extern QString extern_nickname;
extern QString extern_password;

extern int extern_id;
extern int my_extern_id;

QString _who;

myProfileDialog::myProfileDialog(QString who, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myProfileDialog)
{
    _who = who;
    ui->setupUi(this);
    createDB();
    InitInfo();
}

myProfileDialog::~myProfileDialog()
{
    delete ui;
}

void myProfileDialog::createDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(db.open()){
        qDebug() << "数据库打开成功";
    }
    else{
        qDebug() << db.lastError().text();
    }
}

void myProfileDialog::InitInfo(){
    // 信息显示
    if(_who != "admin"){
        ui->usernameEdit->setText(extern_username);
        ui->nicknameEdit->setText(extern_nickname);
        ui->passwordEdit->setText(extern_password);
        extern_id = my_extern_id;
    }
    else{
        ui->usernameEdit->setText(change_extern_username);
        ui->nicknameEdit->setText(change_extern_nickname);
        ui->passwordEdit->setText(change_extern_password);
    }
}

// 修改按钮
void myProfileDialog::on_changeButton_clicked()
{
    // 激活
    if(ui->changeButton->text() == "修改")
    {
        ui->changeButton->setText("确定");
        ui->nicknameEdit->setEnabled(true);
        if(_who == "admin")
            ui->usernameEdit->setEnabled(true);
        ui->passwordEdit->setEnabled(true);
    }
    // 激活应用
    else{
        QSqlQuery query;
        QString input_username = ui->usernameEdit->text();
        QString input_nickname = ui->nicknameEdit->text();
        QString input_password = ui->passwordEdit->text();
        QString str = QString("UPDATE users "
                              "SET username='%1', nickname='%2', password='%3' "
                              "WHERE id=%4")
                .arg(input_username).arg(input_nickname)
                .arg(input_password).arg(extern_id);

        query.exec(tr("SELECT id FROM users WHERE username='%1'").arg(input_username));
        query.next();
        qDebug() << "查到的id: " << query.value(0).toInt();
        if(query.value(0).toInt() != extern_id && query.value(0).toInt() != 0){
            QMessageBox msg;
            msg.setText("该用户名已存在!");
            msg.exec();
        }
        else{
            if(query.exec(str)){
                qDebug() << "信息更新成功";
                QMessageBox msg;
                msg.setText("信息修改成功");
                msg.exec();
                this->close();
                if(_who == "admin"){
                    MenuWidget *control = new MenuWidget;
                    control->show();
                }
            }
            else{
                qDebug() << "信息修改失败:" << str;
            }
        }

    }
}

