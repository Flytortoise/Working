#ifndef THREADWORK_H
#define THREADWORK_H

#include <mutex>
#include <thread>
#include <condition_variable>

#include "threadtask.h"
#include "threadpool.h"

using namespace std;

class ThreadWork
{
public:
    ThreadWork();
    ~ThreadWork();

    void run();
    void Interrupt();
    void Join();

    void SetThreadTask(ThreadTask *);
    void SetThreadPool(ThreadPool *);
    thread::id GetThreadID() { return m_thread.get_id(); }

private:
    mutex m_task_mutex;
    condition_variable m_task_condition;
    thread m_thread;

    ThreadTask *m_task;
    ThreadPool *m_pool;

    bool m_isInterrupt;     //是否中断
};

#endif // THREADWORK_H
