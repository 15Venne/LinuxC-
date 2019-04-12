#include<iostream>

using std::cout;
using std::endl;

class Stack
{
public:
    Stack(int maxcnt)
    : _maxcnt(maxcnt)
    , _top(0)
    , _arr(new int[maxcnt]())
    {
        cout << "Stack(int)" << endl;       
    }

    void push(int num);
    void pop();
    int top();
    bool emty();
    bool full();

    ~Stack()
    {
        if(_arr)
        {
            delete [] _arr;
            _arr = nullptr;
            cout << "delete" << endl;
        }
        cout << "~Stack()" << endl;
    }
private:
    int _maxcnt;
    int _top;
    int *_arr;
};

void Stack::push(int num)
{
    if(true == full())
    {
        cout << "stack is full" << endl;
    }
    else
    {
        _arr[_top] = num;
        _top++;
    }
}

void Stack::pop()
{
    if(true == emty())
    {
        cout << "stack is empty" << endl;
    }
    else
    {
        _top--;
    }
}

int Stack::top()
{
    return _arr[_top-1];
}

bool Stack::emty()
{
    if(_top == 0)
    {
        return true;
    }
    else
    { return false; }
}

bool Stack::full()
{
    if(_top == _maxcnt)
    {
        return true;
    }
    else
    { return false; }
}

int main()
{
    Stack st(10);
    st.push(10);
    st.push(12);
    st.push(14);
    cout << st.top() << endl;
    st.pop();
    cout << st.top() << endl;
    return 0;
}
