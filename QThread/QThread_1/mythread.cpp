#include "mythread.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QtAlgorithms>


// 生成随机数
Generate::Generate(QObject *parent) : QThread(parent)
{

}

void Generate::run()
{
    QVector<int> list;
    QElapsedTimer time;
    time.start();
    for(int i = 0; i < m_num; i++)
    {
        list.push_back(qrand() % 10000);
    }

    long long milsec = time.elapsed();

    qDebug() << "生成" << m_num << "个随机数所用时间" << milsec << "毫秒";

    emit sendArry(list);
}

void Generate::recvNum(int num)
{
    m_num = num;
}

BubbleSort::BubbleSort(QObject *parent) : QThread(parent)
{

}


void BubbleSort::recvArry(QVector<int> list)
{
    m_list = list;
}


void BubbleSort::run()
{
    QElapsedTimer time;
    time.start();

    for (int i = 0; i < m_list.size(); i++) {
        // 标记是否发生了交换，优化冒泡排序
        bool swapped = false;

        // 从头到 m_list.size() - i - 1 逐步比较和交换
        for (int j = 0; j < m_list.size() - i - 1; j++) {
            if (m_list.at(j) > m_list.at(j + 1)) {
                qSwap(m_list[j], m_list[j + 1]);
                swapped = true;
            }
        }

        // 如果在某一轮没有发生交换，则排序已完成
        if (!swapped) break;
    }

    long long milsec = time.elapsed();
    qDebug() << "冒泡排序生成" << m_list.size() << "个随机数所用时间" << milsec << "毫秒";

    // 给主界面发送数据
    emit sendArry(m_list);
}

QuickSort::QuickSort(QObject *parent) : QThread(parent)
{

}

void QuickSort::recvArry(QVector<int> list)
{
    m_list = list;
}


void QuickSort::run()
{
    QElapsedTimer time;
    time.start();
    std::sort(m_list.begin(), m_list.end());

    long long milsec = time.elapsed();
    qDebug() << "快速排序生成" << m_list.size() << "个随机数所用时间" << milsec << "毫秒";

    // 给主界面发送数据
    emit sendArry(m_list);
}

