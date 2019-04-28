
#include<math.h>

#include<iostream>
#include<algorithm>
#include<vector>
#include<iterator>

using std::cout;
using std::endl;
using std::sort;
using std::vector;
using std::ostream_iterator;

class Point
{
public:
    Point(int ix, int iy)
    : _ix(ix)
    , _iy(iy)
    {  }
    friend std::ostream & operator<<(std::ostream &os, const Point & rhs);
    float getDistance() const
    {
        return sqrt(_ix *_ix + _iy * _iy);
    }
private:
    int _ix;
    int _iy;
};

std::ostream & operator<<(std::ostream &os, const Point & rhs)
{
    os << "(" << rhs._ix
       << "," << rhs._iy
       << ")";
    return os;
}

struct Compare
{
    bool operator()(const Point &lhs, const Point &rhs) const
    {
        return lhs.getDistance() < rhs.getDistance();
    }
};

void test0()
{
    vector<Point> points{
        Point(-1, 2),
        Point(1, 2),
        Point(3, 4),
        Point(5, 4),
        Point(-3, 2)
    };
    sort(points.begin(), points.end(), Compare());
    copy(points.begin(), points.end(), ostream_iterator<Point>(cout, " "));
    cout << endl;
}

int main()
{
    test0();
    return 0;
}
