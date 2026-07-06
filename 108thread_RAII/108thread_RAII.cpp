// 108thread_RAII.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <chrono>
//Linux -lpthread
using namespace std;
using namespace std::chrono_literals;
// RAII
class XMutex
{
public:
    XMutex(mutex& mux) :mux_(mux)
    {
        cout << "Lock" << endl;
        mux.lock();
    }
    ~XMutex()
    {
        cout << "Unlock" << endl;
        mux_.unlock();
    }
private:
    mutex& mux_;
};
static mutex mux;
void TestMutex(int status)
{
    XMutex lock(mux);
    if (status == 1)
    {
        cout << "=1" << endl;
        return;
    }
    else
    {
        cout << "!=1" << endl;
        return;
    }
}

static mutex gmutex;
void TestLockGuard(int i)
{
    gmutex.lock();
    {
        //已经拥有锁，不lock
        lock_guard<mutex> lock(gmutex, adopt_lock);
        //结束释放锁
    }
    {
        lock_guard<mutex> lock(gmutex);
        cout << "begin thread " << i << endl;
    }
    for (;;)
    {
        {
            lock_guard<mutex> lock(gmutex);
            cout << "In " << i << endl;
        }
        this_thread::sleep_for(500ms);
    }
}

static mutex mux1;
static mutex mux2;

void TestScope1()
{
    //模拟死锁 停100ms等另一个线程锁mux2
    this_thread::sleep_for(100ms);
    cout << this_thread::get_id() << " begin mux1 lock" << endl;
    //mux1.lock();
    cout << this_thread::get_id() << " begin mux2 lock" << endl;
    //mux2.lock(); //死锁
    //c++11
    // lock(mux1, mux2);
    //c++17 
    scoped_lock lock(mux1, mux2); // 解决死锁

    cout << "TestScope1" << endl;
    this_thread::sleep_for(1000ms);

    //mux1.unlock();
    //mux2.unlock();
}
void TestScope2()
{
    cout << this_thread::get_id() << " begin mux2 lock" << endl;
    mux2.lock();
    this_thread::sleep_for(500ms);
    cout << this_thread::get_id() << " begin mux1 lock" << endl;
    mux1.lock();//死锁
    cout << "TestScope2" << endl;
    this_thread::sleep_for(1500ms);
    mux1.unlock();
    mux2.unlock();
}


int main(int argc, char* argv[])
{
    {
        //演示死锁情况
        {
            thread th(TestScope1);
            th.detach();
        }
        {
            thread th(TestScope2);
            th.detach();
        }
    }
    getchar();
    {
        //共享锁
        static shared_timed_mutex  tmux;
        //读取锁 共享锁
        {
            //调用共享锁 
            shared_lock<shared_timed_mutex> lock(tmux);
            cout << "read data" << endl;
            //退出栈区 释放共享锁
        }
        //写入锁 互斥锁
        {
            unique_lock<shared_timed_mutex> lock(tmux);
            cout << "write data" << endl;
        }

    }
	getchar();
    {
        static mutex mux;
        {
            unique_lock<mutex> lock(mux);
            lock.unlock();
            //临时释放锁
            lock.lock();
        }

        {
            //已经拥有锁 不锁定，退出栈区解锁
            mux.lock();
            unique_lock<mutex> lock(mux, adopt_lock);
        }
        {
            //延后加锁 不拥有 退出栈区不解锁
            unique_lock<mutex> lock(mux, defer_lock);
            //加锁 退出栈区解锁
            lock.lock();
        }
        {
            //mux.lock();
            //尝试加锁 不阻塞 失败不拥有锁
            unique_lock<mutex> lock(mux, try_to_lock);

            if (lock.owns_lock())
            {
                cout << "owns_lock" << endl;
            }
            else
            {
                cout << "not owns_lock" << endl;
            }
        }

    }

    getchar();

    for (int i = 0; i < 3; i++)
    {
        thread th(TestLockGuard, i + 1);
        th.detach();
    }
    TestMutex(1);
    TestMutex(2);

    getchar();
    return 0;
}
