#include<iostream>
#include<string.h>

using std::cout;
using std::endl;

class computer
{
public:
    computer()
    : _brand(nullptr), _price(0)
    {}
    computer(const char *brand, float price);
    computer(const computer &rhs);
    void print();
    
    ~computer()
    {
        delete [] _brand;
        _brand = nullptr;
        cout << "~computer()" << endl;
    }
private:
    char *_brand;
    float _price;    
};

computer::computer(const char *brand, float price)
    //_brand(brand) //浅拷贝
: _brand(new char[strlen(brand)+1]()), _price(price)
{
    strcpy(_brand, brand);
    cout << "computer(const char*, float) " << endl;
}

computer::computer(const computer &rhs)    //复制构造函数
: _brand(new char[strlen(rhs._brand)+1]()), _price(rhs._price)
{
    strcpy(_brand, rhs._brand);
    cout << "computer(const computer &)" << endl;
}

void computer::print()
{
    cout << ">>brand: " << _brand << endl
         << ">>price: " << _price << endl;
}

int test0()
{
    computer pc1("xiaomi", 7500);
    pc1.print();
    static computer pc2("MSI", 7777); //全局/静态区
    pc2.print();
    return 0;
}

int test1()
{
    computer *p1 = new computer("Thinkpad", 8888);
    p1->print();

    delete p1;

    test0();
    return 0;
}

void test2()
{
    computer pc1("DELL", 6666);
    cout<< "pc1: " << endl;
    pc1.print();
    cout << endl;

    computer pc2 = pc1;   //调用复制构造函数
    cout << "pc2: " << endl;
    pc2.print();
}

void display(computer com)  //值传递/进行复制
{
    cout<< "com:" << endl;
    com.print();
}

void test3()
{
    computer pc1("Thinkpad", 8888);
    display(pc1);
}

computer getComputer()
{
    computer pc1("Thinkpad", 8888);
    cout << "pc1: " << endl;
    pc1.print();
    return pc1; //复制一
}

void test4()
{
    computer pc2 = getComputer(); //复制二
    cout << "pc2: " << endl;
    pc2.print();
}

int main()
{
    //test1();
    //test2();
    //test3();
    test4();
    return 0;
}
