#include<iostream>
#include<memory>
using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;

class Point 
: public std::enable_shared_from_this<Point> // 辅助类
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
    
    shared_ptr<Point> add(Point &pt)
    //Point * add(Point &pt)
    {
        _ix += pt._ix;
        _iy += pt._iy;
        return shared_from_this();
        //return shared_ptr<Point>(this);
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }
private:
    int _ix;
    int _iy;
};

void test0() //error, 会造成test1段错误
{
    Point *pt = new Point(1, 2);
    unique_ptr<Point> up1(pt);
    cout << "up1: " ; up1->print();
    unique_ptr<Point> up2(pt);
    cout << "up2: " ; up2->print();
}

void test1()
{
    Point *pt = new Point(1, 2);
    shared_ptr<Point> sp1(pt);
    cout << "sp1: ";
    sp1->print();
    shared_ptr<Point> sp2(sp1); //
    cout << "sp2: ";
    sp2->print();
    shared_ptr<Point> sp3(sp1->add(*sp2));
    cout << "sp3: ";
    sp3->print();
    cout << "sp1: ";
    sp1->print();
}

int main()
{
    //test0();
    cout << "-----------" << endl;
    test1();
    return 0;
}






