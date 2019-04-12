#include<iostream>

using std::cout;
using std::endl;

class Queue
{
public:
    Queue(int cntmax)
    : _cntMax(cntmax)
    , _front(0)
    , _rear(0)
    , _arr(new int[cntmax]())
    {
        cout << "Queue(int)" << endl;
    }
    void push(int);
    void pop();
    int front();
    int back();
    bool emty();
    bool full();

    ~Queue()
    {
        if(_arr)
        {
            delete [] _arr;
            _arr = nullptr;
            cout << "delete" << endl;
        }
        cout << "~Queue()" << endl;
    }
private:
    int _cntMax;//队列总大小
    int _front; //头，删
    int _rear;  //尾，插
    int *_arr;
};

void Queue::push(int value)
{
    if(true == full())
    {
        cout << "Queue is full" << endl;
    }
    else
    {
        _arr[_rear] = value;
        _rear = (_rear+1)%_cntMax;
    }
}

void Queue::pop()
{
    if(true == emty())
    {
        cout << "Queue is empty" << endl;
    }
    else
    {
        _front = (_front+1)%_cntMax;
    }
}

int Queue::front()
{
    if(true == emty())
    {
        cout << "Queue is empty" << endl;
    }
    else
    {
        return _arr[_front];
    }
}

int Queue::back()
{
    if(true == emty())
    {
        cout << "Queue is empty" << endl;
    }
    else
    {
        if(_rear == 0)
        {
            return _arr[_cntMax];
        }
        else
        { return _arr[_rear-1]; }
    }
}

bool Queue::emty()
{
    if(_front == _rear)
    {
        return true;
    }
    else
    { return false;  }
}

bool Queue::full()
{
    if(_front == (_rear+1)%_cntMax)
    {
        return true;
    }
    else
    { return false;  }
}

int main()
{
    Queue que(10);
    que.push(10);
    que.push(12);
    que.push(14);
    cout << que.front() << endl;
    cout << que.back() << endl;
    que.pop();
    cout << endl;
    cout << que.front() << endl;
    cout << que.back() << endl;
    return 0;
}
