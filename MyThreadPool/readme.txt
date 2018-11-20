2018年11月20日
MyThreadPool

参考：
    https://www.cnblogs.com/cpper-kaixuan/p/3640485.html


存在的问题：
    用VS的32位和64位都没有问题（但仍然可能存在多线程问题）
    用QT 5.11.0 MinGW32bit,可能会出现不前进的状态，用QT调试会出现段错误，在特定位置加一条输出，就能正常运行了。
    用QT 5.11.0 msvc2017 64bit 一点输出都没有。


Q：
    crt\src\thr\mutex.c(131):unlock of unowned mutex"

A：
    unlock的线程与lock的线程要保持一致，且成对出现，保证lock一定会unlock且unlock不会重复
