

#include"thread.h"

#include<unistd.h>
#include<iostream>
#include<memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class Mythread
{
public:
    void run()
    {
        ::srand(::clock());
        while(1)
        {
            int number = ::rand() % 100;
            cout << ">> sub thread " << pthread_self()
                 <<" get a number: " << number << endl;
            ::sleep(1);
        }
    }
};//

void process()
{
    cout << endl << "process method begin" << endl;
    srand(clock());
    while(1)
    {
        int number = rand() % 100;
        cout << ">> sub thread " << pthread_self()
             <<" get a number: " << number << endl;
        sleep(1);
    }

}

void test0()
{
    cout << "test0()" << endl;
    unique_ptr<wd::Thread> thread(new wd::Thread(
        std::bind(&Mythread::run, Mythread()) //参数二临时对象                                            
    ));

    thread->start();
    thread->join();
}

void test1()
{
    cout << endl << "test1()" << endl;
    unique_ptr<wd::Thread> thread(new wd::Thread(process));//直接传函数
    thread->start();
    thread->join();
}

int main()
{
    cout << ">> main Thread " << pthread_self() << endl;
    //test0();
    test1();
    return 0;
}
