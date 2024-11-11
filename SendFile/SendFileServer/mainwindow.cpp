#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recvfile.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 初始化状态
    ui->setupUi(this);
    ui->port->setText("8989");
    setWindowTitle("TCP - 服务器端");

    // 创建服务器对象
    m_s = new QTcpServer(this);

    // 监听连接信号
    connect(m_s, &QTcpServer::newConnection, this, [ = ]()
    {
        QTcpSocket *tcp = m_s->nextPendingConnection();
        m_status->setText("连接成功");

        // 创建子线程
        RecvFile *subThread = new RecvFile(tcp);

        subThread->start();

        // 接收文件接收完毕的信号
        connect(subThread, &RecvFile::recvOver, this, [ = ](QString fileName)
        {
            qDebug() << fileName << "文件接收完毕";
        });

        // 客户端断开了连接
        connect(tcp, &QTcpSocket::disconnected, this, [ = ]()
        {
            tcp->deleteLater();
            m_status->setText("未连接");
        });
    });

    // 状态栏
    m_status = new QLabel;
    m_status->setText("未连接");
    ui->statusbar->addWidget(new QLabel("连接状态: "));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any, port);
    ui->setListen->setEnabled(false);
}
