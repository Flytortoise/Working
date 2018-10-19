/*
name: CThreadManger
功能：与客户交互，接受需要创建的线程初始个数，并接受客户端提交的任务
    
*/

#ifndef CTHREAD_MANAGER_H_
#define CTHREAD_MANAGER_H_

#include "CJob.h"
#include "CThreadPool.h"

class CThreadManager
{
public:
    CThreadManager();
    CThreadManager(int num);
    virtual ~CThreadManager();

    void Run(CJob *job, void *jobdata);
    void SetParallelNum(int num){
        m_NumOfThread = num;
    }

    void TerminateAll();
    void clear();

protected:

private:
    CThreadPool *m_Pool;        //线程池
    int m_NumOfThread;          //初始并发数

};


#endif