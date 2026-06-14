#include <thread>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;
//Linux -lpthread
using namespace std;

class Para
{
public:
    Para() { cout << "Create Para" << endl; }
    Para(const Para& p) { cout << "Copy Para" << endl; }
    ~Para() { cout << "Drop Para" << endl; }
    string name;
};

void ThreadMain(int p1, float p2, string str, Para p4)
{
    this_thread::sleep_for(100ms);
    cout << "ThreadMain " << p1 << " " << p2 << " " << str << " " << p4.name << endl;
}

int main(int argc, char* argv[])
{
    thread th;
    {
        float f1 = 12.1f;
        Para p;
        p.name = "test Para class";
        //所有的参数做复制
        th = thread(ThreadMain, 101, f1, "test string para", p);
    }
    th.join();

    return 0;
}