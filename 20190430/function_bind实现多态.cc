#include<iostream>
#include<functional>
#include<cmath>

#define _USE_MATH_DEFINES

using std::cout;
using std::endl;
using std::function;
using std::bind;

class Figure
{
public:
    void setcb_void(function<void()> &&cb)
    {
        _cb1 = std::move(cb);
    }

    void setcb_double(function<double()> &&cb)
    {
        _cb2 = std::move(cb);
    }

    void handleCb_void()
    {
        if(_cb1)
        { _cb1(); }
    }

    double handleCb_double()
    {
        if(_cb2)
        {
            return _cb2();
        }else{
            return 0;
        }
    }

private:
    function<void()> _cb1;
    function<double()> _cb2;
};//


class Circle
{
public:
    Circle(double r)
    : _radius(r)
    {  }

    void CirDisplay() const
    {
        cout << "Circle";
    }

    double CirArea()
    {
        return M_PI * _radius * _radius;
    }
private:
    double _radius;

};//

class Rectangle
{
public:
    Rectangle(double len, double wid)
    : _length(len), _width(wid)
    {  }

    void RecDisplay() const
    {
        cout << "Rectangle";
    }

    double RecArea()
    {
        return _length * _width;
    }

private:
    double _length;
    double _width;
};//

class Triangle
{
public:
    Triangle(double a, double b, double c)
    : _a(a), _b(b), _c(c)
    {  }

    void TriDisplay() const
    {
        cout << "Triangle";
    }

    double TriArea()
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
    fig.handleCb_void();
    cout << "的面积是: ";
    cout << fig.handleCb_double() << endl;
}

void test0()
{
    Circle cir(10);
    Rectangle rec(10, 20);
    Triangle tri(3, 4, 5);

    Figure fig;
    fig.setcb_void(bind(&Circle::CirDisplay, &cir)); //bind
    fig.setcb_double(bind(&Circle::CirArea, &cir));
    print(fig);

    fig.setcb_void(bind(&Rectangle::RecDisplay, &rec));
    fig.setcb_double(bind(&Rectangle::RecArea, &rec));
    print(fig);

    fig.setcb_void(bind(&Triangle::TriDisplay, &tri));
    fig.setcb_double(bind(&Triangle::TriArea, &tri));
    print(fig);
}

int main()
{
    test0();
    return 0;
}
