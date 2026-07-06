// 110condition_variable.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <thread>
#include <iostream>
#include <mutex>
#include <list>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;
list<string> msgs_;
mutex mux;
condition_variable cv;
void ThreadWrite()
{
    for (int i = 0;;i++)
    {
        stringstream ss;
        ss << "Write msg " << i;
        unique_lock<mutex> lock(mux); 
        msgs_.push_back(ss.str());
        lock.unlock();
        cv.notify_one(); //发送信号
        //cv.notify_all();
        this_thread::sleep_for(3s);
    }
}
void ThreadRead(int i)
{
    for (;;)
    {
        cout << i << "read msg" << endl;
        unique_lock<mutex> lock(mux);
        //cv.wait(lock);//解锁、阻塞等待信号
        cv.wait(lock, [i]
            {
                cout << i << " wait" << endl;
                return !msgs_.empty();
            });
        //获取信号后锁定
        while (!msgs_.empty())
        {
            cout << i << " read " << msgs_.front() << endl;
            msgs_.pop_front();
        }

    }
}
int main(int argc, char* argv[])
{
    thread th(ThreadWrite);
    th.detach();
    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadRead, i + 1);
        th.detach();
    }
    getchar();
    return 0;
}