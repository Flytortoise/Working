#include "CWorkerThread.h"

CWorkerThread::CWorkerThread(){
    m_Job = nullptr;
    m_JobData = nullptr;
    m_ThreadPool = nullptr;
    isStop = false;
}

CWorkerThread::~CWorkerThread(){
    if(m_Job != nullptr){
        delete m_Job;
    }

    if(m_ThreadPool != nullptr){
        delete m_ThreadPool;
    }
}

void CWorkerThread::Run(){
    //printf("CWorkerThread::Run\n"); 
    SetThreadState(THREAD_RUNING);


    while(1){
        std::mutex m_tmp;
        std::unique_lock<std::mutex> lck(m_tmp);
        while(m_Job == nullptr){
            //printf("ddd\n");
            m_JobCond.wait(lck);
            if(isStop == true){
                printf("12\n");
                return;
            }
        }

        m_Job->Run(m_JobData);
        m_Job->SetWorkThread(nullptr);
        m_Job = nullptr;
        //printf("%d:%s\n", __LINE__,__FUNCTION__);
        m_ThreadPool->MoveToIdleList(this);
        if(m_ThreadPool->m_IdleList.size() > m_ThreadPool->GetAvailHighNum()){
            m_ThreadPool->DeleteIdleThread(m_ThreadPool->m_IdleList.size() - m_ThreadPool->GetInitNum());
        }

        m_WorkMutex.unlock();
    }
}

void CWorkerThread::SetJob(CJob *job, void *jobdata){
    m_VarMutex.lock();
    m_Job = job;
    m_JobData = jobdata;
    job->SetWorkThread(this);
    m_VarMutex.unlock();
    m_JobCond.notify_all();
}

void CWorkerThread::SetThreadPool(CThreadPool *p){
    m_VarMutex.lock();
    m_ThreadPool = p;
    m_VarMutex.unlock();
}
