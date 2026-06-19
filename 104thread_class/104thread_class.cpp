// 104thread_class.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <thread>
#include <iostream>
#include <string>
#include <chrono>
using namespace std::chrono_literals;

//Linux -lpthread
using namespace std;

class MyThread
{
public:
    //入口线程函数
    void Main()
    {
        cout << "MyThread Main " << name << ":" << age;
    }
    string name;
    int age = 100;
};
class XThread
{
public:
    virtual void Start()
    {
        is_exit_ = false;
        th_ = std::thread(&XThread::Main, this);
    }
    virtual void Stop()
    {
        is_exit_ = true;
        Wait();
    }
    virtual void Wait()
    {
        if (th_.joinable())
            th_.join();
    }
    bool is_exit() { return is_exit_; }
private:
    virtual void Main() = 0;
    std::thread th_;
    bool is_exit_ = false;
};
class TestXThread :public XThread
{
public:
    void Main() override
    {
        cout << "TestXThread Main begin" << endl;
        while (!is_exit())
        {
            this_thread::sleep_for(100ms);
            cout << "." << flush;
        }
        cout << "TestXThread Main end" << endl;
    }
    string name;
};

int main(int argc, char* argv[])
{
    TestXThread testth;
    testth.name = "TestXThread name ";
    testth.Start();
    this_thread::sleep_for(1s);
    testth.Stop();

    testth.Wait();
    getchar();

    MyThread myth;
    myth.name = "Test name 001";
    myth.age = 20;
    thread th(&MyThread::Main, &myth);
    th.join();

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
