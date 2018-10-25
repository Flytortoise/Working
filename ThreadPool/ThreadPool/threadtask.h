#ifndef THREADTASK_H
#define THREADTASK_H

//class ThreadPool;
//class WorkThread;

class ThreadTask
{
public:
    ThreadTask();
    virtual ~ThreadTask();

    virtual void Run(void *p) = 0;

    //void SetThreadPool(ThreadPool *threadpool);
    //void SetWorkThread(WorkThread *workthread);

protected:
    //ThreadPool *m_threadpool;
    //WorkThread *m_workthread;
};

#endif // THREADTASK_H
