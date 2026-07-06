// 116thread_pool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "xthread_pool.h"
#include <iostream>
#include <chrono>
using namespace std;
class MyTask :public XTask
{
public:
    int Run()
    {
        cout << "================================================" << endl;
        cout << this_thread::get_id() << " Mytask " << name << endl;
        cout << "================================================" << endl;
        for (int i = 0; i < 10; i++)
        {
            //MyTask::Run()
            //↓
            //if (is_exit()) break;
            //↓
            //调用 XTask::is_exit 这个 std::function
            //↓
            //执行 AddTask 里设置的 lambda
            //↓
            //[this]{ return this->is_exit(); }
            //↓
            //调用 XThreadPool::is_exit()
            //↓
            //返回 XThreadPool::is_exit_
            if (is_exit())break;
            cout << ".=" << this_thread::get_id() << flush;
            this_thread::sleep_for(500ms);
        }
        return 0;
    }
    std::string name = "";
};
int main(int argc, char* argv[])
{

    XThreadPool pool;
    int th_count = thread::hardware_concurrency(); //系统支持的线程核心数
	std::cout << "thread::hardware_concurrency() = " << th_count << std::endl;
    pool.Init(16);
    pool.Start();

    //MyTask task1;
    //task1.name = "test name 001";
    //pool.AddTask(&task1);
    //MyTask task2;
    //task2.name = "test name 002";
    //pool.AddTask(&task2);

    {
        auto task3 = make_shared<MyTask>();
        task3->name = "test shared 003";
        pool.AddTask(task3);

        auto task4 = make_shared<MyTask>();
        task4->name = "test shared 004";
        pool.AddTask(task4);
        auto re = task4->GetReturn();
        cout << "task4->GetReturn() = " << re << endl;
    }
    this_thread::sleep_for(100ms);
    cout << "task run  count = " << pool.task_run_count() << endl;


    this_thread::sleep_for(1s);
    pool.Stop();
    cout << "task run  count = " << pool.task_run_count() << endl;

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
