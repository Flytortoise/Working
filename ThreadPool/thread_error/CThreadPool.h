/*
CThreadPool
线程的容器

 */

#ifndef CTHREADPOOL_H_
#define CTHREADPOOL_H_

#include <vector>
#include <mutex>
#include <map>
#include <condition_variable>
#include "CWorkerThread.h"
#include "CJob.h"

using namespace std;

typedef std::mutex CThreadMutex;
typedef std::condition_variable CCondition; 

class CWorkerThread;

class CThreadPool
{

friend class CWorkerThread;

public:
    CThreadPool();
    CThreadPool(int initnum);
    virtual ~CThreadPool();

    void SetMaxNum(int maxnum){
        m_MaxNum = maxnum;
    }

    int GetMaxNum(){
        return m_MaxNum;
    }

    void SetAvailLowNum(int minnum){
        m_AvailLow = minnum;
    }

    int GetAvailLowNum(){
        return m_AvailLow;
    }

    void SetAvailHighNum(int highnum){
        m_AvailHigh = highnum;
    }

    int GetAvailHighNum(){
        return m_AvailHigh;
    }

    int GetActualAvailNum(){
        return m_AvailNum;
    }

    int GetAllNum(){
        return m_ThreadList.size();
    }

    int GetBusyNum(){
        return m_BusyList.size();
    }

    void SetInitNum(int initnum){
        m_InitNum = initnum;
    }

    int GetInitNum(){
        return m_InitNum;
    }

    void TerminateAll();
    void Run(CJob *job, void *jobdata);
    void clear();

    CThreadMutex m_BusyMutex;
    CThreadMutex m_IdleMutex;
    CThreadMutex m_JobMutex;
    CThreadMutex m_VarMutex;

    CCondition m_BusyCond;
    CCondition m_IdleCond;
    CCondition m_IdleJobCond;
    CCondition m_MaxNumCond;

    vector<CWorkerThread *> m_ThreadList;
    vector<CWorkerThread *> m_BusyList;
    vector<CWorkerThread *> m_IdleList;
    map<CWorkerThread *, std::thread > m_map;

protected:
    CWorkerThread *GetIdleThread();

    void AppendToIdleList(CWorkerThread *jobthread);
    void MoveToBusyList(CWorkerThread *idlethread);
    void MoveToIdleList(CWorkerThread *busythread);

    void DeleteIdleThread(int num);
    void CreateIdleThread(int num);

private:
    unsigned int m_MaxNum;          //当前线程池中所允许并发存在的线程的最大数目
    unsigned int m_AvailLow;        //当前线程池中允许存在的空闲线程的最小数目
                                        //如果空闲数目低于该值，就必须增加空闲线程，增加个数为 m_InitNum
    unsigned int m_AvailHigh;       //当前线程池中所允许的空闲的线程的最大数目，
                                        //如果空闲数目高于该值，表明当前负载较轻，删除多余的空闲线程，删除个数也为 m_InitNum
    unsigned int m_AvailNum;        //当前线程池中存在的线程个数，介于m_AvailLow和m_AvailHigh之间，
                                        //如果线程的个数始终在这之间，即平衡，如何设定两边界限的值，是需要考虑的
    unsigned int m_InitNum;         //最初创建时线程池中的个数

};


#endif