


#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include"noncopyable.h"

#include<pthread.h>

namespace wd
{

class Thread
: public Noncopyable
{
public:
    Thread();
    
    void start();
    void join();

    virtual ~Thread();

private:
    virtual void run() = 0;
    static void *threadFunc(void *);//

private:
    pthread_t _pthid;
    bool _isRunning;
};//


}//end of namespace wd



#endif
