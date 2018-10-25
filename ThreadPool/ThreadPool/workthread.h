#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

#include "threadtask.h"

class ThreadPool;

class WorkThread
{
public:
    enum{
        STOPPED,
        RUNNING,
        WAITTING
    };

    WorkThread();
    virtual ~WorkThread();

    void Start();
    virtual void Run();
    void SetThreadTask(ThreadTask *);
    virtual void Interrupt();
    void Join();

    std::thread::id GetThreadID();
    bool isInterrupt();

    void SetThreadPool(ThreadPool *);
    bool isJoinAble();


private:
    ThreadTask *m_task;
    std::mutex m_task_mutex;
    std::mutex m_run_mutex;
    std::condition_variable m_run_condition;

    std::atomic<bool> m_isInterrupt;
    std::thread m_thread;

    ThreadPool *m_threadpool;
    std::mutex m_threadpool_mutex;

    int m_state;
};

#endif // WORKTHREAD_H
