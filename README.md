# thread_study

一个循序渐进的 C++ 多线程学习仓库。项目从 `std::thread` 的创建与回收开始，逐步覆盖参数传递、类封装、互斥锁、读写锁、RAII、条件变量、异步返回值、线程池、并行算法和 C++20 `std::barrier`。

仓库中的每个编号目录都是一个独立的 Visual Studio 控制台工程。建议按照编号顺序学习，并在调试器中观察线程 ID、构造与析构顺序、锁的持有状态以及线程退出过程。

## 你可以学到什么

- `std::thread` 对象与操作系统线程之间是什么关系。
- 为什么可连接线程在析构前必须调用 `join()` 或 `detach()`。
- 线程参数为什么默认会复制，什么时候必须使用 `std::ref()`。
- 裸指针、引用和对象在线程中的生命周期风险。
- `mutex`、`timed_mutex`、`recursive_mutex`、`shared_mutex` 的使用场景。
- 如何使用 `lock_guard`、`unique_lock`、`shared_lock`、`scoped_lock` 管理锁。
- 条件变量如何代替轮询，避免线程无意义地反复检查队列。
- `promise/future`、`packaged_task`、`async` 的区别。
- 一个固定线程池如何组织任务队列、工作线程、退出通知和返回值。
- 如何把 CPU 密集型任务切片到多个线程，以及如何同步多个执行阶段。

## 仓库结构

```text
thread_study/
├── 101first_thread/                 第一个线程
├── 102thread_detach/                join、detach 与线程退出
├── 103thread_para/                  线程参数复制、指针和引用
├── 104thread_class/                 面向对象的线程封装
├── 105thread_lambda/                Lambda 线程入口
├── 106thread_mutex/                 多种互斥锁
├── 107thread_shared/                共享读锁和独占写锁
├── 108thread_RAII/                  RAII 锁与死锁处理
├── 109thread_msg_server/            轮询式消息线程
├── 110condition_variable/           条件变量
├── 111thread_msg_server_condition/  条件变量消息线程
├── 112promise_future/               promise 与 future
├── 113packaged_task/                packaged_task
├── 114async_thread/                 async 启动策略
├── 115base16_thread_c++17/          多线程 Base16 编码
├── 116thread_pool/                  通用线程池
├── 119thread_pool_video/            FFmpeg 转码线程池
└── 120barrier_thread_c++20/         C++20 barrier
```

## 构建环境

当前工程主要面向：

- Windows 10/11
- Visual Studio
- MSVC 平台工具集 `v145`
- C++20
- `x64` 平台

每个目录都有独立的 `.slnx` 和 `.vcxproj`。例如：

```text
103thread_para/103thread_para.slnx
110condition_variable/110condition_variable.slnx
116thread_pool/116thread_pool.slnx
120barrier_thread_c++20/120barrier_thread_c++20.slnx
```

在 Visual Studio 中：

1. 打开目标目录中的 `.slnx`。
2. 选择 `Debug` 和 `x64`。
3. 使用 `F5` 调试，或使用 `Ctrl+F5` 直接运行。
4. 如果工程提示没有 `v145`，在“项目属性 -> 常规 -> 平台工具集”中选择本机已安装的工具集。

也可以在 Visual Studio Developer PowerShell 中构建：

```powershell
msbuild 103thread_para\101first_thread.vcxproj /p:Configuration=Debug /p:Platform=x64
```

`120barrier_thread_c++20` 必须使用支持 C++20 `std::barrier` 的编译器和标准库。

## 示例总览

| 示例 | 核心内容 | 运行时重点观察 |
| --- | --- | --- |
| [`101first_thread`](101first_thread) | `std::thread`、线程 ID、`sleep_for()`、`join()` | 主线程在 `join()` 处等待，子线程结束后才继续 |
| [`102thread_detach`](102thread_detach) | `join()`、`detach()`、停止标记 | 分离线程不再受 `thread` 对象管理，主进程退出时它可能尚未完成 |
| [`103thread_para`](103thread_para) | 参数复制、裸指针、引用、`std::ref()` | `Para` 的复制和析构次数，以及对象是否活到线程结束 |
| [`104thread_class`](104thread_class) | 成员函数线程入口、抽象基类、`Start/Stop/Wait` | `Stop()` 设置退出标记后仍调用 `Wait()` 的原因 |
| [`105thread_lambda`](105thread_lambda) | Lambda、捕获 `this`、线程参数 | Lambda 参数仍由 `std::thread` 保存，捕获的对象必须保持有效 |
| [`106thread_mutex`](106thread_mutex) | `mutex`、`try_lock`、`timed_mutex`、`recursive_mutex` | 竞争、等待、超时和递归加锁的差异 |
| [`107thread_shared`](107thread_shared) | `shared_timed_mutex`、共享锁、独占锁 | 多个读线程可以并行，写线程必须独占 |
| [`108thread_RAII`](108thread_RAII) | 自定义 RAII、`lock_guard`、`unique_lock`、`scoped_lock` | 作用域结束自动解锁，以及多锁死锁问题 |
| [`109thread_msg_server`](109thread_msg_server) | 消息队列、互斥锁、轮询 | 工作线程每 10ms 检查一次队列，会产生无效唤醒 |
| [`110condition_variable`](110condition_variable) | `wait()`、谓词、`notify_one()` | `wait()` 会先释放锁，被唤醒后重新获得锁 |
| [`111thread_msg_server_condition`](111thread_msg_server_condition) | 条件变量改造消息服务 | `Stop()` 必须通知条件变量，否则线程可能永远睡眠 |
| [`112promise_future`](112promise_future) | 在线程间设置和获取结果 | `future.get()` 会阻塞到 `promise.set_value()` |
| [`113packaged_task`](113packaged_task) | 可调用对象包装、`wait_for()` | 任务本身可在线程中运行，结果通过 future 获取 |
| [`114async_thread`](114async_thread) | `launch::deferred` 与异步执行 | deferred 任务直到 `get()` 才在调用者线程执行 |
| [`115base16_thread_c++17`](115base16_thread_c++17) | 手工切片、硬件并发数、并行算法 | 单线程、手工多线程、`execution::par` 的耗时差异 |
| [`116thread_pool`](116thread_pool) | 工作线程、任务队列、条件变量、返回值 | 任务入队、唤醒、执行、`promise` 返回结果和停止流程 |
| [`119thread_pool_video`](119thread_pool_video) | 在线程池中调用 FFmpeg | 多个外部转码进程并行执行，日志按线程 ID 分文件 |
| [`120barrier_thread_c++20`](120barrier_thread_c++20) | 阶段同步 | 所有参与者到达屏障后，才会一起进入下一阶段 |

## 101：创建、等待和回收线程

`101first_thread` 的主线程先打印自己的线程 ID，然后创建子线程执行 `ThreadMain()`：

```cpp
std::thread th(ThreadMain);
th.join();
```

执行逻辑：

1. `std::thread` 构造时启动新线程。
2. 主线程和子线程从此并发执行，谁先打印并没有固定保证。
3. 主线程调用 `join()` 后阻塞。
4. 子线程循环十次，每次休眠一秒。
5. 子线程函数返回，操作系统线程结束。
6. `join()` 返回，主线程继续执行。

`join()` 阻塞的是调用它的线程，不是整个进程。其他线程仍然可以继续运行。

## 102：`join()`、`detach()` 与析构规则

一个 `std::thread` 满足以下条件之一时称为“可连接”：

- 它已经启动一个线程；
- 尚未执行 `join()`；
- 尚未执行 `detach()`；
- 尚未被移动到另一个 `std::thread`。

可连接的 `std::thread` 如果直接析构，标准库会调用 `std::terminate()`，常见表现是：

```text
abort() has been called
```

两种处理方式：

| 方法 | 含义 | 主要风险 |
| --- | --- | --- |
| `join()` | 当前线程等待目标线程结束并回收资源 | 目标线程不退出时，调用者会一直等待 |
| `detach()` | 线程脱离 `std::thread` 对象独立运行 | 无法再等待它，参数和宿主对象可能提前销毁 |

示例中的 `is_exit` 用于通知循环退出，但它是普通 `bool`。一个线程写、另一个线程读而没有同步，在标准 C++ 中属于数据竞争。更严谨的实现应使用 `std::atomic<bool>`，或者在同一把互斥锁保护下访问。

## 103：线程参数为什么会复制

`std::thread` 默认不会把调用表达式中的局部变量直接交给新线程使用。它会先把函数和参数保存到线程内部的调用状态中，通常按值进行退化复制或移动。

```cpp
Para p;
std::thread th(ThreadMain, 101, 12.1f, "test", p);
```

这里大致经历：

1. 主线程创建局部对象 `p`。
2. `std::thread` 把 `p` 复制到自己的内部存储，确保启动过程不依赖原参数表达式。
3. 新线程真正调用 `ThreadMain(..., Para p4)` 时，函数形参 `p4` 本身也是一个按值对象，因此可能再次从内部存储移动或复制构造。
4. `ThreadMain()` 返回后销毁形参。
5. 线程内部保存的参数对象销毁。
6. 原始局部变量在自己的作用域结束时销毁。

实际复制次数可能受到标准库实现、移动构造函数和编译器优化影响，不应把某次输出的固定次数当成语言保证。

### 为什么引用必须使用 `std::ref`

函数声明是：

```cpp
void ThreadMainRef(Para& p);
```

启动时应写：

```cpp
std::thread th(ThreadMainRef, std::ref(p));
```

`std::ref(p)` 创建一个 `reference_wrapper<Para>`。它可以按值安全地存入线程内部，但调用函数时又会转换回 `Para&`。如果直接传 `p`，`std::thread` 仍尝试保存一份值，无法按预期绑定到非常量左值引用。

使用引用并不延长 `p` 的生命周期，所以示例仍然要 `join()`，保证 `p` 在线程访问期间没有离开作用域。

### 裸指针和 `detach()` 的危险组合

```cpp
Para p;
std::thread th(ThreadMainPtr, &p);
th.detach();
```

`&p` 只是地址。线程分离后，当前作用域可能先结束并销毁 `p`，子线程随后访问该地址就会产生悬空指针和未定义行为。休眠时间不能提供生命周期保证。

## 104：把线程封装成类

`XThread` 定义了统一生命周期：

```text
Start()
  ├── is_exit_ = false
  └── 创建线程执行虚函数 Main()

Stop()
  ├── is_exit_ = true
  └── Wait()

Wait()
  └── 如果 joinable()，调用 join()
```

`Main()` 是私有纯虚函数：

```cpp
private:
    virtual void Main() = 0;
```

这不会阻止子类重写它。C++ 的访问控制检查“能否通过当前表达式直接调用成员”，不限制虚函数重写。把它设为私有表示外部代码不能随意调用线程入口，只能通过 `Start()` 启动；子类仍然可以提供 `override`。

纯虚函数使 `XThread` 成为抽象类，无论它位于 `public`、`protected` 还是 `private`，都不能直接实例化。访问级别和“是否为抽象类”是两个独立概念。

`Stop()` 不能只设置标记后立即返回，因为对象可能紧接着析构，而工作线程仍在 `sleep_for()` 或仍在访问 `this`。`Wait()` 保证线程函数已经真正返回，宿主对象才可以安全销毁。

当前示例中的 `is_exit_` 同样是普通 `bool`，严格实现应改成原子变量或使用同步保护。

## 106 到 108：锁与 RAII

### 互斥锁对照

| 类型 | 特点 | 示例用途 |
| --- | --- | --- |
| `std::mutex` | 同一时刻只能由一个线程持有 | 保护输出和普通临界区 |
| `std::timed_mutex` | 支持 `try_lock_for()` 和 `try_lock_until()` | 等待锁时设置超时 |
| `std::recursive_mutex` | 同一线程可以重复加锁 | 外层函数持锁后调用同样需要该锁的内层函数 |
| `std::shared_timed_mutex` | 共享读、独占写 | 多读少写场景 |

不要在生产代码中用 `sleep_for()` 猜测锁是否已经释放。休眠只适合演示调度效果。

### RAII 锁对象

| 类型 | 能否手动解锁 | 典型用途 |
| --- | ---: | --- |
| `lock_guard` | 否 | 最简单的作用域独占锁 |
| `unique_lock` | 是 | 条件变量、延迟加锁、临时释放 |
| `shared_lock` | 是 | 获取共享读锁 |
| `scoped_lock` | 否 | 一次安全锁住多把互斥锁 |

`adopt_lock` 表示调用者已经持有锁，RAII 对象只负责接管和释放；使用时如果实际上没有持锁，行为不正确。`defer_lock` 表示先创建管理对象但暂不加锁；`try_to_lock` 表示尝试加锁但不阻塞。

`108thread_RAII` 同时保留了死锁演示代码。两个线程以不同顺序分别锁 `mux1` 和 `mux2` 时可能形成循环等待。`std::scoped_lock(mux1, mux2)` 会使用避免死锁的算法一次管理多把锁。

## 109 到 111：消息队列与条件变量

### 轮询版本

`109thread_msg_server` 的发送线程把字符串压入 `msgs_`，工作线程每隔 10ms 醒来检查：

```text
SendMsg()
  └── 加锁 -> push_back -> 解锁

Main()
  └── sleep 10ms -> 加锁 -> 检查队列 -> 逐条处理
```

优点是逻辑直观；缺点是即使没有消息，线程也会不断醒来。

### 条件变量版本

`111thread_msg_server_condition` 使用：

```cpp
cv_.wait(lock, [this] {
    return is_exit() || !msgs_.empty();
});
```

其内部语义是：

1. 调用者先持有 `unique_lock`。
2. 如果谓词为假，`wait()` 原子地释放互斥锁并休眠。
3. `SendMsg()` 入队后调用 `notify_one()`。
4. 等待线程被唤醒，并在返回前重新获得互斥锁。
5. 再次检查谓词，处理虚假唤醒。

`Stop()` 除了设置退出标记，还必须 `notify_all()`。否则工作线程可能正在 `wait()` 中，没有新消息就不会醒来，自然也无法观察退出标记，随后 `Wait()` 会永久阻塞。

## 112 到 114：三种异步结果模型

| 工具 | 谁提供可调用对象 | 谁决定执行线程 | 结果对象 |
| --- | --- | --- | --- |
| `promise/future` | 调用者自己编写线程函数并持有 promise | 调用者显式创建 `thread` | `future` |
| `packaged_task` | `packaged_task` 包装函数 | 可直接调用，也可移动到线程中 | `future` |
| `async` | 直接传函数和参数 | 标准库根据启动策略决定 | `future` |

### `promise/future`

`promise.set_value()` 写入一次结果，`future.get()` 阻塞读取一次结果。`promise` 不可复制，所以传入线程时使用 `std::move(p)`。

### `packaged_task`

`packaged_task<string(int)>` 把 `TestPack(int)` 包装成一个任务。任务执行时自动把返回值写入共享状态。`wait_for()` 可检查 `ready`、`timeout` 或 `deferred`。

### `async`

- `launch::deferred`：不创建新线程，直到调用 `get()` 或 `wait()` 才在当前线程执行。
- `launch::async`：要求异步执行。
- 未指定策略：实现可以选择 async 或 deferred，不能依赖它一定创建新线程。

## 115：Base16 并行编码

每个输入字节拆成高四位和低四位，再分别查表得到两个十六进制字符：

```text
输入 1 字节:  1010 1111
高四位索引:  1010 -> 'a'
低四位索引:  1111 -> 'f'
输出 2 字节:  "af"
```

示例比较三种实现：

1. 单线程顺序循环。
2. 根据 `hardware_concurrency()` 手工切片，每个线程处理不同区间。
3. 使用 C++17 `std::for_each(std::execution::par, ...)`。

手工切片时最后一个线程负责余数。`hardware_concurrency()` 只是提示值，标准允许返回 `0`；通用代码在除法前应当处理该情况。性能测试还应使用 Release 构建、多次运行和更稳定的基准方法。

## 116：线程池执行流程

核心对象：

| 对象 | 责任 |
| --- | --- |
| `XTask` | 定义纯虚函数 `Run()`，保存退出查询函数和返回值 promise |
| `XThreadPool` | 保存工作线程、任务队列、互斥锁、条件变量和退出状态 |
| `MyTask` | 示例业务任务，重写 `Run()` |

任务流程：

```text
主线程 AddTask(shared_ptr<XTask>)
        |
        v
互斥锁保护下 push_back 到 tasks_
        |
        v
cv_.notify_one()
        |
        v
某个工作线程从 GetTask() 醒来并取出队首任务
        |
        v
task->Run()
        |
        v
task->SetValue(result)
        |
        v
调用者通过 GetReturn() 获取结果
```

使用 `shared_ptr<XTask>` 的目的是让任务从提交者作用域转移到队列和工作线程后仍然有效。任务内部的 `is_exit` 函数对象被设置为 Lambda，它会查询线程池的退出状态。

`Stop()` 的流程是设置退出标记、通知所有线程、逐个 `join()`，最后清空线程容器。当前实现是教学版本：退出标记是普通 `bool`，异常被空 `catch` 吞掉，停止时队列中尚未执行的任务不会完成其 promise，重复启动也没有完整复位逻辑。

## 119：FFmpeg 视频转码任务

程序创建 16 个工作线程，然后循环读取命令：

```text
v  创建一个视频转码任务
l  查看当前正在运行的任务数量
e  退出并停止线程池
```

每个 `XVideoTask` 最终拼接并执行类似命令：

```text
ffmpeg.exe -y -i input.mp4 -s 640x480 output.mp4 >线程ID.txt 2>&1
```

运行要求：

1. 安装 FFmpeg。
2. 确保 `ffmpeg.exe` 位于 `PATH` 或程序工作目录。
3. 输入路径和输出路径必须存在且可访问。
4. 输出文件不能被其他程序独占。

当前命令通过 `system()` 执行，路径没有自动加引号。包含空格或命令控制字符的路径可能失败，也存在命令注入风险，因此只能作为本地学习示例。

## 120：`std::barrier`

示例创建计数为 3 的屏障和三个线程。每个线程先按编号休眠不同时间，然后调用：

```cpp
bar.arrive_and_wait();
```

前两个到达的线程会等待，直到第三个线程也到达。计数归零后，三个线程一起进入下一阶段。`barrier` 可重复用于多个阶段；只需要一次性等待时，也可以了解 `std::latch`。

示例把线程 `detach()` 后依靠 `getchar()` 保持进程存活，这只是便于观察。更可靠的代码应保存线程并逐个 `join()`。

## 常见问题

### `abort() has been called`

最常见原因是一个仍然 `joinable()` 的 `std::thread` 被析构。检查所有返回路径和异常路径，确保线程最终被 `join()` 或 `detach()`。

### `sleep_for` 没有匹配的模板实例

需要传入 `std::chrono::duration`，例如：

```cpp
using namespace std::chrono_literals;
std::this_thread::sleep_for(100ms);
std::this_thread::sleep_for(3s);
```

或者：

```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(100));
```

### 虚函数出现“无法解析的外部符号”

声明普通虚函数：

```cpp
virtual void Main();
```

意味着它有一个定义，链接器需要找到 `Test::Main()` 的函数体。若只想要求子类实现，应写成纯虚函数：

```cpp
virtual void Main() = 0;
```

### 日志顺序每次不同

线程调度顺序没有固定保证。即使两个线程依次创建，也不能据此推断谁先执行。多线程同时写 `cout` 时，文本还可能互相穿插。

### `Stop()` 后为什么还要 `Wait()`

停止标记只是“发出请求”，并不表示线程已经退出。线程可能正在休眠、持锁、处理任务或刚好尚未读取标记。`Wait()` 才建立“线程已经结束”的确定时序。

## 推荐学习路线

### 第一阶段：线程生命周期

1. `101first_thread`
2. `102thread_detach`
3. `103thread_para`
4. `104thread_class`
5. `105thread_lambda`

重点掌握 `joinable()`、对象生命周期、参数复制和 `this` 指针安全。

### 第二阶段：同步

1. `106thread_mutex`
2. `107thread_shared`
3. `108thread_RAII`
4. `110condition_variable`

重点掌握临界区、锁粒度、死锁条件和条件变量谓词。

### 第三阶段：线程间通信

1. `109thread_msg_server`
2. `111thread_msg_server_condition`
3. `112promise_future`
4. `113packaged_task`
5. `114async_thread`

对比消息队列和共享状态两类通信方式。

### 第四阶段：并行任务系统

1. `115base16_thread_c++17`
2. `116thread_pool`
3. `119thread_pool_video`
4. `120barrier_thread_c++20`

重点理解任务划分、任务所有权、工作线程停止和阶段同步。

## 建议实验

- 给 `Para` 增加移动构造函数，观察复制和移动输出如何变化。
- 把 `103thread_para` 中的按值参数改成 `const Para&` 并使用 `std::cref()`。
- 把退出标记改成 `std::atomic<bool>`，比较代码语义。
- 删除 `111` 中 `Stop()` 的 `notify_all()`，观察为什么会卡在 `join()`。
- 为线程池增加带谓词的 `cv_.wait()`，处理虚假唤醒。
- 为线程池增加剩余任务处理策略：立即取消或排空队列后退出。
- 给 `barrier` 增加完成函数，观察每一阶段由哪个线程执行完成回调。

## 代码边界

这些程序用于学习，并非生产级并发库。部分示例主动保留了裸指针、分离线程、普通 `bool` 跨线程访问、无限循环和简化异常处理，目的是观察问题。把代码用于真实项目之前，需要补充原子状态、异常传播、线程安全析构、取消协议、任务上限、日志同步和压力测试。
