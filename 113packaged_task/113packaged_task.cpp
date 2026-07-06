// 113packaged_task.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <thread>
#include <iostream>
#include <future>
#include <string>
#include <chrono>
using namespace std;

string TestPack(int index)
{
    cout << "begin Test Pack " << index << endl;
    this_thread::sleep_for(2s);
    return "Test Pack return";
}
int main(int argc, char* argv[])
{
    packaged_task< string(int) > task(TestPack);
    auto result = task.get_future();
    //task(100);
    thread th(move(task), 101);

    cout << "begin result get" << endl;

    //测试是否超时
    for (int i = 0; i < 30; i++)
    {
        if (result.wait_for(100ms) != future_status::ready)
        {
            continue;
        }
    }
    if (result.wait_for(100ms) == future_status::timeout)
    {
        cout << "wait result timeout" << endl;
    }
    else
        cout << "result get " << result.get() << endl;
    th.join();
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
