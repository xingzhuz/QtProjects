#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include "sendfile.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startConnect(unsigned short port, QString ip);
    void sendFile(QString path);    // 发送信号个子线程开始发送文件了
    void closeSocket();             // 发送信号给子线程关闭套接字对象

private slots:
    void on_connectServer_clicked(); // 开始连接按钮的槽函数
    void on_selFile_clicked();      // 选择文件按钮的槽函数
    void on_sendFile_clicked();     // 发送文件按钮的槽函数
    void on_disconnect_clicked();   // 断开连接按钮的槽函数

private:
    Ui::MainWindow *ui;
    QLabel *m_status;          // 任务栏状态
    bool m_connected = false;  // 连接状态标志
    QThread *t;                // 子线程
    SendFile *worker;          // 发送文件的任务对象
};

#endif // MAINWINDOW_H
