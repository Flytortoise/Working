#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <mutex>

#include "workthread.h"
#include "threadtask.h"

class ThreadPool
{
public:
    ThreadPool();

    void Start();                   //启动线程池
    void AddTask(ThreadTask *);     //添加任务,并自动执行
    void Loop();                    //等待所有线程执行结束

    void SetInitNum(int num) { m_InitNum = num; }
    int GetInitNum() { return m_InitNum; }
    void SetMaxNum(int num) { m_MaxNum = num; }
    int GetMaxNum() { return m_MaxNum; }
    void SetMinNum(int num) { m_MinNum = num; }
    int GetMinNum() { return m_MinNum; }
    void SetIdleMaxNum(int num) { m_IdleMaxNum = num; }
    int GetIdleMaxNum() { return m_IdleMaxNum; }

    MyThread *

private:
    int m_InitNum;      //单位增加（减小）线程大小
    int m_MaxNum;       //最大允许的总线程数
    int m_MinNum;       //最小允许的总线程数
    int m_CurNum;       //当前存在的线程数
    int m_IdleMaxNum;   //最大允许的空闲线程数

    std::list<MyThread *> m_BusyThreadList;     //执行的线程
    std::list<MyThread *> m_IdleThreadList;     //空闲的线程
};

#endif // THREADPOOL_H
