#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TCP - 服务器");
    ui->sendMessage->setEnabled(false);

    // 创建 QTcpServer 对象
    m_server = new QTcpServer(this);

    // 检测是否有新的客户端连接
    connect(m_server, &QTcpServer::newConnection, this, [ = ]()
    {
        // 进行连接
        m_tcp = m_server->nextPendingConnection();
        ui->record->append("成功和客户端建立了新的连接...");
        ui->sendMessage->setEnabled(true);

        // 更新状态栏
        m_status->setText("连接");

        // 检测是否有客户端数据
        connect(m_tcp, &QTcpSocket::readyRead, this, [ = ]()
        {
            // 接收数据
            QString recvMsg = m_tcp->readAll();
            ui->record->append("客户端Say: " + recvMsg);
        });

        // 客户端断开了连接
        connect(m_tcp, &QTcpSocket::disconnected, this, [ = ]()
        {
            ui->record->append("客户端已经断开了连接...");
            m_tcp->deleteLater();
            m_status->setText("未连接");
            ui->sendMessage->setEnabled(false);
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

// 启动服务器端的服务按钮
void MainWindow::on_startServer_clicked()
{
    unsigned short port = ui->port->text().toUShort();

    // 设置服务器监听
    m_server->listen(QHostAddress::Any, port);
    ui->startServer->setEnabled(false);
}

// 点击发送数据按钮
void MainWindow::on_sendMessage_clicked()
{
    QString sendMsg = ui->msg->toPlainText();
    m_tcp->write(sendMsg.toUtf8());
    ui->record->append("服务器Say: " + sendMsg);
    ui->msg->clear();
}
