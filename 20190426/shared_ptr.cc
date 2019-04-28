#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
using std::cout;
using std::endl;
using std::vector;
using std::ifstream;
using std::shared_ptr;
 
class Point
{
public:
	Point(int ix = 0, int iy = 0)
	: _ix(ix)
	, _iy(iy)
	{	cout << "Point(int,int)" << endl;}

	void print() const
	{
		cout << "(" << _ix
			 << "," << _iy
			 << ")" << endl;
	}

	~Point()
	{	cout << "~Point()" << endl;}

private:
	int _ix;
	int _iy;
};

int test0(void)
{
	shared_ptr<Point> sp(new Point(1, 2));
	sp->print();
	(*sp).print();
	cout << "sp'get() = " << sp.get() << endl;

	cout << "sp'use_count() = " << sp.use_count() << endl;

	shared_ptr<Point> sp2(sp);
	cout << endl << "执行复制构造之后:" << endl;
	cout << "sp'use_count() = " << sp.use_count() << endl;
	cout << "sp2'use_count() = " << sp2.use_count() << endl;

	vector<shared_ptr<Point>> points;
	points.push_back(sp);
	points[0]->print();
	(*points[0]).print();
	cout << "sp'use_count() = " << sp.use_count() << endl;
	cout << "sp2'use_count() = " << sp2.use_count() << endl;
	cout << "points[0]'use_count() = " << points[0].use_count() << endl;

	return 0;
}

int test1()
{
	//shared_ptr的作用: 将一个表达对象语义的对象切换为表达值语义
	shared_ptr<ifstream> ifs(new ifstream("unique_ptr.cc"));
	shared_ptr<ifstream> ifs2(ifs);

	return 0;
}

int main()
{
    test0();
//   test1();
    return 0;
}
