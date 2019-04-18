#include<iostream>
using std::cout;
using std::endl;

//方法三， atexit + pthread_once
//
class Singleton
{
public:
    static Singleton *getInstance()
    {
        pthread_once(&_once, init);
        return _pInstance;
    }
    static void init()
    {
        _pInstance = new Singleton();
        atexit(destroy);
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
    static pthread_once_t _once;
};

Singleton * Singleton::_pInstance = nullptr;
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;

int main()
{
    Singleton::getInstance()->print();
    return 0;
}
