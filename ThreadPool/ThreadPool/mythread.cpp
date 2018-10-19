#include "mythread.h"
#include <stdio.h>

MyThread::MyThread()
{
    m_isInterrupt = false;
}

MyThread::~MyThread(){
    if(!m_isInterrupt){
        this->Interrupt();
    }

    if(this->m_thread.joinable()){
        this->m_thread.join();
    }
}

void MyThread::Start(){
    std::thread th(std::bind(&MyThread::Run, this));
    this->m_thread = std::move(th);
}


std::thread::id MyThread::GetThreadID(){
    return m_thread.get_id();
}

void MyThread::Interrupt(){
    m_isInterrupt = true;
}

bool MyThread::isInterrupt(){
    return m_isInterrupt;
}

void MyThread::Join(){
    m_thread.join();
}

