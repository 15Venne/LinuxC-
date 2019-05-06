#include <string.h>
#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;

class Base
{
public:
	Base(double base,
		 const char * data1)
	: _base(base)
	, _data1(new char[strlen(data1) + 1]())
	{	
		cout << "Base(double) " << endl;
		strcpy(_data1, data1);
	}	

	virtual
	void display()const 
	{	
		cout << "Base::_base = " << _base <<endl;
		cout << "_data1 = " << _data1 << endl;
	}

	//在基类中只要定义了虚函数，必须要将其析构函数设为虚函数
	//
	//其派生类析构函数会自动成为虚函数，无论有没有加virtual关键字
	virtual
	~Base()
	{	
		cout << "~Base()" << endl;	
		delete [] _data1;
	} 

private:
	double _base;
	char * _data1;
};

class Derived
: public Base
{
public:
	Derived(double base, 
			double derived,
			const char * data1,
			const char * data2)
	: Base(base, data1)
	, _derived(derived)
	, _data2(new char[strlen(data2) + 1]())
	{
		cout << "Derived(double,double)" << endl;
		strcpy(_data2, data2);
	}

	void display() const
	{
		Base::display();
		cout << "Derived::_derived = " << _derived << endl
			 << "_data2: " << _data2 << endl;
	}

	~Derived()
	{	
		cout << "~Derived()" << endl;	
		delete [] _data2;
	}

private:
	double _derived;
	char * _data2;
};
 
int main(void)
{
	unique_ptr<Base> pbase(new Derived(1.11, 2.22, "hello", "world"));
	pbase->display();

	return 0;
}
