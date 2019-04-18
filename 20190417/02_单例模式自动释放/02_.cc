#include<iostream>
using std::cout;
using std::endl;

//方法一， atexit+静态成员函数(destroy)
//
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(!_pInstance)
        {
            atexit(destroy); // 注册destroy;
            _pInstance = new Singleton();
        }
        return _pInstance;
    }
    static void destroy()
    {
        if(_pInstance)
        { 
            delete _pInstance;
            cout << "void destroy()" << endl;
        }
    }

    void print() const
    { cout << "print() const" << endl;}
private:
    Singleton()
    { cout << "Singleton()" << endl; }
    ~Singleton()
    { cout << "~Singleton()" << endl; }
    static Singleton *_pInstance;
};

Singleton * Singleton::_pInstance = getInstance();

int main()
{
    Singleton::getInstance()->print();
    return 0;
}
