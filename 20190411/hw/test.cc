#include <iostream>
using std::cout;
using std::endl;

//单例模式:
//要求是: 通过该类只能创建出一个对象
//1. 该对象不能是栈对象、全局对象,只剩下一个堆对象

class Singleton
{
public:
	static Singleton * getInstance()
	{
		if(_pInstance == nullptr) 
		{
			_pInstance = new Singleton();
		}
		return _pInstance;
	}

	static void free()
	{
		if(_pInstance) 
		{
			delete _pInstance;
		}
	}

	void print() const
	{	cout << "Singleton::print()" << endl;	}

private:
	Singleton(){	cout << "Singleton()" << endl;	}
	~Singleton() {	cout << "~Singleton()" << endl;	}

	static Singleton * _pInstance;
};

Singleton * Singleton::_pInstance = nullptr;
 
//Singleton s1;//error

int main(void)
{
	//Singleton s2;//error
	//Singleton s3;

	//Singleton * p1 = new Singleton();//error,在类之外无法调用构造函数
	Singleton * p1 = Singleton::getInstance();
	Singleton * p2 = Singleton::getInstance();

	cout << "p1 = " << p1 << endl
		 << "p2 = " << p2 << endl;

	//更多的是这种用法
	Singleton::getInstance()->print();
	//....

	//delete p1;//error 希望该语句编译无法通过
	Singleton::free();
	return 0;
}
