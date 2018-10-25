#include <exception>
#include <iostream>
#include <algorithm>
#include "threadpool.h"
#include "workthread.h"
#include "filegol.h"
#include "def.h"


ThreadPool::ThreadPool()
{

    m_status = ThreadPool::INIT;
    m_MaxNum = MAXNUM;
    m_InitNum = INITNUM;
    m_IdleMaxNum = IDLEMAXNUM;
    m_IdleMinNum = IDLEMINNUM;
    m_CurNum = INITNUM;

    m_BusyThreadList.clear();
    m_IdleThreadList.clear();

    for(int i = 0; i < m_InitNum; i++){
        WorkThread *work = new WorkThread();
        work->SetThreadPool(this);
        AppendToIdleList(work);
    }


}


ThreadPool::~ThreadPool(){

    ShutDown();
    std::cout << "ThreadPool End!" << std::endl;

}

void ThreadPool::Loop(){

    for(auto thread : m_BusyThreadList){
        if(thread->isJoinAble())
            thread->Join();
    }

    for(auto thread : m_IdleThreadList){
        if(thread->isJoinAble())
            thread->Join();
    }


}

WorkThread *ThreadPool::GetIdleThread(){

    std::unique_lock<std::mutex> lck(m_IdleMutex);
    while(m_IdleThreadList.size() == 0){
        m_IdleCondition.wait(lck);
    }

    if(m_IdleThreadList.size() > 0){
        WorkThread *work = m_IdleThreadList.back();
        m_IdleThreadList.pop_back();
        //std::cout << "Get Idle thread  " << work->GetThreadID() << std::endl;
        return work;
    }


    return nullptr;
}

void ThreadPool::AppendToIdleList(WorkThread *work){


    std::unique_lock<std::mutex> lck(m_IdleMutex);

    m_IdleThreadList.push_back(work);
    work->Start();


}

void ThreadPool::MoveToBusyList(WorkThread *idlethrea){
    {
        std::unique_lock<std::mutex> lck(m_BusyMutex);
        m_BusyThreadList.push_back(idlethrea);
        m_CurNum--;
    }

/*
    std::unique_lock<std::mutex> lck(m_IdleMutex);
    auto pos = std::find(m_IdleThreadList.begin(), m_IdleThreadList.end(), idlethrea);
    if(pos != m_IdleThreadList.end())
        m_IdleThreadList.erase(pos);
*/

}

void ThreadPool::MoveToIdleList(WorkThread *busythread){
    {
        std::unique_lock<std::mutex> lck(m_IdleMutex);
        m_IdleThreadList.push_back(busythread);
        m_CurNum++;
    }

    std::unique_lock<std::mutex> lck(m_BusyMutex);
    auto pos = std::find(m_BusyThreadList.begin(), m_BusyThreadList.end(), busythread);
    if(pos != m_BusyThreadList.end())
        m_BusyThreadList.erase(pos);

    m_IdleCondition.notify_all();
    m_MaxNumCondition.notify_all();

    //std::cout << "MoveToIdleList::m_IdleThreadList::" << m_IdleThreadList.size() << std::endl;
    //std::cout << "MoveToIdleList::m_BusyThreadList::" << m_BusyThreadList.size() << std::endl;
   
}

void ThreadPool::CreateIdleThread(int num){

    if(num < 0 || m_CurNum + num > m_MaxNum)
        throw std::range_error("ThreadPool CreateIdleThread :: num Range Out!");

    for(int i = 0; i < num; i++){
        WorkThread *work = new WorkThread();
        work->SetThreadPool(this);
        AppendToIdleList(work);

        std::unique_lock<std::mutex> lck(m_CurNumMutex);
        m_CurNum++;
    }
}

void ThreadPool::DeleteIdleThread(int num){

    std::cout << "DeleteIdleThread::num::" << num << std::endl;

    if(num < 0 || m_CurNum - num < 0){
        throw std::range_error("ThreadPool DeleteIdleThread :: num Range Out!");
    }

    std::unique_lock<std::mutex> lck(m_IdleMutex);
    for(int i = 0; i < num && m_IdleThreadList.size() != 0; i++){
        WorkThread *work = m_IdleThreadList.back();
        work->Interrupt();
        if(work->isJoinAble())
            work->Join();
        delete work;
		m_IdleThreadList.pop_back();
        m_CurNum--;
    }
    std::cout << "END::" << m_IdleThreadList.size() << std::endl;

}

void ThreadPool::Start(){
    m_status = ThreadPool::RUNNING;
}

void ThreadPool::AddTask(ThreadTask *task){
    if(task == nullptr)
        throw std::invalid_argument("ThreadPool AddTask :: nullptr!");

    if(m_status == SHUTDOWM){
        throw  std::invalid_argument("ThreadPool AddTask::Status is ShutDown ! \n Please Start ThreadPool");
    }

    std::unique_lock<std::mutex> lck(m_AddTaskMutex);
    if(m_BusyThreadList.size() == m_MaxNum){        //busy线程达到上限
        m_MaxNumCondition.wait(lck);
    }

    //std::cout << m_IdleThreadList.size() << std::endl;

    if(m_IdleThreadList.size() < m_IdleMinNum){     //空闲线程过低
        unsigned  int allsize = m_IdleThreadList.size() + m_BusyThreadList.size() + m_InitNum;
        if(allsize < m_MaxNum)
            CreateIdleThread(m_InitNum - m_IdleThreadList.size());
        else
            CreateIdleThread(m_MaxNum - allsize);
    }


    WorkThread *work = GetIdleThread();
    if(work != nullptr){
        MoveToBusyList(work);
        work->SetThreadTask(task);
    }
}

void ThreadPool::MoveToIdle(WorkThread *work){
   
	std::unique_lock<std::mutex> lck(m_MoveToIdleMutex);
	this->MoveToIdleList(work);
    if(this->m_IdleThreadList.size() > m_IdleMaxNum){   //空闲线程过多
        this->DeleteIdleThread(this->m_IdleThreadList.size()
                                       - this->GetInitNum());
    }

	std::cout << "m_Busy::" << m_BusyThreadList.size() << std::endl;
	if (m_BusyThreadList.size() == 0)
		m_BusyNULLCondition.notify_all();
}

int count = 1;
void ThreadPool::ShutDown(){

    std::cout << "m_IdleThreadList::" << m_IdleThreadList.size() << std::endl;
    std::cout << "m_BusyThreadList::" << m_BusyThreadList.size() << std::endl;

    std::unique_lock<std::mutex> lck(m_BusyNULLMutex);
    m_status = ThreadPool::SHUTDOWM;

    if(m_BusyThreadList.size() > 0)
        m_BusyNULLCondition.wait(lck);

    std::cout << "m_IdleThreadList::" << m_IdleThreadList.size() << std::endl;
    std::cout << "m_BusyThreadList::" << m_BusyThreadList.size() << std::endl;


    for(auto it : m_IdleThreadList){
        std::cout << count++ << std::endl;
        it->Interrupt();
    }
    std::cout << "shutdown::" << std::endl;
    this->DeleteIdleThread(m_IdleThreadList.size());
    this->Loop();
}













