

#include"thread.h"

#include<iostream>

using std::cout;
using std::endl;

namespace wd
{

Thread::Thread()
: _pthid(0), _isRunning(false)
{
    cout << "Thread()" << endl;
}

void Thread::start()
{
    pthread_create(&_pthid, NULL, threadFunc, this);
    _isRunning = true;
}

void *Thread::threadFunc(void *p)
{
    Thread *pt = static_cast<Thread*>(p);
    if(pt)
    { pt->run(); }

    return nullptr;//
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid, NULL);
    }
}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);//
    }
    cout << "~Thread()" << endl;
}

}//end of namespace wd
