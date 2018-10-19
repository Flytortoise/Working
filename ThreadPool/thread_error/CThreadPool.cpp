#include "CThreadPool.h"
#include <stdio.h>
#include <assert.h>
#include <algorithm>

#include <thread>

static int count = 0;

CThreadPool::CThreadPool()
{
    m_MaxNum = 50;
    m_AvailLow = 5;
    m_InitNum = m_AvailNum = 10;
    m_AvailHigh = 20;

    for(int i = 0; i < m_InitNum; i++){
        CWorkerThread *p = new CWorkerThread();
        p->SetThreadPool(this);
        AppendToIdleList(p);
        std::thread th(&CWorkerThread::Run, p);
        //p->Start();
    }
}

CThreadPool::CThreadPool(int initnum) 
{ 
    assert(initnum>0 && initnum<=30); 
    m_MaxNum   = 30; 
    m_AvailLow = initnum-10>0?initnum-10:3; 
    m_InitNum=m_AvailNum = initnum ;  
    m_AvailHigh = initnum+10; 

    m_BusyList.clear(); 
    m_IdleList.clear(); 
    for(int i=0;i<m_InitNum;i++){ 
        CWorkerThread* thr = new CWorkerThread(); 
        thr->SetThreadPool(this); 
        
        //printf("CThreadPool::CThreadPool\n");
        AppendToIdleList(thr); 
        //th.join();
        //printf("123\n");
        //thr->Start();       //begin the thread,the thread wait for job 
    } 
    //printf("asd\n");
}

CThreadPool::~CThreadPool(){
    TerminateAll();
}

void CThreadPool::TerminateAll(){
    /*
    for(int i = 0; i < m_ThreadList.size(); i++){
        CWorkerThread *p = m_ThreadList[i];
        p->Join();
    }
    */
   
    for(auto it = m_map.begin(); it != m_map.end(); ++it){
        it->second.join();
    }

    return;
}


CWorkerThread *CThreadPool::GetIdleThread(){
    std::mutex m_tmp;
    std::unique_lock<std::mutex> lck(m_tmp); 
    while(m_IdleList.size() == 0)
        m_IdleCond.wait(lck);

    m_IdleMutex.lock();
    if(m_IdleList.size() > 0){
        CWorkerThread *p = (CWorkerThread *)m_IdleList.front();
        printf("Get Idle thread %d \n",p->GetThreadID());
        m_IdleMutex.unlock();
        return p;
    }

    m_IdleMutex.unlock();
    return nullptr;
}

void CThreadPool::AppendToIdleList(CWorkerThread *jobthread){
    m_IdleMutex.lock();
    jobthread->SetThreadID(::count++);
    m_IdleList.push_back(jobthread);
    m_ThreadList.push_back(jobthread);

    //std::thread *th = new std::thread(&CWorkerThread::Start, jobthread);
    m_map[jobthread] = std::thread(&CWorkerThread::Start, jobthread);

    m_IdleMutex.unlock();
}

void CThreadPool::MoveToBusyList(CWorkerThread *idlethread){
    m_BusyMutex.lock();
    m_BusyList.push_back(idlethread);
    m_AvailNum--;
    m_BusyMutex.unlock();

    m_IdleMutex.lock();
    vector<CWorkerThread *>::iterator pos;
    pos = find(m_IdleList.begin(), m_IdleList.end(), idlethread);
    if(pos != m_IdleList.end())
        m_IdleList.erase(pos);

    m_IdleMutex.unlock();
}

void CThreadPool::MoveToIdleList(CWorkerThread *busythread){
    m_IdleMutex.lock();
    m_IdleList.push_back(busythread);
    m_AvailNum++;
    m_IdleMutex.unlock();

    m_BusyMutex.lock();
    vector<CWorkerThread *>::iterator pos;
    pos = find(m_BusyList.begin(), m_BusyList.end(), busythread);
    if( pos != m_BusyList.end())
        m_BusyList.erase(pos);
    m_BusyMutex.unlock();

    m_IdleCond.notify_all();
    m_MaxNumCond.notify_all();
}

void CThreadPool::CreateIdleThread(int num){
    printf("%d:%s\n", __LINE__, __FUNCTION__);

    for(int i = 0; i < num; i++){
        CWorkerThread *p = new CWorkerThread();
        p->SetThreadPool(this);
        AppendToIdleList(p);
        m_VarMutex.lock();
        m_AvailNum++;
        m_VarMutex.unlock();
        //p->Start();
    }
}

void CThreadPool::DeleteIdleThread(int num){
    printf("Enter into CThreadPool::DeleteIdleThread \n");

    m_IdleMutex.lock();
    printf("Delete Num is %d\n", num);
    for(int i = 0; i < num; i++){
        CWorkerThread *p = nullptr;
        if(m_IdleList.size() > 0){
            p = (CWorkerThread *)m_IdleList.front();
            printf("Get Idle thread %d\n", p->GetThreadID());
        }

        vector<CWorkerThread *>::iterator it;
        it = find(m_IdleList.begin(), m_IdleList.end(), p);
        if(it != m_IdleList.end()){
            m_IdleList.erase(it);
        }


        m_AvailNum--;
        printf("The idle thread available num :%d\n", m_AvailNum);
        printf("The idleList num:%d \n", (int)m_IdleList.size());
    }

    m_IdleMutex.unlock();
}

void CThreadPool::Run(CJob *job, void *jobdata){
    assert(job != nullptr);

    std::mutex m_tmp;
    std::unique_lock<mutex> lck(m_tmp);
    if(GetBusyNum() == m_MaxNum){
        printf("%d:%s\n", __LINE__, __FUNCTION__);
        m_MaxNumCond.wait(lck);
    }

    if(m_IdleList.size() < m_AvailLow){
        if(GetAllNum()+m_InitNum-m_IdleList.size() < m_MaxNum){
            CreateIdleThread(m_InitNum - m_IdleList.size());
        }
        else{
            CreateIdleThread(m_MaxNum - GetAllNum());
        }
    }

    CWorkerThread *p = GetIdleThread();
    if(p != nullptr){
        p->m_WorkMutex.lock();
        MoveToBusyList(p);
        p->SetThreadPool(this);
        job->SetWorkThread(p);
        printf("Job is set to thread %d\n", p->GetThreadID());
        p->SetJob(job, jobdata);
    }
}

void CThreadPool::clear(){
    for(int i = 0; i < m_IdleList.size(); i++){
        m_IdleList[i]->isStop = true;
    }

    for(int i = 0; i < m_BusyList.size(); i++){
        m_BusyList[i]->isStop = true;
    }
}







