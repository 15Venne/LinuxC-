
#include<stdio.h>
#include<errno.h>
#include<pthread.h>
#include<unistd.h>

#include<iostream>
#include<queue>
using std::queue;
using std::cout;
using std::endl;


class Noncopyable
{
protected:
    Noncopyable(){  }
    ~Noncopyable(){  }

    Noncopyable(const Noncopyable &) = delete ;
    Noncopyable &operator=(const Noncopyable &) = delete ;
};//

class Thread
: public Noncopyable
{
public:
    Thread();
    virtual ~Thread();

    void start();
    void join();

private:
    virtual void run() = 0;
    static void *threadFunc(void *);
private:
    pthread_t _pthid;
    bool _isRunning;
};//

//以下实现 Thread类内方法
//
Thread::Thread()
: _pthid(0), _isRunning(false)
{
    cout << "Thread()" <<  endl;
}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
    }
    cout << "~Thread()" << endl;
}

void Thread::start()
{
    pthread_create(&_pthid, NULL, threadFunc, this);
    _isRunning = true;
}

void *Thread::threadFunc(void *p)
{
    Thread *pt = static_cast<Thread*>(p);
    pt->run();
    return nullptr;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid, NULL);
    }
}
//----------------------------------------------------
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
    pthread_mutex_t _mutex;
    bool _isLocking;
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
    MutexLock & _mutex;
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
//---------------------------------------------------
//条件变量
class Condition
: Noncopyable
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
};

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
        perror("pthread_cond_destroy");
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
//------------------------------------------------------------------
//任务队列 TaskQueue
class TaskQueue
{
public:
    TaskQueue(size_t);//1
    bool full() const;//2
    bool empty() const;//3
    void push(int);//4
    int pop();//5
private:
    size_t _sz;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    queue<int> _que;
};//

TaskQueue::TaskQueue(size_t sz)
: _sz(sz), _mutex(), _notEmpty(_mutex), _notFull(_mutex) 
{  }//1

bool TaskQueue::full() const
{
    return _que.size() == _sz;
}//2

bool TaskQueue::empty() const
{
    return _que.empty();
}//3

void TaskQueue::push(int num)
{
    {
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        _notFull.wait();
    }
    
    _que.push(num);
    }//
    _notEmpty.notify();
}//4

int TaskQueue::pop()
{
    int ret;
    {
    MutexLockGuard autoLock(_mutex);
    while(empty())
    {
        _notEmpty.wait();
    }
    ret = _que.front();
    _que.pop();
    }//
    _notFull.notify();
    return ret;
}//5
//---------------------------------------------------------
//生产者producer
class Producer
: public Thread
{
public:
    Producer(TaskQueue &taskQue)
    : _taskQue(taskQue)
    {  }
private:
    void run();//1
    TaskQueue &_taskQue;
};//

void Producer::run()
{
    ::srand(::clock());
    while(1)
    {
        int number = ::rand() % 100;
        _taskQue.push(number);
        cout << ">> Producer thread " << pthread_self()
             << ": produce a number = " << number << endl;
        ::sleep(1);
    }
}//1
//--------------------------------------------------------------
//消费者Consumer
class Consumer
: public Thread
{
public:
    Consumer(TaskQueue &taskQue)
    : _taskQue(taskQue)
    {  }
private:
    void run();//1
    TaskQueue &_taskQue;
};//

void Consumer::run()
{
    while(true)
    {
        int number = _taskQue.pop();
        cout << ">> Consumer thread " << pthread_self()
             << " : consume a number = " << number << endl;
        ::sleep(2);
    }
}




