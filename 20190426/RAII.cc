
//Resource Acquisition Is Initializetion
#include<iostream>

using std::cout;
using std::endl;

template <class Type>
class RAII
{
public:
    explicit
    RAII(Type *ptr)
    : _ptr(ptr)
    {
        cout << "RAII(Type *)" << endl;
    }

    RAII(const RAII &rhs) = delete;    //托管模式禁止复制与赋值
    RAII &operator=(const RAII&) = delete;

    Type *operator->()
    {
        return _ptr;
    }

    Type &operator*()
    {
        return *_ptr;
    }

    void reset(Type *newPtr)
    {
        if(_ptr)
        {
            delete _ptr;
        }
        _ptr = newPtr;
    }

    ~RAII()
    {
        if(_ptr)
        {
            delete _ptr;
        }
        cout << "~RAII()" << endl;
    }
private:
    Type * _ptr;

};

void test0()
{
    RAII<int> ra(new int(15));
    cout << *ra << endl;
}

int main()
{
    test0();
    return 0;
}






