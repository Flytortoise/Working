#ifndef THREADTASK_H
#define THREADTASK_H


class ThreadTask
{
public:
    ThreadTask();
    virtual ~ThreadTask();

    virtual void Run(void *p) = 0;
};

#endif // THREADTASK_H
