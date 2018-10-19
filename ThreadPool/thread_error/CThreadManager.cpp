#include "CThreadManager.h"

#define DEFAULT 10

CThreadManager::CThreadManager(){
    m_NumOfThread = DEFAULT;
    m_Pool = new CThreadPool(m_NumOfThread);
}

CThreadManager::CThreadManager(int num): m_NumOfThread(num){
    m_Pool = new CThreadPool(m_NumOfThread);
}

CThreadManager::~CThreadManager()
{
    if(m_Pool){
        delete m_Pool;
    }
}

void CThreadManager::Run(CJob *job, void *jobdata){
    m_Pool->Run(job, jobdata);
}

void CThreadManager::TerminateAll(){
    m_Pool->TerminateAll();
}

void CThreadManager::clear(){
    m_Pool->clear();
}
