#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QVector>


// 生成随机数
class Generate : public QObject
{
    Q_OBJECT
public:
    explicit Generate(QObject *parent = nullptr);

    void working(int num);

signals:
    void sendArry(QVector<int> list);

public slots:
};


// 冒泡排序
class BubbleSort : public QObject
{
    Q_OBJECT
public:
    explicit BubbleSort(QObject *parent = nullptr);

    void working(QVector<int> list);

signals:
    void sendArry(QVector<int> list);

public slots:
};

// 快速排序
class QuickSort : public QObject
{
    Q_OBJECT
public:
    explicit QuickSort(QObject *parent = nullptr);

    void working(QVector<int> list);

signals:
    void sendArry(QVector<int> list);

public slots:
};


#endif // MYTHREAD_H
