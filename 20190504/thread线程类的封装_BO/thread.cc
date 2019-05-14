

#include"thread.h"
#include<iostream>

using std::cout;
using std::endl;

namespace wd
{

Thread::Thread(std::function<void()> &&cb)
: _pthid(0), _cb(std::move(cb)), _isRunning(false)
{
    cout << "Thread(std::function<void() &&>)" << endl;
}

void Thread::start()
{
    pthread_create(&_pthid, NULL, threadFunc, this);
    _isRunning = true;
}

void *Thread::threadFunc(void *p)
{
    Thread *pt = static_cast<Thread*>(p);
    //if(pt)
    //{
        pt->_cb();
    //}
    return nullptr;
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
        pthread_detach(_pthid);
    }
    cout << "~Thread()" << endl;
}


}//end of namespace wd
