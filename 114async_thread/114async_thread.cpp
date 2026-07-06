// 114async_thread.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <thread>
#include <iostream>
#include <future>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

string TestAsync(int index)
{
    cout << index << " begin in TestAsync " << this_thread::get_id() << endl;
    this_thread::sleep_for(2s);
    return "TestAsync string return";
}
int main(int argc, char* argv[])
{
    //创建异步线程

    cout << "main thread id " << this_thread::get_id() << endl;
    //不创建线程启动异步任务
    auto future = async(launch::deferred, TestAsync, 100);
    this_thread::sleep_for(100ms);
    cout << "begin future get " << endl;
    cout << "future.get() = " << future.get() << endl;
    cout << "end future get" << endl;

    //创建异步线程
    cout << "=====创建异步线程====" << endl;
    auto future2 = async(TestAsync, 101);
    this_thread::sleep_for(100ms);
    cout << "begin future2 get " << endl;
    cout << "future2.get() = " << future2.get() << endl;
    cout << "end future2 get" << endl;


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
