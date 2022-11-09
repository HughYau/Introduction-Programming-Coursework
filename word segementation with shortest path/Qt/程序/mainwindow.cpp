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

void MainWindow::on_pushButton_clicked()//���а�ť
{
    wg W;
    string in;
    vector<QString>out;
    int num;
    int flag = 0;
    QString st;
    st = ui->plainTextEdit->toPlainText();
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");//��������ַ���UTF-8תgb2312
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

void MainWindow::on_pushButton_2_clicked()//������ť
{
    ui->textBrowser->clear();
}
