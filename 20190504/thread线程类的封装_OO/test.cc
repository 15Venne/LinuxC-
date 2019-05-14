

#include"thread.h"

#include<unistd.h>
#include<iostream>
#include<memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class Mythread
: public wd::Thread
{
    void run()
    {
        ::srand(::clock());
        while(true)
        {
            int number = ::rand() % 100;
            cout << ">> sub thread " << pthread_self()
                 << " get a number: " << number << endl;
            ::sleep(1);
        }
    }
};//

int main()
{
    cout << ">> main Thread " << pthread_self() << endl;
    unique_ptr<wd::Thread> thread(new Mythread());

    thread->start();
    thread->join();

    return 0;
}
