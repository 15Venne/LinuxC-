#include<iostream>
using std::cout;
using std::endl;

class Base
{
public:
    Base(double base)
    : _base(base)
    {
        cout << "Base(double)" << endl;
    }

    virtual
    void display() const
    {
        cout << "Base::display(), _base = " << _base << endl;
    }

    void fun1()
    {
        this->display();
    }


private:
    double _base;
};//

class Derived
: public Base
{
public:
    Derived(double base, double derived)
    : Base(base)
    , _derived(derived)
    {
        cout << "Derived(double ,double)" << endl;
    }

    void display() const
    {
        cout << "Derived::display, _derived = " << _derived << endl;
    }
private:
    double _derived;
};

void test0()
{
    Derived d(1.11, 2.22);
    Base *p = &d;
    p->fun1();
    Base b(3.33);
    p = &b;
    p->fun1();
}

int main()
{
    test0();
    return 0;
}
