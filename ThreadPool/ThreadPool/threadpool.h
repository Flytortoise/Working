#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <mutex>
#include <condition_variable>


#include "threadtask.h"

class WorkThread;

class ThreadPool
{

public:
    enum{
        INIT,
        RUNNING,
        SHUTDOWM
    };

    ThreadPool();
    virtual ~ThreadPool();

    void Start();                   //启动线程池
    void AddTask(ThreadTask *);     //添加任务,并自动执行，不负责任务空间的释放
    void ShutDown();               //退出线程池，等所有busy线程执行结束才退出

    void SetInitNum(int num) { m_InitNum = num; }
    int GetInitNum() { return m_InitNum; }
    void SetMaxNum(int num) { m_MaxNum = num; }
    int GetMaxNum() { return m_MaxNum; }
    void SetIdleMinNum(int num) { m_IdleMinNum = num; }
    int GetIdleMinNum() { return m_IdleMinNum; }
    void SetIdleMaxNum(int num) { m_IdleMaxNum = num; }
    int GetIdleMaxNum() { return m_IdleMaxNum; }

    void MoveToIdle(WorkThread *work);

private:
    void Loop();                    //等待所有线程执行结束

protected:
    WorkThread *GetIdleThread();
    void AppendToIdleList(WorkThread *mythread);          //添加到IdleList
    void MoveToBusyList(WorkThread *idlethread);          //移动到BusyList
    void MoveToIdleList(WorkThread *busythread);          //移动到IdleLIst

    void DeleteIdleThread(int num);                     //删除指定数目的空闲线程
    void CreateIdleThread(int num);                     //添加指定数目的空闲线程

private:
    int m_status;       //当前线程池的工作状态
    int m_InitNum;      //单位增加（减小）线程大小
    int m_MaxNum;       //最大允许的总线程数
    int m_CurNum;       //当前空闲的线程数
    int m_IdleMaxNum;   //最大允许的空闲线程数
    int m_IdleMinNum;   //最小允许的空闲线程数

    std::vector<WorkThread *> m_BusyThreadList;     //执行的线程
    std::vector<WorkThread *> m_IdleThreadList;     //空闲的线程

    std::mutex m_IdleMutex;
    std::mutex m_BusyMutex;
    std::mutex m_CurNumMutex;
    std::mutex m_AddTaskMutex;
    std::mutex m_BusyNULLMutex;
	std::mutex m_MoveToIdleMutex;


    std::condition_variable m_IdleCondition;
    std::condition_variable m_MaxNumCondition;
    std::condition_variable m_BusyNULLCondition;
};

#endif // THREADPOOL_H
