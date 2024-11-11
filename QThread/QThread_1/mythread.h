#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QVector>


// 生成随机数
class Generate : public QThread
{
    Q_OBJECT
public:
    explicit Generate(QObject *parent = nullptr);

    void recvNum(int num);

protected:
    void run() override;

private:
    int m_num;

signals:
    void sendArry(QVector<int> list);

public slots:
};


// 冒泡排序
class BubbleSort : public QThread
{
    Q_OBJECT
public:
    explicit BubbleSort(QObject *parent = nullptr);

    void recvArry(QVector<int> list);

protected:
    void run() override;

private:
    QVector<int> m_list;

signals:
    void sendArry(QVector<int> list);

public slots:
};

// 快速排序
class QuickSort : public QThread
{
    Q_OBJECT
public:
    explicit QuickSort(QObject *parent = nullptr);

    void recvArry(QVector<int> list);

protected:
    void run() override;

private:
    QVector<int> m_list;

signals:
    void sendArry(QVector<int> list);

public slots:
};


#endif // MYTHREAD_H
