#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <QObject>
#include <QString>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QTableView>
#include <QDebug>
#include <QtSql/QSqlQueryModel>
#include <QStringListModel>
#include "ui_mainwindow.h"
#include "connectionform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    cls_connectionForm* connectionForm;
private slots:
    //обработка нажатия кнопки
    void submit_clicked();
    //нажатие на команду из списка
    void command_clicked(QListWidgetItem*);
    //нажатие на таблицн
    void table_clicked(QListWidgetItem*);
    //загрузка файла истории
    void loadHistory_clicked();
    void on_pushButton_clicked();

    //подключение к бд
    void connectToDataBase();
};
#endif // MAINWINDOW_H
