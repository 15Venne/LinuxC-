



#include<stdio.h>
#include<pthread.h>
#include<unistd.h>


#include<iostream>
#include<functional>
#include<queue>

using std::cout;
using std::endl;
using std::function;
using std::queue;

class Noncopyable
{
protected:
    Noncopyable()
    {  }
    ~Noncopyable()
    {  }
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable operator=(const Noncopyable&) = delete;
};//
//---------------------------------------------------------
//Thread 供绑定类
class Thread
: public Noncopyable
{
public:
    Thread(function<void()> &&);//1
    ~Thread();//2

    void start();//3
    void join();//4
private:
    static void *threadFunc(void *);//5
    pthread_t _pthid;
    function<void()> _cb;
    bool _isRunning;
};//

Thread::Thread(function<void()> &&cb)
: _pthid(0), _cb(std::move(cb)), _isRunning(false) 
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
    pt->_cb();
    return nullptr;
}//5
//-----------------------------------------------
//锁
class MutexLock
: public Noncopyable
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
    pthread_mutex_t _mutex;
    bool _isLocking;
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
        perror("pthread_mutex_init");
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
    MutexLockGuard(MutexLock &);//1
    ~MutexLockGuard();//2
private:
    MutexLock &_mutex;
};//

MutexLockGuard::MutexLockGuard(MutexLock &mu)
: _mutex(mu)
{
    _mutex.lock();
}//1
MutexLockGuard::~MutexLockGuard()
{
    _mutex.unlock();
}//2
//-------------------------------------------------------
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
    pthread_cond_t _cond;
    MutexLock &_mutex;
};//

Condition::Condition(MutexLock &mu)
: _cond(), _mutex(mu)
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
//--------------------------------------------------------------
//任务队列
class TaskQueue
{
public:
    TaskQueue(size_t);//1
    bool empty() const;//2
    bool full() const;//3
    void push(int);//4
    int pop();//5
private:
    size_t _sz;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    queue<int> _que;
};//

TaskQueue::TaskQueue(size_t sz)
: _sz(sz), _mutex(), _notFull(_mutex), _notEmpty(_mutex)
{
    
}//1

bool TaskQueue::empty() const
{
    return _que.empty();
}//2

bool TaskQueue::full() const
{
    return _que.size() == _sz;
}//3

void TaskQueue::push(int val)
{
    {
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        _notFull.wait();
    }
    _que.push(val);
    }
    _notEmpty.notify();
}//4

int TaskQueue::pop()
{
    int ret;
    {
    MutexLockGuard autoLock(_mutex) ;
    while(empty())
    {
        _notEmpty.wait();
    }
    ret = _que.front();
    _que.pop();
    }
    _notFull.notify();
    return ret;
}//5
//---------------------------------------------------------

