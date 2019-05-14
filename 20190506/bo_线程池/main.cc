
#include"head.h"

int main()
{
    ThreadPool tpool(4, 10);
    tpool.start();

    unique_ptr<Task> task(new MyTask());

    int cnt = 20;
    while(cnt--)
    {
        tpool.addTask(task.get());
    }
    //sleep(1);//在线程池的stop设置退出心跳
    tpool.stop();
    cout << "end" << endl;
    return 0;
}
