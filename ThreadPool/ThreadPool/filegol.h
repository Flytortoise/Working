#ifndef FILEGOL_H
#define FILEGOL_H

#include <fstream>
#include <iostream>

void LOG(const char *buff, unsigned int LINE,  const char *FILE );

#define DELOG(BUFF) LOG((BUFF),(__LINE__),(__FILE__))


#endif // FILEGOL_H
