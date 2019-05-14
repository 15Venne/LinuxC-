
#include"head.h"

#include<memory>
using std::unique_ptr;

class Producer
{
public:
    void produce(TaskQueue &que)
    {
        srand(clock());
        while(1)
        {
            int number = rand() % 100;
            que.push(number);
            cout << ">> Producer thread produce a number: "
                 << number << endl;
            sleep(1);
        }
    }
};//

class Consumer
{
public:
    void consume(TaskQueue &que)
    {
        while(1)
        {
            int number = que.pop();
            cout << ">>>> Consumer thread consume a number: "
                 << number << endl;
            sleep(2);
        }
    }
};//

void test0()
{
    TaskQueue que(10);
    Producer pro;
    Consumer con;
    unique_ptr<Thread> proThread(new Thread(
        std::bind(&Producer::produce, &pro, std::ref(que))
                                           ));
    unique_ptr<Thread> conThread(new Thread(
        std::bind(&Consumer::consume, &con, std::ref(que))
                                           ));

    proThread->start();
    conThread->start();

    proThread->join();
    conThread->join();
}

int main()
{
    test0();
    return 0;
}
