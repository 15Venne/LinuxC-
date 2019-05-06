#include<cmath>
#include<iostream>
using std::cout;
using std::endl;

#define _USE_MATH_DEFINES

class Figure
{
public:
    virtual void display() const = 0;
    virtual double area() = 0;
};//

class Circle
: public Figure
{
public:
    Circle(double r)
    : _radius(r)
    {  }

    void display() const
    {
        cout << "Circle";
    }

    double area()
    {
        return M_PI * _radius * _radius;
    }
private:
    double _radius;

};//

class Rectangle
: public Figure
{
public:
    Rectangle(double len, double wid)
    : _length(len), _width(wid)
    {  }

    void display() const
    {
        cout << "Rectangle";
    }

    double area()
    {
        return _length * _width;
    }

private:
    double _length;
    double _width;
};//

class Triangle
: public Figure
{
public:
    Triangle(double a, double b, double c)
    : _a(a), _b(b), _c(c)
    {  }

    void display() const
    {
        cout << "Triangle";
    }

    double area()
    {
        double p = (_a + _b + _c) / 2;
        return sqrt(p * ( p - _a) * (p - _b) * ( p - _c));
    }

private:
    double _a;
    double _b;
    double _c;
};//

void print(Figure &fig)
{
    fig.display();
    cout << "的面积是: ";
    cout << fig.area() << endl;
}

void test0()
{
    Circle cir(10);
    Rectangle rec(10, 20);
    Triangle tri(3, 4, 5);
    print(cir);
    print(rec);
    print(tri);

}


int main()
{
    test0();
    return 0;
}
