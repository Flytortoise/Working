#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <thread>
#include <atomic>

using std::thread;
using std::atomic;

class MyThread
{
public:
    MyThread();
    virtual ~MyThread();

    void Start();
    virtual void Run() = 0;
    void Interrupt();
    void Join();

    std::thread::id GetThreadID();
    bool isInterrupt();

private:
    atomic<bool> m_isInterrupt;
    thread m_thread;
};

#endif // MYTHREAD_H
