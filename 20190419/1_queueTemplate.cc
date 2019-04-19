
#include<string.h>

#include<iostream>
using std::cout;
using std::endl;
using std::string;


template <typename Type, size_t kSize = 10>
class Queue
{
public:
    Queue()
    : _cntMax(kSize)
    , _front(0)
    , _rear(0)
    , _arr(new Type[kSize]())
    {
        cout << "Queue()" << endl;
    }
    
    void push(const Type &val);
    void pop();
    Type front();
    Type back();
    bool empty() const;
    bool full() const;

    ~Queue()
    {
        if(_arr){
            delete [] _arr;
            _arr = nullptr;
            cout << "~Queue()" << endl;
        }
    }
private:
    int _cntMax;
    int _front;
    int _rear;
    Type *_arr;
};

template <typename Type, size_t kSize>
void Queue<Type, kSize>::push(const Type &val)
{
    if(!full())
    {
        _arr[_rear] = val;
        _rear = (_rear + 1) % _cntMax;
    }
}

template <typename Type, size_t kSize>
void Queue<Type, kSize>::pop()
{
    if(!empty())
    {
        _front = (_front + 1) % _cntMax;
    }
}

template <typename Type, size_t kSize>
Type Queue<Type, kSize>::front()
{
    if(!empty())
    {
        return _arr[_front];
    }
}

template <typename Type, size_t kSize>
Type Queue<Type, kSize>::back()
{
    if(!empty())
    {
        if(_rear == 0)
        {
            return _arr[_cntMax - 1];
        }else
        {
            return _arr[_rear - 1];
        }
    }
}

template <typename Type, size_t kSize>
bool Queue<Type, kSize>::empty() const
{
    return _front == _rear;
}

template <typename Type, size_t kSize>
bool Queue<Type, kSize>::full() const
{
    return _front == (_rear + 1) % _cntMax;
}


int test0() //int
{
    Queue<int> que;
    cout << "que is empty?: " << que.empty() << endl;
    que.push(10);
    que.push(12);
    que.push(14);
    que.pop();
    cout << "que.front(): " << que.front() << endl;
    cout << "que.back(): " << que.back() << endl;
    cout << "que is empty?: " << que.empty() << endl;

    for(int i = 0; i < 10; ++i)
    {
        que.push(15 + i);
    }
    cout << "---------------------------" << endl;
    cout << "que is full?: " << que.full() << endl;
    cout << "que.front(): " << que.front() << endl;
    cout << "que.back(): " << que.back() << endl;
    return 0;
}

int test1() // string
{
    Queue<string> que;
    cout << "que is empty?: " << que.empty() << endl;
    que.push("Jojo");
    que.push("DIO");
    que.push("abc");
    que.pop();
    cout << "que.front(): " << que.front() << endl;
    cout << "que.back(): " << que.back() << endl;
    cout << "que is empty?: " << que.empty() << endl;

    for(int i = 0; i < 10; ++i)
    {
        que.push(string(3, 'a' + i));
    }
    cout << "---------------------------" << endl;
    cout << "que is full?: " << que.full() << endl;
    cout << "que.front(): " << que.front() << endl;
    cout << "que.back(): " << que.back() << endl;
    return 0;
}

int main()
{
    test0();
    cout << endl;
    test1();
    return 0;
}
