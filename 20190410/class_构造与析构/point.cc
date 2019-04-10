#include<iostream>
using std::cout;
using std::endl;

class point
{
public:
    point(int ix, int iy)
    : _ix(ix), _iy(iy) //初始化顺序与定义顺序一样,先_ix, 再_iy
    {
        cout << "point()" << endl 
             << "_ix= " << _ix << endl
             << "_iy= " << _iy << endl;
    }
    void print()
    {
        cout << "(" << _ix
             << "," << _iy
             << ")" << endl;
    }
    ~point()//析构函数,只有一个
    {
        cout << "~point()" << endl;
    }
private:
    int _ix;
    int _iy;
};

int main()
{
    point pt(3, 4);
    pt.print();
    return 0;
}
