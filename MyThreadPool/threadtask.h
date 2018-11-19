#ifndef THREADTASK_H
#define THREADTASK_H


class ThreadTask
{
public:
    ThreadTask();
    virtual ~ThreadTask();
    virtual void run() = 0;
};

#endif // THREADTASK_H
