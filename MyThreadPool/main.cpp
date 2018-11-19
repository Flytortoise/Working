#include <iostream>
#include <Windows.h>

#include "threadtask.h"
#include "threadpool.h"

using namespace std;

class ThreadTaskA : public ThreadTask{
public:
    void run(){
        //Sleep(1000);
        cout << "TaskA" << endl;
    }
};

class ThreadTaskB : public ThreadTask{
public:
    void run(){
        //Sleep(1);
        cout << "TaskB" << endl;
    }
};



int main()
{

    ThreadPool *pool = ThreadPool::Instance();
    for(int i = 0; i < 5; i++){
        pool->AddTask(new ThreadTaskA);
        pool->AddTask(new ThreadTaskB);
    }

    delete pool;

    return 0;
}
