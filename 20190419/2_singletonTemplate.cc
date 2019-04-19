
#include<string.h>

#include<iostream>
using std::cout;
using std::endl;

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Poine(int, int)" << endl;
    }

    void print()
    {
        cout << "(" << _ix
             << "," << _iy
             << ")" ;
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }

private:
    int _ix;
    int _iy;
};// 

class Computer
{
public:
    Computer()
    : _brand(new char[1]())
    , _price(0)
    {
        cout << "Computer()" << endl;
    }

    Computer(const char *brand, float price)
    : _brand(new char[strlen(brand) + 1]())
    , _price(price)
    {
        cout << "Computer(const char*, float)" << endl;
        strcpy(_brand, brand);
    }
    

    ~Computer()
    {
        if(_brand)
        {
            delete [] _brand;
            _brand = nullptr;
        }
        cout << "~Computer()" << endl;
    }
private:
    char * _brand;
    float _price;
};// 

template <typename Type>
class Singleton
{
public:
    template <typename... Args>
    static Type *getInstance(Args... args)
    {
        if(!_pInstance)
        {
            atexit(destroy);
            _pInstance = new Type(args...);
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
    
private:
    Singleton()
    { cout << "Singleton()" << endl; }
    ~Singleton()
    { cout << "~Singleton()" << endl; }
private:
    static Type *_pInstance;
};


template <typename Type>
Type *Singleton<Type>::_pInstance = nullptr;

//Point(1, 2);
//Point(3, 4);

//Computer com("Mac", 8888);

int main()
{
    Point *p3 = Singleton<Point>::getInstance(1, 2);
    Point *p4 = Singleton<Point>::getInstance(1, 2);
    Computer *p1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    Computer *p2 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    p3->print();cout << endl;
    p4->print();cout <<endl;
    return 0;
}
