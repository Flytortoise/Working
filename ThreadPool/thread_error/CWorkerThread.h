/*
CWorkerThreader
具体的工作线程

 */

#ifndef CWorkerThread_H_
#define CWorkerThread_H_

#include <mutex>
#include <condition_variable>
#include "CThreadPool.h"
#include "CJob.h"
#include "CThread.h"

class CThreadPool;

typedef std::mutex CThreadMutex;
typedef std::condition_variable CCondition; 

class CWorkerThread : public CThread
{
public:
    CWorkerThread();
    virtual ~CWorkerThread();
    void Run();
    void SetJob(CJob *job, void *jobdata);
    CJob* GetJob() { return m_Job; }
    void SetThreadPool(CThreadPool *);
    CThreadPool *GetThreadPool() { return m_ThreadPool; }

    CCondition m_JobCond;
    CThreadMutex m_WorkMutex;

    bool isStop;

protected:


private:
    CThreadPool *m_ThreadPool;
    CJob *m_Job;
    void *m_JobData;

    CThreadMutex m_VarMutex;
    //bool m_IsEnd;

};


#endif