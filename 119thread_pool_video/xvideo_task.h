#pragma once
#include "xthread_pool.h"
class XVideoTask :public XTask
{
public:
    std::string in_path;
    std::string out_path;
    int width = 0;
    int height = 0;
private:
    int Run();
};

