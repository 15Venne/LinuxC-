
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include"Noncopy.h"

#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<errno.h>

#include<functional>
#include<queue>
#include<memory>
#include<vector>

using std::function;
using std::queue;
using std::vector;
using std::unique_ptr;

//--------------------------------------------------------------------------
//锁
class MutexLock
: public Noncopyable
{
public:
    MutexLock()
    : _isLocking(false)
    {
        if(pthread_mutex_init(&_mutex, NULL))
        {
            perror("pthread_mutex_init");
        }
    }
    ~MutexLock()
    {
        if(pthread_mutex_destroy(&_mutex))
        {
            perror("pthread_mutex_destroy");
        }
    }
    void lock()
    {
        if(pthread_mutex_lock(&_mutex))
        {
            perror("pthread_mutex_lock");
            return;
        }
        _isLocking = true;
    }
    void unlock()
    {
        if(pthread_mutex_unlock(&_mutex))
        {
            perror("pthread_mutex_unlock");
            return;
        }
        _isLocking = false;
    }
    pthread_mutex_t *getMutexPtr()
    {
        return &_mutex;
    }
private:
    bool _isLocking;
    pthread_mutex_t _mutex;
};//

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
//-------------------------------------------------------------------------------------
//Condition
class Condition
{
public:
    Condition(MutexLock &mu)
    : _mutex(mu)
    {  
        if(pthread_cond_init(&_cond, NULL))
        {
            perror("pthread_cond_init");
        }
    }
    ~Condition()
    {
        if(pthread_cond_destroy(&_cond))
        {
            perror("pthread_cond_destroy");
        }
    }
    void wait()
    {
        if(pthread_cond_wait(&_cond, _mutex.getMutexPtr()))
        {
            perror("pthread_cond_wait");
        }
    }
    void notify()
    {
        if(pthread_cond_signal(&_cond))
        {
            perror("pthread_cond_signal");
        }
    }
    void notifyAll()
    {
        if(pthread_cond_broadcast(&_cond))
        {
            perror("pthread_cond_broadcast");
        }
    }
private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};//
//-----------------------------------------------------------------------------------
//Thread 线程
class Thread
: Noncopyable
{
public:
    Thread(function<void()> &&cb)
    : _pthid(0)
    , _cb(std::move(cb))
    , _isRunning(false)
    {  }
    void start()
    {
        pthread_create(&_pthid, NULL, threadFunc, this);
        _isRunning = true;
    }
    void join()
    {
        if(_isRunning)
        {
            pthread_join(_pthid, NULL);
        }
    }
    ~Thread()
    {
        if(_isRunning)
        {
            pthread_detach(_pthid);
        }
    }
private:
    static void *threadFunc(void *);//1
private:
    pthread_t _pthid;
    function<void()> _cb;
    bool _isRunning;
};

void *Thread::threadFunc(void *p)
{
    Thread *pt = static_cast<Thread*>(p);
    if(pt)
    {
        pt->_cb();
    }
    return nullptr;
}
//---------------------------------------------------------------------------------
//TaskQueue
using Taskfun = std::function<void()>;
class TaskQueue
{
public:
    TaskQueue(size_t sz);//1
    bool empty() const;//2
    bool full() const;//3
    void push(Taskfun);//4
    Taskfun pop();//5
    void wakeup();//6
private:
    size_t _sz;
    queue<Taskfun> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
};//

TaskQueue::TaskQueue(size_t sz)
: _sz(sz)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{  }//1

bool TaskQueue::empty() const
{
    return _que.empty();
}//2
bool TaskQueue::full() const
{
    return (_que.size() == _sz);
}//3

void TaskQueue::push(Taskfun t)
{
    {
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        _notFull.wait();
    }
    _que.push(t);
    }
    _notEmpty.notify();
}//4

Taskfun TaskQueue::pop()
{
    Taskfun task;
    {
    MutexLockGuard autoLock(_mutex);
    while(_flag && empty())
    {
        _notEmpty.wait();
    }
    if(_flag)
    {
        task = _que.front();
        _que.pop();
        _notFull.notify();
    }else{
        return nullptr;
    }
    }
    return task;
}//5
void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}//6
//------------------------------------------------------------------------------------
//线程池Threadpool
class Threadpool
{
public:
    Threadpool(size_t, size_t);//1
    void start();//2
    void stop();//3
    void addTask(Taskfun &&);//4
private:
    void threadFunc();//5
    Taskfun getTask();//6
private:
    size_t _threadNum;
    size_t _queSize;
    TaskQueue _taskQue;
    bool _isExit;
    vector<unique_ptr<Thread>> _threads;
};//
Threadpool::Threadpool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{ 
    _threads.reserve(_threadNum) ;  
}//1

void Threadpool::start()
{
    for(size_t idx = 0; idx < _threadNum; ++idx)
    {
        unique_ptr<Thread> pthread(new Thread(std::bind(&Threadpool::threadFunc, this)));
        _threads.push_back(std::move(pthread));
    }
    for(auto & t : _threads)
    {
        t->start();
    }
}//2
void Threadpool::stop()
{
    if(!_isExit)
    {
        while(!_taskQue.empty())
        {
            ::sleep(1);
        }

        _isExit = true;
        _taskQue.wakeup();

        for(auto & t : _threads)
        {
            t->join();
        }
    }
}//3
void Threadpool::addTask(Taskfun &&t)
{
    _taskQue.push(t);
}//4
void Threadpool::threadFunc()
{
    while(!_isExit)
    {
        Taskfun task = getTask();
        if(task)
        {
            task();
        }
    }
}//5
Taskfun Threadpool::getTask()
{
    return _taskQue.pop();
}//6
//-------------------------------------------------------------------------------------










#endif
