#include "threadwork.h"

ThreadWork::ThreadWork()
    : m_thread(&ThreadWork::run, this)
{
    m_task = nullptr;
    m_pool = nullptr;

    m_isInterrupt= false;
}

ThreadWork::~ThreadWork(){
    Interrupt();        //中断当前线程

    Join();
}

void ThreadWork::run(){
    while(1){
        {
        unique_lock<mutex> lck(m_task_mutex);
        while(m_task == nullptr || m_pool == nullptr){
            if(m_isInterrupt)
                return;
            m_task_condition.wait(lck);
        }
        }

        m_task->run();
        m_task = nullptr;
        m_pool->MoveToFree(this);

    }
}

void ThreadWork::Interrupt(){
    m_isInterrupt = true;      //一旦设为true，说明当前线程退出
    m_task_condition.notify_all();
}

void ThreadWork::Join(){
    if(m_thread.joinable())
        m_thread.join();
}


void ThreadWork::SetThreadTask(ThreadTask *task){
    {
    unique_lock<mutex> lck(m_task_mutex);
    m_task = task;
    }

    if(m_pool != nullptr)
        m_task_condition.notify_all();
}

void ThreadWork::SetThreadPool(ThreadPool *pool){
    {
    unique_lock<mutex> lck(m_task_mutex);
    m_pool = pool;
    }

    if(m_task != nullptr)
        m_task_condition.notify_all();
}
