#include <iostream>
using std::cout;
using std::endl;

class Point
{
public:
	Point(int ix = 0, int iy = 0)
	: _ix(ix)
	, _iy(iy)
	{	cout << "Point(int,int)" << endl;}
    
    Point & operator=(const Point &rhs)
    {
        cout << "Point & operator=(const Point &)" << endl;
        _ix = rhs._ix;
        _iy = rhs._iy;
    }

	~Point()
	{
		cout << "~Point()" << endl;
	}

	friend class Complex;
	friend std::ostream & operator<<(std::ostream & os, const Point & rhs);
private:
	int _ix;
	int _iy;
};

std::ostream & operator<<(std::ostream & os, const Point & rhs)
{
	os << "(" << rhs._ix
	   << "," << rhs._iy
	   << ")";
	return os;
}

class Complex
{
public:
	Complex(double real = 0, double image = 0)
	: _real(real)
	, _image(image)
	{
		cout << "Complex(double,double)" << endl;
	}

	Complex(const Point & rhs)
	: _real(rhs._ix)
	, _image(rhs._iy)
	{
		cout << "Complex(const Point & rhs)" << endl;
	}

	void print() const
	{
		cout << _real << " + " << _image << "i" << endl;
	}

	Complex & operator+=(const Complex & rhs)
	{
		_real += rhs._real;
		_image += rhs._image;

		return *this;
	}

	//前置的效率更高
	//前置return的是引用, 后置返回的是对象
	Complex & operator++()//前置形式
	{
		++_real;
		++_image;
		return *this;
	}

	Complex operator++(int)//后置形式, int只作为标记，不真正传参
	{
		Complex tmp(*this);//先保存对象变化之前的值
		++_real;
		++_image;
		return tmp;
	}

	//类型转换函数
	// 成员函数
	// 在函数形式上没有返回值
	// 在函数体内必须以值传递方式return一个目标类型的变量
	// 参数列表是无参的
	//  
	// 一般情况下，不要使用
	operator int()
	{
		cout << "operator int()" << endl;
		return _real * _image;
	}

	operator double()
	{
		cout << "operator double()" << endl;
		return static_cast<double>(_real) / _image;
	}

	operator Point()
	{
		cout << "operator Point()" << endl;
		return Point(_real, _image);
	}

	friend Complex operator+(const Complex & lhs, const Complex & rhs);
	friend std::ostream & operator<<(std::ostream & os, const Complex & rhs);

private:
	double _real;
	double _image;
};

std::ostream & operator<<(std::ostream & os, const Complex & rhs)
{
	os << rhs._real << " + " << rhs._image << " i" << endl;
	return os;
}

Complex operator+(const Complex & lhs, const Complex & rhs)
{
	Complex tmp(lhs);
	tmp += rhs;
	return tmp;
}

int main(void)
{
	Complex c1(1, 2);
	cout << "c1 = " << c1 << endl;

	Point pt(11, 12);
	cout << "pt = " << pt << endl <<endl;

	//违反常规思维
	//c1 = pt;//implicit 其他类型向自定义类型进行转换
	//
    Point pt2(1,2);
    cout << "pt2 = " << pt2 << endl<<endl;
	pt = c1;//将自定义类型向其他类型转换
	cout << "pt = " << pt << endl;
    int x = c1;
	double y = c1;
	cout << "x = " << x << endl
		 << "y = " << y << endl;
	return 0;
}
