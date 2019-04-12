#include<pthread.h>
#include<unistd.h>

#include<iostream>

using std::cout;
using std::endl;

class MutexLock
{
public:
    MutexLock()
    : _pmutex(new pthread_mutex_t)
    {
        pthread_mutex_init(_pmutex, NULL);
        cout << "MutexLock()" << endl;
    }

    void lock();
    void unlock();
    pthread_mutex_t *getMutex()
    {
        return _pmutex;
    }

    ~MutexLock()
    {
        if(_pmutex)
        {
            delete _pmutex;
            _pmutex = nullptr;
            cout << "delete" << endl;
        }
        cout << "~MutexLock()" << endl;
    }
private:
    pthread_mutex_t *_pmutex;
};//class MutexLock

void MutexLock::lock()
{
    cout << "lock()" << endl;
    pthread_mutex_lock(_pmutex);
}
void MutexLock::unlock()
{
    cout << "unlock()" << endl;
    pthread_mutex_unlock(_pmutex);
}

class Condition
{
public:
    Condition(MutexLock *p)
    : _pcond(new pthread_cond_t)
    , _pmutexLock(p)
    {
        pthread_cond_init(_pcond, NULL);
        cout << "Condition()" << endl;
    }

    void wait();
    void notify();
    void notifyall();
    MutexLock *getMutexLock()
    {
        return _pmutexLock;
    }

    ~Condition()
    {
        if(_pcond)
        {
            delete _pcond;
            _pcond = nullptr;
            cout << "delete" << endl;
        }
        cout << "~Condition()" << endl;
    }
private:
    pthread_cond_t *_pcond;
    MutexLock *_pmutexLock;
};//class Condition

void Condition::wait()
{
    cout << "wait" << endl;
    pthread_cond_wait(_pcond, _pmutexLock->getMutex());
}
void Condition::notify()
{
    cout << "notify()" << endl;
    pthread_cond_signal(_pcond);
}
void Condition::notifyall()
{
    cout << "notifyall()" << endl;
    pthread_cond_broadcast(_pcond);
}

void *threadFunc(void *p)
{
    Condition *pc = (Condition*)p;
    cout << "i am threadFunc" << endl;
    pc->getMutexLock()->lock();
    pc->wait();
    pc->getMutexLock()->unlock();
    cout << endl;
    pc->getMutexLock()->lock();
    pc->wait();
    pc->getMutexLock()->unlock();

    pthread_exit(NULL);
}

int main()
{
    MutexLock lock1;
    Condition cond1(&lock1); 
    lock1.lock();
    lock1.unlock();
    
    pthread_t thA;
    pthread_create(&thA, NULL, threadFunc, &cond1);
    usleep(10000);
    cout << "i am main thread ";
    cond1.notify();
    usleep(10000);
    cout << "i am main thread ";
    cond1.notifyall();
    pthread_join(thA, NULL);
    return 0;
}
