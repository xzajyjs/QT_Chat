#ifndef MENUWIDGET_HPP
#define MENUWIDGET_HPP

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWidget; }
QT_END_NAMESPACE

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
    void createDB();
    void queryTable();      // 显示所有商品数据

private slots:

    void on_orderButton_clicked();

    void on_searchButton_clicked();

    void on_RenewpushButton_clicked();

    void on_changeButton_clicked();

private:
    Ui::MenuWidget *ui;
    QSqlDatabase db;
    QSqlQueryModel model;
};
#endif // MENUWIDGET_HPP
