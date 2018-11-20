#include <iostream>
#include <algorithm>
#include "threadpool.h"
#include "def.h"
#include "threadtask.h"
#include "threadwork.h"


ThreadPool *ThreadPool::m_instance = nullptr;

ThreadPool *ThreadPool::Instance(){
    if(m_instance == nullptr){
        m_instance = new ThreadPool();
    }

    return m_instance;
}

void ThreadPool::Destory(){
    if(m_instance != nullptr){
        delete m_instance;
        m_instance = nullptr;
    }
}

ThreadPool::ThreadPool()
{
    m_status = INIT;
    m_MinFreeSize = MINFREESIZE;
    m_MaxFreeSize = MAXFREESIZE;
    m_MaxBusySize = MAXBUSYSIZE;

    CreateFreeThread(m_MinFreeSize);
}

ThreadPool::~ThreadPool(){
    if(m_status != SHUTDOWN)
        this->ShutDown();
}

void ThreadPool::ShutDown(){
    m_status = SHUTDOWN;

    {
    //cout << "LINE:" <<__LINE__ << endl;
    unique_lock<mutex> lck(m_BusyMutex);
    if(m_BusyVector.size() != 0)
        m_BusyNotEmptyCondition.wait(lck);
    //cout << "LINE:" <<__LINE__ << endl;

    }

    DeleteFreeThread(m_FreeVector.size());


    std::cout << "Thread Pool END" << endl;
}

void ThreadPool::AddTask(ThreadTask *task){
    if(task == nullptr)
        return;

    //cout << "AddTask" << endl;
    m_status = RUNNING;
    ThreadWork *work = GetFreeThreadWork();
    work->SetThreadTask(task);              //加入task后，自动执行
    //cout << "AddTask end" << endl;
}

ThreadWork* ThreadPool::GetFreeThreadWork(){
    ThreadWork *work = nullptr;

    //增加FreeVector的大小，调用MoveToFree,是在其他线程进行的，所以需要同步
    m_FreeMutex.lock();
    if(m_FreeVector.size() < m_MinFreeSize){    //尽可能保证FreeVector大小同步
        m_FreeMutex.unlock();       //在CreateFreetThread会上这把锁
        CreateFreeThread(m_MinFreeSize - m_FreeVector.size());
    }
	else{
		m_FreeMutex.unlock();
	}
    

    {
    unique_lock<mutex> lck(m_FreeMutex);
    work = m_FreeVector.back();
    }

    {
    unique_lock<mutex> lck(m_BusyMutex);
    if(m_BusyVector.size() >= m_MaxBusySize)
        m_BusyNotMaxCondition.wait(lck);
    }
    MoveToBusy(work);

    //减小FreeVector的大小，是调用MoveToBusy,在主线程进行的，所以肯定是同步的
    if(m_FreeVector.size() > m_MaxFreeSize){
        DeleteFreeThread(m_FreeVector.size() - m_MaxFreeSize);
    }

    return work;
}

void ThreadPool::MoveToFree(ThreadWork *work){
    {
    unique_lock<mutex> lck(m_FreeMutex);
    m_FreeVector.push_back(work);
    }

    unique_lock<mutex> lck(m_BusyMutex);
    auto pos = find(m_BusyVector.begin(), m_BusyVector.end(), work);
    if(pos != m_BusyVector.end())
        m_BusyVector.erase(pos);

    //cout << m_BusyVector.size() << endl;
    if(m_BusyVector.size() == 0)
        m_BusyNotEmptyCondition.notify_all();

    if(m_BusyVector.size() < m_MaxBusySize)
        m_BusyNotMaxCondition.notify_all();
}

void ThreadPool::MoveToBusy(ThreadWork *work){
    {
    unique_lock<mutex> lck(m_BusyMutex);
    m_BusyVector.push_back(work);
    }

    unique_lock<mutex> lck(m_FreeMutex);
    auto pos = find(m_FreeVector.begin(), m_FreeVector.end(), work);
    if(pos != m_FreeVector.end())
        m_FreeVector.erase(pos);

}

bool ThreadPool::CreateFreeThread(unsigned int num){
    {
    unique_lock<mutex> lck(m_FreeMutex);
    //再一次确认大小
    if(m_FreeVector.size() >= m_MaxFreeSize || m_FreeVector.size() + num > m_MaxFreeSize)
        return false;


    for(unsigned int i = 0; i < num; i++){
        ThreadWork *work = new ThreadWork();
        work->SetThreadPool(this);
        m_FreeVector.push_back(work);
    }
    }

    return true;
}

bool ThreadPool::DeleteFreeThread(unsigned int num){
    unique_lock<mutex> lck(m_FreeMutex);
    if(num <= 0 || static_cast<WorkVectorType>(num) > m_FreeVector.size())
        return false;

    //cout << __LINE__ << endl;
    ThreadWork *MyWork = nullptr;
    for(unsigned int i = 1; i <= num; i++){
        ThreadWork *work = m_FreeVector.back();
        if(work->GetThreadID() == std::this_thread::get_id()){      //当前线程，不能删除
            MyWork = work;
            m_FreeVector.pop_back();
            i--;
            continue;
        }

        work->Interrupt();          //中断
        work->Join();               //阻塞
        m_FreeVector.pop_back();    //删除
        delete work;                //释放
    }

    if(MyWork != nullptr)
        m_FreeVector.push_back(MyWork);

    return true;
}






























