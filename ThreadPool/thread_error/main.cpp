#include <stdio.h>

#include "CJob.h"

#include "CThreadManager.h"

class CXJob:public CJob 
{ 
public: 
    CXJob(){} 
    ~CXJob(){} 
    void Run(void* jobdata)    { 
        printf("The Job comes from CXJOB\n"); 
        sleep(2); 
    } 
}; 

class CYJob:public CJob 
{ 
public: 
    CYJob(){} 
    ~CYJob(){} 
    void Run(void* jobdata)    { 
        printf("The Job comes from CYJob\n"); 
    } 
}; 

main() 
{ 
    CThreadManager* manage = new CThreadManager(10); 
   // for(int i=0;i<10;i++) 
    { 
        CXJob* job = new CXJob(); 
        manage->Run(job,NULL); 
    } 
    sleep(2); 
    //printf("asdas\n");
    CYJob* job = new CYJob(); 
    manage->Run(job,NULL); 
    manage->clear();
    manage->TerminateAll(); 
    printf("end\n");
    //system("pause");
} 