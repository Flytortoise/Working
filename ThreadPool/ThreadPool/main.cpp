#include <iostream>
#include <windows.h>

#include "threadpool.h"
#include "threadtask.h"
#include "workthread.h"
#include "filegol.h"
using namespace std;

class TaskA : public ThreadTask
{
public:
    void Run(void *p){
        Sleep(1);
        cout << "TaskA"<< endl;

    }
};

class TaskB : public ThreadTask
{
public:
    void Run(void *p){
        Sleep(1);
        cout << "TaskB"<< endl;
    }
};


int main()
{

    ThreadPool *p = new ThreadPool();
    p->Start();
    for(int i = 0; i < 5; i++){
        p->AddTask(new TaskA());
    }


    for(int i = 0; i < 8; i++){
        p->AddTask(new TaskB());
    }

    p->ShutDown();                 //退出线程池
    delete p;
    cout << "Main END!" << endl;


	system("pause");
    return 0;
}
