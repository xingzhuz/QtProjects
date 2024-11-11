#include "sendfile.h"

#include <QHostAddress>
#include <QFile>
#include <QDebug>
#include <QFileInfo>

SendFile::SendFile(QObject *parent) : QObject(parent)
{

}

void SendFile::connectServer(unsigned short port, QString ip)
{
    // 连接服务器
    m_tcp = new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip), port);

    // 检测是否连接成功，成功发送信号给主线程
    connect(m_tcp, &QTcpSocket::connected, this, &SendFile::connectOK);

    // 检测服务器是否断开连接
    connect(m_tcp, &QTcpSocket::disconnected, this, [ = ]()
    {
        // 客户端也断开连接
        m_tcp->close();
        m_tcp->deleteLater(); // 销毁 m_tcp 对象

        qDebug() << "服务器断开";

        // 再次发送给主线程信号，表示需要客户端断开连接
        emit gameOver();
    });
}

// 发送文件的任务函数
void SendFile::sendFile(QString path)
{
    QFile file(path);
    QFileInfo info(path);

    // 首先发送文件名
    m_tcp->write(info.fileName().toUtf8() + "\n");

    // 获取文件大小
    long long fileSize = info.size();

    if (!file.open(QFile::ReadOnly))
    {
        qDebug() << "打开文件失败";
        return;
    }

    // 发送文件大小
    m_tcp->write((char*)&fileSize, sizeof(fileSize));

    long long totalSent = 0;     // 用于跟踪已发送字节数
    const int bufferSize = 4096; // 定义缓冲区大小
    char buffer[bufferSize];     // 创建一个缓冲区

    while (true)
    {
        qint64 bytesRead = file.read(buffer, bufferSize); // 读取数据到缓冲区

        if (bytesRead <= 0) // 如果没有读取到数据，结束循环
            break;

        // 发送数据给服务器
        m_tcp->write(buffer, bytesRead); // 发送实际读取的字节数
        totalSent += bytesRead;  // 更新已发送字节数

        // 等待数据写入完成
        m_tcp->waitForBytesWritten();

        // 计算并发送给主线程百分比
        int percent = static_cast<int>((totalSent * 100) / fileSize);
        emit curPercent(percent);
    }

    file.close();
    emit sendFinished();

}

// 关闭套接字对象
void SendFile::closeSocket()
{
    if (m_tcp)
    {
        m_tcp->close(); // 关闭连接
    }
}

