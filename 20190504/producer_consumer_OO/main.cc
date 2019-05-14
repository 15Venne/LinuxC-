

#include"head.h"

#include<memory>

using std::unique_ptr;

void test0()
{
    TaskQueue que(10);
    unique_ptr<Thread> pro(new Producer(que));
    unique_ptr<Thread> con(new Consumer(que));

    pro->start();
    con->start();

    pro->join();
    con->join();
    
}
int main()
{
    test0();
    return 0;
}
