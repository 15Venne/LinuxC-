

#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

#include<iostream>
#include<queue>
#include<memory>
#include<vector>

using std::cout;
using std::endl;
using std::queue;
using std::unique_ptr;
using std::vector;

class Noncopyable
{
protected:
    Noncopyable(){}
    ~Noncopyable(){}

    Noncopyable(const Noncopyable &) = delete;
    Noncopyable operator=(const Noncopyable &) = delete;
};//
//---------------------------------------------------------
//锁
class MutexLock
: Noncopyable
{
public:
    MutexLock();//1
    ~MutexLock();//2
    void lock();//3
    void unlock();//4
    pthread_mutex_t *getMutexLockPtr()
    {
        return &_mutex;
    }
private:
    bool _isLocking;
    pthread_mutex_t _mutex;
};//
MutexLock::MutexLock()
: _isLocking(false)
{
    if(pthread_mutex_init(&_mutex, NULL))
    {
        perror("pthread_mutex_init");
    }
}//1

MutexLock::~MutexLock()
{
    if(pthread_mutex_destroy(&_mutex))
    {
        perror("pthread_mutex_destroy");
    }
}//2
void MutexLock::lock()
{
    if(pthread_mutex_lock(&_mutex))
    {
        perror("pthread_mutex_lock");
        return;
    }
    _isLocking = true;
}//3
void MutexLock::unlock()
{
    if(pthread_mutex_unlock(&_mutex))
    {
        perror("pthread_mutex_unlock");
        return;
    }
    _isLocking = false;
}//4

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mu)
    : _mutex(mu)
    {
        _mutex.lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};//

//-----------------------------------------------------------
//条件变量
class Condition
: public Noncopyable
{
public:
    Condition(MutexLock &);//1
    ~Condition();//2
    void wait();//3
    void notify();//4
    void notifyAll();//5
private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};//

Condition::Condition(MutexLock &mu)
: _mutex(mu)
{
    if(pthread_cond_init(&_cond, NULL))
    {
        perror("pthread_cond_init");
    }
}//1
Condition::~Condition()
{
    if(pthread_cond_destroy(&_cond))
    {
        perror("pthread_cond_destroy");
    }
}//2
void Condition::wait()
{
    if(pthread_cond_wait(&_cond, _mutex.getMutexLockPtr()))
    {
        perror("pthread_cond_wait");
    }
}//3
void Condition::notify()
{
    if(pthread_cond_signal(&_cond))
    {
        perror("pthread_cond_signal");
    }
}//4
void Condition::notifyAll()
{
    if(pthread_cond_broadcast(&_cond))
    {
        perror("pthread_cond_broadcast");
    }
}//5
//---------------------------------------------------------------
//线程基类Thread
class Thread
: Noncopyable
{
public:
    Thread();//1
    virtual ~Thread();//2
    void start();//3
    void join();//4

private:
    static void *threadFunc(void*);//5
    virtual void run() = 0;
private:
    pthread_t _pthid;
    bool _isRunning;
};//

Thread::Thread()
: _pthid(0), _isRunning(false)
{  }//1
Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
    }
}//2
void Thread::start()
{
    pthread_create(&_pthid, NULL, threadFunc, this);
    _isRunning = true;
}//3
void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid, NULL);
    }
}//4
void *Thread::threadFunc(void *p)
{
    Thread *pt = static_cast<Thread*>(p);
    pt->run();
    return nullptr;
}//5
//---------------------------------------------------------------
//任务类 Task
class Task
{
public:
    virtual void process() = 0;
};//
//---------------------------------------------------------------
//任务队列 TaskQue
class TaskQue
{
public:
    TaskQue(size_t);//1
    bool empty() const;//2
    bool full() const;//3
    void push(Task*);//4
    Task *pop();//5
    void setFlag()
    {
        _flag = false;
        _notEmpty.notifyAll();
    }
private:
    size_t _sz;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
    queue<Task*> _que;
};//
TaskQue::TaskQue(size_t sz)
: _sz(sz), _mutex(), _notEmpty(_mutex), _notFull(_mutex), _flag(true)
{  }//1

bool TaskQue::empty() const
{
    return _que.empty();
}//2

bool TaskQue::full() const
{
    return _que.size() == _sz;
}//3

void TaskQue::push(Task *pt)
{
    {
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        _notFull.wait();
    }
    _que.push(pt);
    }
    _notEmpty.notify();
}//4

Task *TaskQue::pop()
{
    Task *tmp;
    {
    MutexLockGuard autoLock(_mutex);
    while(_flag && empty())
    {
        _notEmpty.wait();
    }
    tmp = _que.front();
    _que.pop();
    }
    _notFull.notify();
    return tmp;
}//5
//-------------------------------------------------------------------
class WorkerThread;
//线程池ThreadPool
class ThreadPool
{
    friend class WorkerThread;
public:
    ThreadPool(size_t, size_t);//1
    void start();//2
    void stop();//3

    void addTask(Task*);//4
private:
    void threadFunc();//5
    Task *getTask();//6
private:
    size_t _threadNum;
    size_t _queSize;
    TaskQue _que;
    bool _isExit;
    vector<unique_ptr<Thread>> _threads;
};//

//WorkerThread--------------------------------
class WorkerThread
:public Thread
{
public:
    WorkerThread(ThreadPool &tpool)
    : _threadpool(tpool)
    {  }
    
private:
    void run()
    {
        _threadpool.threadFunc();
    }
    ThreadPool & _threadpool;
};//------------------------------------------


ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _que(queSize)
, _isExit(false)
{
    _threads.reserve(_threadNum);
}//1

void ThreadPool::start()
{
    for(size_t i = 0; i!= _threadNum; ++i)
    {
        unique_ptr<Thread> t(new WorkerThread(*this));
        _threads.push_back(std::move(t));
    }
    for(auto &t : _threads)
    {
        t->start();
    }
}//2

void ThreadPool::stop()
{
    //if(!_isExit)
    while(!_isExit)
    {
        if(_que.empty()) //这里可能要加锁
        {
            _isExit = true;
            _que.setFlag();
            for(auto &t : _threads)
            {
                t->join();
            }
        } 
        sleep(1);
    }
}//3

void ThreadPool::addTask(Task *pt)
{
    _que.push(pt);
}//4

void ThreadPool::threadFunc()
{   
    while(!_isExit)
    {
        Task *t = getTask();
        if(t)
        {
            t->process();
        }
    }
}//5

Task *ThreadPool::getTask()
{
    return _que.pop();
}//6

//-----------------------------------------------------------------
//真正的任务Task
//
class MyTask
: public Task
{
public:
    void process()
    {
        srand(clock());
        int number = rand() % 100;
        cout << ">> get a number = " << number << endl;
    }
};

