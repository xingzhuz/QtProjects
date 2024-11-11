#ifndef RECVFILE_H
#define RECVFILE_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>

class RecvFile : public QThread
{
    Q_OBJECT
public:
    explicit RecvFile(QTcpSocket *tcp, QObject *parent = nullptr);

protected:
    void run() override;

private:
    QTcpSocket *m_tcp;
    QString fileName;  // 文件名
    QFile *file;       // 文件对象
    long long total;   // 文件总大小
    long long count;   // 已接收大小

signals:
    void recvOver(QString fileName);  // 接收文件完毕的信号

public slots:
};

#endif // RECVFILE_H
