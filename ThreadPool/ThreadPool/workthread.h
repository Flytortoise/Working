#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include "mythread.h"
#include "threadtask.h"

class WorkThread : public MyThread
{
public:
    WorkThread();

    void Run();
    void SetThreadTask(ThreadTask *);

private:
    ThreadTask *m_task;
};

#endif // WORKTHREAD_H
