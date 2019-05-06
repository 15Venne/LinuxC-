#include<iostream>

using std::cout;
using std::endl;

#define PAI 3.14159

class Circle
{
public:
    Circle()
    : _r(0)
    {
        cout << "Circle()" << endl;
    }

    Circle(double r)
    : _r(r)
    {
        cout << "Circle(double)" << endl;
    }

    double getArea()
    {
        return _r*_r*PAI;
    }

    double getPerimeter()
    {
        return PAI*2*_r;
    }

    void show()
    {
        cout << "r = " << _r << endl
             << "area = " << getArea() << endl
             << "perimeter = " << getPerimeter() << endl;
    }

    ~Circle()
    {
        cout << "~Circle()" << endl;
    }
private:
    double _r;
};//Circle

class Cylinder
: public Circle
{
public:
    Cylinder(double r, double h)
    : Circle(r)
    , _h(h)
    {
        cout << "Cylinder(double ,double)" << endl;
    }

    double getVolume()
    {
        return getArea()*_h;
    }

    void showVolume()
    {
        cout << "Volume = " << getVolume() << endl;
    }
    ~Cylinder()
    {
        cout << "~Cylinder()" << endl;
    }
private:
    double _h;
};//Cylinder




void test0()
{
    Circle c1;
    Circle c2(4);
    c1.show();
    c2.show();
    cout << "-------------" << endl;
    Cylinder cy1(4, 8);
    cy1.show();
    cy1.showVolume();
}

int main()
{
    test0();
}
