#include<iostream>
using std::cout;
using std::endl;

class Base // 抽象类, 无法创建对象
{
public:
    virtual
    void display() = 0;

    virtual 
    void print() = 0;
};

class Derived
: public Base
{
public:
    void display()
    {
        cout << "Derived::display()" << endl;
    }
};//还有print()没实现，他也还是抽象类

class Grandson
: public Derived
{
public:
    void print()
    {
        cout << "Grandson::print()" << endl;
    }
};

int main()
{
    Base *p = new Grandson();
    p->display();
    p->print();
    cout << "sizeof(Base)" << sizeof(Base) << endl
         << "sizeof(Derived)" << sizeof(Derived) << endl
         << "sizeof(Grandson)" << sizeof(Grandson) << endl;
    delete p;
    return 0;
}
