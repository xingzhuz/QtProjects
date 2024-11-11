#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建子线程的对象
    Generate* gen = new Generate(this);
    BubbleSort* bubble = new BubbleSort(this);
    QuickSort* quick = new QuickSort(this);

    // 启动生成随机数的子线程
    connect(this, &MainWindow::sendNum, gen, &Generate::recvNum);
    connect(ui->start, &QPushButton::clicked, this, [=]{
        emit sendNum(10000);
        gen->start();
    });

    connect(gen, &Generate::sendArry, bubble, &BubbleSort::recvArry);
    connect(gen, &Generate::sendArry, quick, &QuickSort::recvArry);
    connect(gen, &Generate::sendArry, this, [=](QVector<int>list){
        bubble->start();
        quick->start();
        for(int i = 0; i < list.size(); i++)
        {
            ui->randlist->addItem(QString::number(list.at(i)));
        }
    });

    connect(bubble, &BubbleSort::sendArry, this, [=](QVector<int>list){
        for(int i = 0; i < list.size(); i++)
        {
            ui->bubblelist->addItem(QString::number(list.at(i)));
        }
    });

    connect(quick, &QuickSort::sendArry, this, [=](QVector<int>list){
        for(int i = 0; i < list.size(); i++)
        {
            ui->quicklist->addItem(QString::number(list.at(i)));
        }
    });

    // 释放资源
    // 虽然前面指定了父对象将 QThread 的父对象指定为 MainWindow 的实例会在 MainWindow 析构时自动释放 QThread 对象的内存
    // 但 Qt 不会自动停止仍在运行的线程，导致线程对象被析构时仍处于活动状态
    connect(this, &MainWindow::destroyed, this, [=]{
       gen->quit();
       gen->wait();
       delete gen;

       bubble->quit();
       bubble->wait();
       delete bubble;

       quick->quit();
       quick->wait();
       delete quick;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

