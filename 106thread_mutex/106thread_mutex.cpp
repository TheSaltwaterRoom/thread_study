// 106thread_mutex.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
//Linux -lpthread
using namespace std;
using namespace std::chrono_literals;

static mutex mux;
void TestThread()
{
    for (;;)
    {
        //获取锁资源，如果没有则阻塞等待
        //mux.lock(); //
        if (!mux.try_lock())
        {
            cout << "." << flush;
            this_thread::sleep_for(100ms);
            continue;
        }
        cout << "thread " << this_thread::get_id() << " get lock" << endl;
        cout << "==============================" << endl;
        cout << "test 001" << endl;
        cout << "test 002" << endl;
        cout << "test 003" << endl;
        cout << "==============================" << endl;
        mux.unlock();
        this_thread::sleep_for(1000ms);
    }
}

void ThreadMainMux(int i)
{
    for (;;)
    {
        mux.lock();
        cout << i << "[in]" << endl;
        this_thread::sleep_for(1000ms);
        mux.unlock();
        this_thread::sleep_for(1ms);
    }
}

timed_mutex tmux;

void ThreadMainTime(int i)
{


    for (;;)
    {
        if (!tmux.try_lock_for(chrono::milliseconds(500)))
        {
            cout << i << "[try_lock_for timeout]" << endl;
            continue;
        }
        cout << i << "[in]" << endl;
        this_thread::sleep_for(2000ms);
        tmux.unlock();
        this_thread::sleep_for(1ms);
    }
}

recursive_mutex rmux;
void Task1()
{
    rmux.lock();
    cout << "task1 [in]" << endl;
    rmux.unlock();
}
void Task2()
{
    rmux.lock();
    cout << "task2 [in]" << endl;
    rmux.unlock();
}
void ThreadMainRec(int i)
{
    for (;;)
    {
        rmux.lock();
        Task1();
        cout << i << "[in]" << endl;
        this_thread::sleep_for(2000ms);
        Task2();
        rmux.unlock();
        this_thread::sleep_for(1ms);
    }
}

int main(int argc, char* argv[])
{
    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadMainRec, i + 1);
        th.detach();
    }
    getchar();



    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadMainTime, i + 1);
        th.detach();
    }
    getchar();
    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadMainMux, i + 1);
        th.detach();
    }

    getchar();
    for (int i = 0; i < 10; i++)
    {
        thread th(TestThread);
        th.detach();
    }
    getchar();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
