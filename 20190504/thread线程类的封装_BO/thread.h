

#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include"noncopyable.h"
#include<pthread.h>
#include<functional>

namespace wd
{

class Thread
: Noncopyable
{
public:
    Thread(std::function<void()> && cb);

    void start();
    void join();

    ~Thread();

private:
    static void *threadFunc(void *);
private:
    pthread_t _pthid;
    std::function<void()> _cb;
    bool _isRunning;
};//

}//end of namespace wd

#endif
