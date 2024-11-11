#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 创建子线程的对象
    QThread *t1 = new QThread(this);
    QThread *t2 = new QThread(this);
    QThread *t3 = new QThread(this);

    // 2. 创建任务类对象
    Generate *gen = new Generate;
    BubbleSort *bubble = new BubbleSort;
    QuickSort *quick = new QuickSort;

    // 3. 将任务对象移动到某个子线程中
    gen->moveToThread(t1);
    bubble->moveToThread(t2);
    quick->moveToThread(t3);

    // 启动生成随机数子线程
    connect(this, &MainWindow::sendNum, gen, &Generate::working);
    connect(ui->start, &QPushButton::clicked, this, [=]{
        emit sendNum(10000);
        t1->start();
    });

    // 这个是对应的排序收到信号后，就在移动到的线程中执行动作
    connect(gen, &Generate::sendArry, bubble, &BubbleSort::working);
    connect(gen, &Generate::sendArry, quick, &QuickSort::working);

    // 启动两个排序子线程
    connect(gen, &Generate::sendArry, this, [=](QVector<int>list){
        t2->start();
        t3->start();
        for(int i = 0; i < list.size(); i++)
        {
            ui->randlist->addItem(QString::number(list.at(i)));
        }
    });

    // 接收来自排序的信号，也就是排好序的数组
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
       t1->quit();
       t1->wait();
       delete t1;

       t2->quit();
       t2->wait();
       delete t2;

       t3->quit();
       t3->wait();
       delete t3;

       delete gen;
       delete bubble;
       delete quick;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

