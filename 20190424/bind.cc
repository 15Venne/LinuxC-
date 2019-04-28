#include<iostream>
#include<functional>

using std::cout;
using std::endl;
using std::bind;
using std::function;
using std::bind;
using namespace std::placeholders;//占位符命名空间


void display1(int x, int y, int z=0)
{
    cout << "x= " << x << ", y= " << y << ", z= " << z << endl; 
}

void display2(int x, int y)
{
    cout << "x= " << x << ", y= " << y << endl;
}

void (* f)(int ,int) = display2;

typedef void (* Function)(int ,int, int);

class Example
{
public:
    int _ix = 10;
    void print(int x, int y)
    {
        cout << "Example::print() " << endl;
        cout << "x= " << x << ", y= " << y << endl;
    }
private:
    int _number;
};


void test0() //C模式
{
    int a = 1 , b = 2;
    f(a, b);
    Function f1 = display1;
    f1(a, b, 3);
}

void test1()
{
    std::function<void(int, int)> f1 = bind(display1, _1, _2, 10);
    int a = 1, b = 2;
    cout << "绑定: " << endl;
    f1(a, b);

    f1 = display2; // 普通函数
    cout << "普通函数: " << endl;
    f1(a, b);

    f1 = f;//函数指针
    cout << "函数指针: " << endl;
    f1(a, b);
    
    Example exm;
    cout << "成员函数: " << endl;
    f1 = bind(&Example::print, &exm, _1, _2);
    f1(a, b);

    cout << "数据成员: " << endl;
    auto f2 = bind(&Example::_ix, &exm);
    cout << "f2() = " << f2() << endl;
}

void test2()
{
    int a = 1, b = 2, c = 3;
    auto f1 = bind(display1, _3, std::ref(c), c);
    f1(a, b, c);
    c = 30;
    f1(a, b, c);
}

int main()
{
    test0();
    test1();
    test2();
    return 0;
}
















