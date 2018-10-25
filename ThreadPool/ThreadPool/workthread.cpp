#include <functional>
#include "workthread.h"
#include "threadpool.h"

WorkThread::WorkThread()
{
    m_isInterrupt = false;
    m_task = nullptr;
    m_state = STOPPED;
}

WorkThread::~WorkThread(){
    if(!m_isInterrupt){
        this->Interrupt();
    }

    if(this->m_thread.joinable()){
        this->m_thread.join();
    }
}

void WorkThread::Start(){
    std::thread th(std::bind(&WorkThread::Run, this));
    this->m_thread = std::move(th);
}

std::thread::id WorkThread::GetThreadID(){
    return m_thread.get_id();
}

bool WorkThread::isInterrupt(){
    return  m_isInterrupt;
}

void WorkThread::Join(){
    m_thread.join();			//递归！！
}

void WorkThread::SetThreadTask(ThreadTask *t){
    m_task_mutex.lock();
    m_task = t;
    m_task_mutex.unlock();
    m_run_condition.notify_all();
}

void WorkThread::Run(){
    while(1){
        //printf("%d:%s\n", __LINE__, __FUNCTION__);
        std::unique_lock<std::mutex> lck(m_run_mutex);
        while(m_task == nullptr){
            if(isInterrupt()){
                return;
            }

            m_state = WAITTING;
            m_run_condition.wait(lck);
        }
        //printf("%d:%s\n", __LINE__, __FUNCTION__);
        m_state = RUNNING;
        if(m_task != nullptr){
            m_task->Run(nullptr);
            m_task = nullptr;
        }

        m_threadpool->MoveToIdle(this);
    }

}

void WorkThread::Interrupt(){
	
    //m_run_mutex.lock();		//使用m_run_mutex将导致递归锁
    m_isInterrupt = true;
    //m_run_mutex.unlock();

    if(m_state == WAITTING)
        m_run_condition.notify_all();
}

void WorkThread::SetThreadPool(ThreadPool *threadpool){
    m_threadpool_mutex.lock();
    m_threadpool = threadpool;

    m_threadpool_mutex.unlock();
}

bool WorkThread::isJoinAble(){
    return m_thread.joinable();
}


















