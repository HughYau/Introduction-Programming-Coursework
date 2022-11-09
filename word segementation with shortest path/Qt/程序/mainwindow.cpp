#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wg.h"
#include <iostream>
#include <QString>
#include <QTextCodec>
#include <string.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setFixedSize(500,350);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//运行按钮
{
    wg W;
    string in;
    vector<QString>out;
    int num;
    int flag = 0;
    QString st;
    st = ui->plainTextEdit->toPlainText();
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");//将输入的字符从UTF-8转gb2312
    QByteArray arr = pCodec->fromUnicode(st);
    in = arr.data();
    num = ui->spinBox->value();
    if(ui->checkBox->isChecked()){
       flag = 1;
    }
    out = W.Run(in,num,flag);
    for(int i = 0;i < out.size();i++)
    {
        ui->textBrowser->append(out[i]);
    }
    ui->textBrowser->append("\n");
}

void MainWindow::on_pushButton_2_clicked()//清屏按钮
{
    ui->textBrowser->clear();
}
