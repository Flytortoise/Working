/*
    CThread
    实现了对Linux中线程操做的封装，是所有线程类的基类，也是一个抽象类，
    提供了一个抽象接口Run()

 */


#ifndef CTHREAD_H_
#define CTHREAD_H_

#include <cstring>
#include <thread>
#include <stdio.h>

using namespace std;

enum ThreadState
{
    THREAD_RUNING,
};


class CThread
{
public:
    CThread(){}
    CThread(bool createsuspended, bool detach):m_CreateSuspended(createsuspended),
                                               m_Detach(detach)
                                               {}
    virtual ~CThread(){}
    virtual void Run() = 0;
    void SetThreadState(ThreadState state){
        m_ThreadState = state;
    }

    bool Terminate();
    bool Start() {
        //printf("CThread::Start\n"); 
        //thread m_thread(&this->Run, this);
        Run(); 
    }
    bool Exit();
    bool Wakeup();

    ThreadState GetThreadState(){
        return m_ThreadState;
    }

    int GetLastError(){
        return m_ErrCode;
    }

    void SetThreadName(char *thrname){
        strcpy(m_ThreadName, thrname);
    }

    char *GetThreadName(){
        return m_ThreadName;
    }

    int GetThreadID(){
        return m_ThreadID;
    }

    void SetThreadID(int id){
        m_ThreadID = id;
    }

    bool SetPriority(int priority);
    int GetPriority();
    int GetConcurrency();
    void SetConcurrency(int num);
    bool Detach() {  }
    bool Join() {  }
    bool Yield();
    int Self();

protected:
    void SetErrCode(int errcode){
        m_ErrCode = errcode;
    }

    static void *ThreadFunciton(void *);


private:
    int m_ErrCode;
    //Semaphore m_ThreadSemaphore;
    unsigned long m_ThreadID;
    bool m_Detach;
    bool m_CreateSuspended;
    char *m_ThreadName;
    ThreadState m_ThreadState;

};

#endif 