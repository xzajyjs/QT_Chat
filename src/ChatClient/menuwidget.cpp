#include "menuwidget.hpp"
#include "ui_menuwidget.h"
#include "myprofiledialog.hpp"
#include "logindialog.hpp"

extern QString extern_username;
int extern_id;
QString change_extern_username;
QString change_extern_nickname;
QString change_extern_password;

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
{
    ui->setupUi(this);
    createDB();
    queryTable();
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

// 创建数据库
void MenuWidget::createDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(db.open()){
        qDebug() << "数据库打开成功!";
    }
    else{
        qDebug() << db.lastError().text();
    }
}

// 显示所有数据库信息
void MenuWidget::queryTable(){
    QString str = QString("SELECT id as ID,username as 用户名,nickname as 昵称,password as 密码 FROM users");
    model.setQuery(str);
    ui->table->setModel(&model);
}

// 修改
void MenuWidget::on_changeButton_clicked()
{
    extern_id = ui->userIdEdit->text().toInt();
    QSqlQuery query;
    QString str = QString("SELECT id FROM users WHERE id=%1").arg(extern_id);
    query.exec(str);query.next();
    if(query.value(0).toString() == ""){
        qDebug() << query.value(0).toString();
        QMessageBox msg;
        msg.setText("不存在该用户");
        msg.exec();
    }
    else{
        // 修改界面

        QSqlQuery query;

        QString str = QString("SELECT username FROM users WHERE id=%1").arg(extern_id);
        query.exec(str);query.next();
        change_extern_username = query.value(0).toString();

        str = QString("SELECT nickname FROM users WHERE id=%1").arg(extern_id);
        query.exec(str);query.next();
        change_extern_nickname = query.value(0).toString();

        str = QString("SELECT password FROM users WHERE id=%1").arg(extern_id);
        query.exec(str);query.next();
        change_extern_password = query.value(0).toString();

        myProfileDialog *profile = new myProfileDialog("admin");
        profile->show();
        this->close();
    }
}

// 排序
void MenuWidget::on_orderButton_clicked()
{
    // 获得按什么排序
    QString value;
    if(ui->speciesComboBox->currentIndex() == 0)
        value = "id";
    else
        value = "nickname";
    // 获取排序方式
    QString sort;
    if(ui->howComboBox->currentIndex() == 0){
        sort = QString("ASC");
    }
    else{
        sort = QString("DESC");
    }
    QString str = QString("SELECT id as ID,username as 用户名,nickname as 昵称,password as 密码 FROM users ORDER BY %1 %2").arg(value).arg(sort);
    model.setQuery(str);
    ui->table->setModel(&model);
}

// 查找
void MenuWidget::on_searchButton_clicked()
{
    QString search = QString("SELECT id as ID,username as 用户名,nickname as 昵称,password as 密码 FROM users "
                             "WHERE nickname LIKE '%%1%'").arg(ui->nicknameEdit->text());
    qDebug() << search;
    model.setQuery(search);
    ui->table->setModel(&model);
}

// 信息修改
void MenuWidget::on_RenewpushButton_clicked()
{
    ui->nicknameEdit->setText("");
    queryTable();
}

// 删除
void MenuWidget::on_delButton_clicked()
{
    QSqlQuery query;
    int res=QMessageBox::information(this, "退出", "是否要退出?", QMessageBox::Yes | QMessageBox::No);
    if(res==QMessageBox::Yes){
        QString str = QString("DELETE FROM users WHERE id=%1").arg(ui->userIdEdit->text().toInt());
        query.exec(str);
        queryTable();   // 显示刷新
        ui->userIdEdit->clear();// 清空输入框
    }
}

