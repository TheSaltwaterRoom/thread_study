// 119thread_pool_video.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <chrono>
#include "xthread_pool.h"
#include "xvideo_task.h"
using namespace std;
/// 命令行视频转码工具
/// ffmpeg工具
/// 用户输入 视频源 输出视频尺寸
/// 在线程池中执行转码任务
/// 转码任务调用ffmpeg
/// ffmpeg -y -i test.mp4 -s 400x300 400.mp4 >log.txt 2>&1

int main(int argc, char* argv[])
{
    XThreadPool pool;
    pool.Init(16);
    pool.Start();
    this_thread::sleep_for(200ms);

    for (;;)
    {
        this_thread::sleep_for(200ms);
        cout << "\n====================================================================\n";
        auto task = make_shared<XVideoTask>();
        cout << "请输入命令（v e l）:";
        string cmd;
        cin >> cmd;
        if (cmd == "e")
            break;
        else if (cmd == "l")
        {
            cout << "task run count = " << pool.task_run_count() << endl;
            continue;
        }
        cout << "视频源：";
        cin >> task->in_path;
        cout << "目标：";
        cin >> task->out_path;
        cout << "输出宽：";
        cin >> task->width;
        cout << "输出高：";
        cin >> task->height;
        pool.AddTask(task);
    }
    pool.Stop();


    //auto vtask1 = make_shared<XVideoTask>();
    //vtask1->in_path = "test.mp4";
    //vtask1->out_path = "640.mp4";
    //vtask1->width = 640;
    //vtask1->height = 480;
    //pool.AddTask(vtask1);
    //vtask1->GetReturn();

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
