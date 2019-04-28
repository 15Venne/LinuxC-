
//unique_ptr

#include<iostream>
#include<memory>
#include<vector>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::vector;

class Point
{
public:
    Point(int ix, int iy)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int, int)" << endl;
    }   

    void print()
    {
        cout << "( " << _ix
             << ", " << _iy
             << " )" << endl;
    }

    ~Point()
    {
        cout << "~Poing()" << endl;
    }
private:
    int _ix;
    int _iy;
};

void test0()
{
    Point *p1 = new Point(1, 2);
    unique_ptr<Point> pun(p1); //托管
    pun->print();
    (*pun).print();
    //auto pun2(pun);
    //unique_ptr<Point> pun3;
    //pun3 = pun;
}

void test1()
{
    unique_ptr<Point> p = (unique_ptr<Point>)new Point(3, 4);
    p->print();
    vector<unique_ptr<Point>> pArr;
    //pArr.push_back(p);  禁止
    pArr.push_back(std::move(p)); // 允许，把p内容转移到pArr[0]
    cout << "pArr[0]: ";
    pArr[0]->print();

    p.reset(new Point(5, 6)); //重设p内容
    p->print();
    cout << "_____________" << endl;

}

int main()
{
    test0();
    cout << "------------------" << endl;
    test1();
    return 0;
}
