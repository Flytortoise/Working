#include "CJob.h"
#include <cstring>

CJob::CJob() : m_pWorkThread(nullptr),m_JobNo(0),m_JobName(nullptr){

}

CJob::~CJob(){
    if(m_JobName != nullptr)
        delete m_JobName;
}

void CJob::SetJobName(char *jobname){
    if(m_JobName != nullptr){
        delete m_JobName;
        m_JobName = nullptr;
    }

    if(jobname != nullptr){
        m_JobName = new char[strlen(jobname)+1]();
        strcpy(m_JobName, jobname);
    }
}

