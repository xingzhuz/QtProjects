#include "mythread.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QtAlgorithms>


// 生成随机数
Generate::Generate(QObject *parent) : QObject(parent)
{

}

void Generate::working(int num)
{
    QVector<int> list;
    QElapsedTimer time;
    time.start();
    for(int i = 0; i < num; i++)
    {
        list.push_back(qrand() % 10000);
    }

    long long milsec = time.elapsed();
    qDebug() << "生成" << num << "个随机数所用时间" << milsec << "毫秒";

    emit sendArry(list);
}

BubbleSort::BubbleSort(QObject *parent) : QObject(parent)
{

}


void BubbleSort::working(QVector<int> list)
{
    QElapsedTimer time;
    time.start();

    for (int i = 0; i < list.size(); i++) {
        // 标记是否发生了交换，优化冒泡排序
        bool swapped = false;

        // 从头到 m_list.size() - i - 1 逐步比较和交换
        for (int j = 0; j < list.size() - i - 1; j++) {
            if (list.at(j) > list.at(j + 1)) {
                qSwap(list[j], list[j + 1]);
                swapped = true;
            }
        }

        // 如果在某一轮没有发生交换，则排序已完成
        if (!swapped) break;
    }

    long long milsec = time.elapsed();
    qDebug() << "冒泡排序生成" << list.size() << "个随机数所用时间" << milsec << "毫秒";

    // 给主界面发送数据
    emit sendArry(list);
}

QuickSort::QuickSort(QObject *parent) : QObject(parent)
{

}

void QuickSort::working(QVector<int> list)
{
    QElapsedTimer time;
    time.start();
    std::sort(list.begin(), list.end());

    long long milsec = time.elapsed();
    qDebug() << "快速排序生成" << list.size() << "个随机数所用时间" << milsec << "毫秒";

    // 给主界面发送数据
    emit sendArry(list);
}

