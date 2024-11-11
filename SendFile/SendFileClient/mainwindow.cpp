#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendfile.h"
#include <QMessageBox>
#include <QThread>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_connected(false)  // 初始化连接状态为未连接
{
    ui->setupUi(this);
    setWindowTitle("TCP - 客户端");

    // 初始化状态
    ui->ip->setText("127.0.0.1");
    ui->port->setText("8989");
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->sendFile->setEnabled(false);
    ui->selFile->setEnabled(false);

    // 未连接只能点击连接服务器
    ui->connectServer->setEnabled(true);
    ui->disconnect->setEnabled(false);

    t = new QThread;           // 创建线程对象
    worker = new SendFile;     // 创建任务对象

    // 任务对象移动到子线程
    worker->moveToThread(t);

    // 接收连接服务器信号
    connect(this, &MainWindow::startConnect, worker, &SendFile::connectServer);

    // 接收主线程断开连接信号，然后发送信号给子线程关闭套接字对象
    connect(this, &MainWindow::closeSocket, worker, &SendFile::closeSocket);

    // 接收主线程发送文件的信号
    connect(this, &MainWindow::sendFile, worker, &SendFile::sendFile);

    // 处理子线程发送来的信号
    connect(worker, &SendFile::connectOK, this, [ = ]()
    {
        m_status->setText("连接成功");
        ui->selFile->setEnabled(true);

        // 此时只能点击断开连接
        ui->connectServer->setEnabled(false);
        ui->disconnect->setEnabled(true);
        m_connected = true;
    });

    // 出来服务器断开连接
    connect(worker, &SendFile::gameOver, this, [ = ]()
    {
        m_status->setText("未连接");
        ui->sendFile->setEnabled(false);

        // 未连接只能点击连接服务器
        ui->connectServer->setEnabled(true);
        ui->disconnect->setEnabled(false);
        m_connected = false;  // 重置连接状态
    });

    // 接收子线程发来的进度条信号，更新进度条
    connect(worker, &SendFile::curPercent, ui->progressBar, &QProgressBar::setValue);

    // 处理文件发送完成信号
    connect(worker, &SendFile::sendFinished, this, [ = ]()
    {
        ui->progressBar->setValue(0);  // 重置进度条
        ui->filePath->clear();         // 清空文件路径输入框
        QMessageBox::information(this, "文件发送", "文件发送完毕，可以选择新文件发送。");
        ui->sendFile->setEnabled(false);
    });

    t->start();

    // 状态栏
    m_status = new QLabel;
    m_status->setText("未连接");
    ui->statusbar->addWidget(new QLabel("连接状态: "));
    ui->statusbar->addWidget(m_status);
}

// 连接服务器按钮的槽函数
void MainWindow::on_connectServer_clicked()
{
    if (!m_connected)
    {
        QString ip = ui->ip->text();
        unsigned short port = ui->port->text().toUShort();

        emit startConnect(port, ip);
        m_connected = true;  // 标记为已连接
    }
}

// 选择文件按钮的槽函数
void MainWindow::on_selFile_clicked()
{
    // 获取选择的文件路径
    QString path = QFileDialog::getOpenFileName();

    if (path.isEmpty())
    {
        QMessageBox::warning(this, "打开文件", "选择的文件路径不能为空!");
        return;
    }

    ui->filePath->setText(path);

    // 此时可以点击发送文件
    ui->sendFile->setEnabled(true);
}

// 发送文件按钮的槽函数
void MainWindow::on_sendFile_clicked()
{
    emit sendFile(ui->filePath->text());
}

void MainWindow::on_disconnect_clicked()
{
    emit closeSocket();
    m_connected = false;  // 重置连接状态
    m_status->setText("未连接");

    ui->connectServer->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->selFile->setEnabled(false);
    ui->sendFile->setEnabled(false);
}

MainWindow::~MainWindow()
{
    if (m_connected)
    {
        emit closeSocket();
    }

    t->quit();
    t->wait();

    delete worker;
    delete t;
    delete ui;
}


