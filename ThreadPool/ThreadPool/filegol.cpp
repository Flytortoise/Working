#include "filegol.h"
#include <mutex>

const char file[] = "LOG.txt";
std::ofstream f;

static int fileflag = 0;

std::mutex filemutex;

void LOG(const char *buff, unsigned int LINE,  const char *FILE){
    std::unique_lock<std::mutex> lck(filemutex);

    if(!fileflag){
        fileflag = 1;
        f.open(file);       //将LOG.txt清空
        f.close();
    }

    f.open(file, std::ios::app);
    f<< "[" << FILE << "] " << LINE  << ":" << buff << '\n';
    f.close();
}

