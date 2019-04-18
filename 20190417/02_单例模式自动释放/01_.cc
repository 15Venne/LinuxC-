#include<iostream>
using std::cout;
using std::endl;

//方法一， 嵌套类+静态对象
//
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(!_pInstance)
        {
            _pInstance = new Singleton();
        }
        return _pInstance;
    }

    void print() const
    { cout << "print() const" << endl;}
private:
    class AutoRelease
    {
    public:
        AutoRelease()
        {
            cout << "AutoRelease()" << endl;
        }
        ~AutoRelease()
        {
            if(_pInstance)
            {
                delete _pInstance;
                cout << "~AutoRelease()" << endl;
            }
        }
    };//嵌套类
private:
    Singleton()
    { cout << "Singleton()" << endl; }
    ~Singleton()
    { cout << "~Singleton()" << endl; }
    static Singleton *_pInstance;
    static AutoRelease _auto; //静态对象
};

Singleton * Singleton::_pInstance = getInstance();
Singleton::AutoRelease Singleton::_auto;

int main()
{
    Singleton::getInstance()->print();
    return 0;
}
