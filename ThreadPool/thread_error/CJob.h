/*

CJob
 */

#ifndef CJOB_H_
#define CJOB_H_

#include "CThread.h"


class CJob
{
public:
    CJob();
    virtual ~CJob();

    int GetJobNo() const { return m_JobNo; }
    void SetJobNo(int jobno) { m_JobNo = jobno; }
    char *GetJobName() const { return m_JobName; }
    void SetJobName(char *jobname);
    CThread *GetWorkThread(CThread *p){
        m_pWorkThread = p;
    }

    void SetWorkThread(CThread *p){
        m_pWorkThread = p;
    }

    virtual void Run(void *p) = 0;

protected:

private:
    int m_JobNo;
    char *m_JobName;
    CThread *m_pWorkThread;

};

#endif