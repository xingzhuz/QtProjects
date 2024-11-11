#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>

class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);

    // 连接服务器
    void connectServer(unsigned short port, QString ip);


    // 发送文件
    void sendFile(QString path);

private:
    QTcpSocket *m_tcp;

signals:
    void connectOK();
    void gameOver();
    void curPercent(int num);
    void sendFinished();
    //    void sendTCP(QTcpSocket *tcp);  // 发送给主线程这个 m_tcp

public slots:
    void closeSocket();

};

#endif // SENDFILE_H
