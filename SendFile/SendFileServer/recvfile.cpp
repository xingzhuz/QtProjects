#include "recvfile.h"
#include <QCoreApplication>
#include <QFile>

RecvFile::RecvFile(QTcpSocket *tcp, QObject *parent) : QThread(parent)
{
    m_tcp = tcp;
}

void RecvFile::run()
{
    // 获取当前目录下的 resources 子目录路径
    QString directoryPath = "../resources";

    QString fileName;
    long long total = 0;
    int count = 0;
    QFile *file = nullptr;

    // 接收数据
    connect(m_tcp, &QTcpSocket::readyRead, this, [ =, &fileName, &total, &count, &file]()
    {
        if (fileName.isEmpty()) // 第一次进入接收文件名
        {
            QByteArray nameData = m_tcp->readLine();
            fileName = QString(nameData).trimmed();

            // 拼接指定目录和文件名，形成完整路径
            QString fullPath = directoryPath + "/" + fileName;

            // 创建文件对象
            file = new QFile(fullPath);

            if (!file->open(QFile::WriteOnly))
            {
                qDebug() << "文件无法打开:" << fullPath;
                delete file;
                file = nullptr;
                m_tcp->close();
                m_tcp->deleteLater();
                emit recvOver("文件无法打开");
                return;
            }

            // 接收文件大小
            m_tcp->read(reinterpret_cast<char*>(&total), sizeof(total));
            count = 0;
        }

        qint64 bufferSize = 4096;  // 每次读取的缓冲区大小，可以根据需要调整

        // 接收数据
        QByteArray buffer;
        qint64 bytesRead;

        // m_tcp->bytesAvailable() 是 Qt 网络库中的 QTcpSocket 类的一个函数，返回当前网络缓冲区中可供读取的字节数
        while ((bytesRead = m_tcp->bytesAvailable()) > 0)
        {
            buffer = m_tcp->read(qMin(bufferSize, bytesRead));  // 读入指定大小的数据
            count += buffer.size();   // 更新接收的总字节数
            file->write(buffer);      // 写入文件
            file->flush();            // 强制写入磁盘
        }

        // 判断是否接收完毕
        if (count >= total)
        {
            file->close();
            file->deleteLater();
            emit recvOver(fileName);
            fileName.clear();  // 准备接收下一个文件
        }
    });

    // 进入事件循环
    // 在后台持续检测，判断是否客户端还需要进行发送文件
    exec();
}

