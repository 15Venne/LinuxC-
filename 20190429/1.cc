#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class Base
{
public:
	Base(const char * pbase)
	: _pbase(new char[strlen(pbase) + 1]())
	{	
		cout << "Base(const char *)" << endl;	
		strcpy(_pbase, pbase);
	}

	Base(const Base & rhs)
	: _pbase(new char[strlen(rhs._pbase) + 1]())
	{
		cout << "Base(const Base & ) " << endl;	
		strcpy(_pbase, rhs._pbase);
	}

	Base & operator=(const Base & rhs)
	{
		cout << "Base& operator=(const Base&)" << endl;
		if(this != & rhs) {
			delete [] _pbase;
			_pbase = new char[strlen(rhs._pbase) + 1]();
			strcpy(_pbase, rhs._pbase);
		}
		return *this;
	}

	void display() const
	{	
		cout << _pbase << endl;
	}

	~Base()
	{	
		delete [] _pbase;
	}

	friend std::ostream & operator<<(std::ostream & os, const Base & rhs);
protected:
	char * _pbase;
};

std::ostream & operator<<(std::ostream & os, const Base & rhs)
{
	os << rhs._pbase;
	return os;
}

class Derived
: public Base
{
public:
	Derived(const char * pbase)
	: Base(pbase)
	{	cout << "Derived(pbase)" << endl;	}

	void display() const
	{	
		Base::display();
	}

private:
};

 
int main(void)
{
	Derived derived("hello");
	cout << "derived = ";
	derived.display();

	Derived derived2(derived);
	cout << "derived2 = ";
	derived2.display();

	Derived derived3("world");
	derived = derived3;
	cout << "derived = ";
	derived.display();

	return 0;
}
