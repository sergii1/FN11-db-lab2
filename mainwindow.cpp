#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <stdio.h>

QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connectionForm = new cls_connectionForm();
    connect(connectionForm->pbtnConnect, SIGNAL(clicked()), this, SLOT(connectToDataBase()));
    //менять сервер и бд здесь
    db.setHostName("195.19.32.74");
    db.setDatabaseName("fn1132_2020");
    db.setPort(5432);
    db.setUserName("student");
    db.setPassword("bmstu");
    if(db.open()){
        ui->setupUi(this);
        connect(ui->Submit, SIGNAL(clicked()), this, SLOT(submit_clicked()));
        connect(ui->listHistory, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(command_clicked(QListWidgetItem*)));
        connect(ui->listTables1, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(table_clicked(QListWidgetItem*)));
        connect(ui->pushLoadHistory, SIGNAL(clicked()), this, SLOT(loadHistory_clicked()));
        //первоначальное заполнение списка таблиц
        for(int i = 0; i < db.tables().size(); ++i)
           ui->listTables1->addItem(db.tables().at(i));
    }
    else
        ui->result->setText(db.lastError().text());
}

MainWindow::~MainWindow()
{
    std::ofstream fout;
    fout.open("History.txt");
    for(int i = 0; i < ui->listHistory->count(); ++i){
        fout << ui->listHistory->item(i)->text().toStdString();
        fout << "\n";
    }
    delete ui;
}

void MainWindow::submit_clicked(){
    if(db.open()){
        //создание объекта запроса
        QSqlQuery myQuery(db);
        //собственно выполнение введенного запроса
            myQuery.exec(ui->textEdit->toPlainText());
            //обработка результата запроса
             if(myQuery.isActive())
                  if(!myQuery.isSelect())
                      ui->result->setText("The command was successful");
                  else {
                      //обновление поля с результатом
                      ui->result->setText("The command was successful");
                      QSqlQueryModel* model = new QSqlQueryModel();
                      model->setQuery(ui->textEdit->toPlainText(), db);
                      ui->tableView->setModel(model);
                  }
             else
                ui->result->setText(myQuery.lastError().text());
             //добавление команды в историю
             ui->listHistory->insertItem(0, ui->textEdit->toPlainText());
             ui->textEdit->clear();
             ui->listTables1->clear();
             for(int i = 0; i < db.tables().size(); ++i)
                ui->listTables1->addItem(db.tables().at(i));
    }
    //ui->textEdit->setText(db.tables().at());

}

void MainWindow::command_clicked(QListWidgetItem* p_Item){
    //если кликнули на команду из истории просто вставляем ее в поле, где вводим запрос
    ui->textEdit->setText(p_Item->text());
}

void MainWindow::table_clicked(QListWidgetItem* p_Table)
{
   // auto model = new QSqlQueryModel();
    //отображение содерщимого таблицы
    QSqlTableModel* model2 = new QSqlTableModel(0, db);
   // model->setQuery("SELECT * FROM " + p_Table->text(), db);
    model2->setTable(p_Table->text());
    model2->setEditStrategy(QSqlTableModel::OnFieldChange);
    model2->select();
    ui->tableView->setModel(model2);
    ui->tableView->show();
}

//открываем файл читаем его и заносим все в историю
void MainWindow::loadHistory_clicked(){
    std::ifstream file;
    file.open("History.txt");
    std::string s;
    while(getline(file, s)){
        ui->listHistory->addItem(QString(s.data()));
    }
}


//показ формы подключения к бд
void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"on_pushButton_clicked";
    connectionForm->show();
}


void MainWindow::connectToDataBase(){
    qDebug()<<"connectToDataBase";
    ui->textEdit->clear();
    ui->listTables1->clear();
    db.setHostName(connectionForm->Host->text());
    db.setDatabaseName(connectionForm->nameDB->text());
    db.setPort(connectionForm->Port->text().toInt());
    db.setUserName(connectionForm->nameUser->text());
    db.setPassword(connectionForm->Password->text());

    if(db.open()){
        //первоначальное заполнение списка таблиц
        for(int i = 0; i < db.tables().size(); ++i)
           ui->listTables1->addItem(db.tables().at(i));
    }
    else
        ui->result->setText(db.lastError().text());

     connectionForm->close();
}
