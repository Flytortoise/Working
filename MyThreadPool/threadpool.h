#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

class ThreadWork;
class ThreadTask;

using WorkVectorType=vector<ThreadWork *>::size_type;

class ThreadPool
{
public:
    enum{
        INIT,
        RUNNING,
        SHUTDOWN,

    };

    static ThreadPool* Instance();
    static void Destory();

    ~ThreadPool();
    void run()                         //启动线程池
    {
        m_status = RUNNING;
    }

    void ShutDown();                    //关闭线程池

    void AddTask(ThreadTask *);         //添加任务
    void MoveToFree(ThreadWork *);      //将工作线程移到Free
    void MoveToBusy(ThreadWork *);      //将工作线程移到Busy

    bool SetMinFreeSize(WorkVectorType size){
        if(size > m_MaxFreeSize || size <= 0)
            return false;

        m_MinFreeSize = size;
        return true;
    }

    bool SetMaxBusySize(WorkVectorType size){
        if(size <= 0)
            return false;

        m_MaxBusySize = size;
        return true;
    }

    bool SetMaxFreeSize(WorkVectorType size){
        if(size < m_MaxFreeSize || size <= 0)
            return false;

        m_MaxFreeSize = size;
        return true;
    }

    WorkVectorType GetMinFreeSize() { return m_MinFreeSize; }
    WorkVectorType GetMaxBusySize() { return m_MaxBusySize; }
    WorkVectorType GetMaxFreeSize() { return m_MaxFreeSize; }

private:
    ThreadPool();
    bool CreateFreeThread(unsigned int num);     //创建指定大小的空闲线程
    bool DeleteFreeThread(unsigned int num);      //删除指定大小的空闲线程
    ThreadWork *GetFreeThreadWork();     //获取一个空闲线程

private:
    static ThreadPool *m_instance;

    vector<ThreadWork *> m_FreeVector;
    vector<ThreadWork *> m_BusyVector;

    mutex m_FreeMutex;
    mutex m_BusyMutex;

    condition_variable m_BusyNotEmptyCondition;
    condition_variable m_BusyNotMaxCondition;

    vector<ThreadWork>::size_type m_MaxBusySize;      //Busy的上限大小，超过这个大小则等待
    vector<ThreadWork>::size_type m_MaxFreeSize;      //Free的上限大小，超过这个需要减少空闲线程的个数
    vector<ThreadWork>::size_type m_MinFreeSize;      //初始大小，也是空闲线程的最小值

    int m_status;           //记录当前线程池状态
};

#endif // THREADPOOL_H
