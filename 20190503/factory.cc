
#include<math.h>

#include<iostream>
#include<memory>

using std::cout;
using std::endl;
using std::unique_ptr;

#define _USE_MATH_DEFINES

class Figure
{
public:
    virtual void display() const = 0;
    virtual double area() = 0;
};

class Circle
: public Figure
{
public:
    Circle(double radius)
    : _radius(radius)
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
        double p = (_a+_b+_c)/2;
        return sqrt(p*(p-_a)*(p-_b)*(p-_c));
    }
private:
    double _a;
    double _b;
    double _c;
};//

void print(Figure &fig)
{
    fig.display();
    cout << "的面积是: " << fig.area() << endl;
}

class Factory
{
public:
    virtual Figure *create() = 0;
    virtual ~Factory() = default;
};//

class CircleFactory
: public Factory
{
public:
    Figure *create()
    {
        //
        Figure *fig = new Circle(10);
        return fig;
    }
};//

class RectangleFactory
: public Factory
{
public:
    Figure *create()
    {
        //
        Figure *fig = new Rectangle(10, 20);
        return fig;
    }
};//

class TriangleFactory
: public Factory
{
public:
    Figure *create()
    {
        //
        Figure *fig = new Triangle(3, 4, 5);
        return fig;
    }
};//

int main()
{
    unique_ptr<Factory> cirFactory(new CircleFactory());
    unique_ptr<Figure> cir1(cirFactory->create());

    unique_ptr<Factory> recFactory(new RectangleFactory());
    unique_ptr<Figure> rec1(recFactory->create());

    unique_ptr<Factory> triFactory(new TriangleFactory());
    unique_ptr<Figure> tri1(triFactory->create());

    print(*cir1);
    print(*rec1);
    print(*tri1);
    return 0;
}
