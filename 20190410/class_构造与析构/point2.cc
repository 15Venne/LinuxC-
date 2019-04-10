#include<iostream>
using std::cout;
using std::endl;

class point 
{
public:
    point(int ix=0, int iy=0)
    : _ix(ix), _iy(iy)
    {
        cout << "point(int=0, int=0)" << endl;
    }
    void print()
    {
        cout << "(" << _ix
             << "," << _iy
             << ")" << endl;
    }
    ~point()
    {
        cout << "~point()" << endl;
    }
private:
    int _ix;
    int _iy;
};

int test0()
{
    point pt(1, 2);
    cout << "pt = ";
    pt.print();

    point pt2 = pt;
    cout << "pt2 = ";
    pt2.print();
    return 0;
}

int main()
{
    test0();
    return 0;
}
