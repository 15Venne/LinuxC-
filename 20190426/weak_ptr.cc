#include<iostream>
#include<memory>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;

class Point
{
public:
    Point(int ix, int iy)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point()" << endl;
    }

    void print()
    {
        cout << "(" << _ix
             << "," << _iy
             << ")" << endl;
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }
private:
    int _ix;
    int _iy;
};

void test0()
{
    weak_ptr<Point> wp;
    shared_ptr<Point> sp(new Point(1, 2));
    wp = sp;
    cout << "sp use_count() = " << sp.use_count() << endl;//weak_ptr不增加引用计数
    cout << "wp use_count() = " << wp.use_count() << endl;
    //wp不能直接访问资源
    
    shared_ptr<Point> sp2 = wp.lock(); // 将weak_ptr提升成shared_ptr;
    if(sp2)
    {
        cout << "提升成功" << endl;
        sp2->print();
    } else{
        cout << "提升失败" << endl;
    }
}

void test1()
{
    weak_ptr<Point> wp;
    {
        shared_ptr<Point> sp(new Point(1, 2));
        wp = sp;
        cout << "wp.expired() = " << wp.expired() << endl;//空间没释放返回0 
    }
    shared_ptr<Point> sp2 = wp.lock();
    if(sp2)
    {
        cout << "提升成功" << endl;
        sp2->print();
    } else{
        cout << "提升失败" << endl;
    }
    cout << "wp.expired() = " << wp.expired() << endl; //空间已释放返回1
}



int main()
{
    test0();
    cout << "-----------------" << endl;
    test1();
    return 0;
}






