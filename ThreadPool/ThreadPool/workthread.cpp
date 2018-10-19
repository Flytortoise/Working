#include "workthread.h"

WorkThread::WorkThread()
{
    m_task = nullptr;
}

void WorkThread::SetThreadTask(ThreadTask *t){
    m_task = t;
}

void WorkThread::Run(){
    if(m_task == nullptr)
        return;

    while(!isInterrupt()){
        m_task->Run(nullptr);
    }
}
