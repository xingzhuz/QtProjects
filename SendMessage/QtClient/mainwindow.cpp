#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TCP - 客户端");

    // 初始化
    ui->sendMessage->setEnabled(false);
    ui->connectServer->setEnabled(true);
    ui->disconnect->setEnabled(false);

    // 创建通信的套接字对象
    m_tcp = new QTcpSocket(this);

    // 检测服务器是否回复了数据
    connect(m_tcp, &QTcpSocket::readyRead, [ = ]()
    {
        // 接收服务器发送的数据
        QByteArray recvMsg = m_tcp->readAll();
        ui->record->append("服务器Say: " + recvMsg);
    });

    // 检测是否和服务器是否连接成功了
    connect(m_tcp, &QTcpSocket::connected, this, [ = ]()
    {
        ui->record->append("恭喜, 连接服务器成功!!!");
        m_status->setText("连接");
        ui->connectServer->setEnabled(false);
        ui->disconnect->setEnabled(true);
        ui->sendMessage->setEnabled(true);
    });

    // 检测服务器是否和客户端断开了连接
    connect(m_tcp, &QTcpSocket::disconnected, this, [ = ]()
    {
        ui->record->append("服务器已经断开了连接, ...");
        m_status->setText("未连接");
        ui->connectServer->setEnabled(true);
        ui->disconnect->setEnabled(false);
        ui->sendMessage->setEnabled(false);
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

// 连接服务器按钮按下之后的处理动作
void MainWindow::on_connectServer_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();

    // 连接服务器
    m_tcp->connectToHost(QHostAddress(ip), port);
}

// 发送数据按钮按下之后的处理动作
void MainWindow::on_sendMessage_clicked()
{
    QString sendMsg = ui->msg->toPlainText();
    m_tcp->write(sendMsg.toUtf8());
    ui->record->append("客户端Say: " + sendMsg);
    ui->msg->clear();
}

// 断开连接按钮被按下之后的处理动作
void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    m_status->setText("未连接");
    ui->connectServer->setEnabled(true);
    ui->disconnect->setEnabled(false);
}



