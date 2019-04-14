//实现只能生成堆对象的代码
#include<string.h>

#include<iostream>
//不能让栈对象生成，栈对象->调用构造函数和析构函数
//因为生成堆对象，即调用new，要先operator new，然后调用构造函数，所以不能把构造函数放private
//于是把析构函数放入private，但是delete第一步是调用析构函数，会导致类之外delete无法使用
//于是需要自己写一个destroy来销毁申请的堆空间
using std::cout;
using std::endl;

class Student
{
public:
    Student(int id, const char *name)
    : _id(id)
    , _name(new char[strlen(name)+1]())
    {
        strcpy(_name, name);
        cout << "Student(int, const char*)" << endl;
    }

    void print() const
    {
        cout << "学号:" << _id << endl
             << "名字:" << _name << endl;
    }

    void *operator new(size_t sz) //sz系统自动分配
    {
        cout << "void *operator new(size_t)" << endl;
        void *p = malloc(sz);
        return p;
    }
    
    void operator delete(void *p)
    {
        free(p);   //释放类自身的空间
        cout << "void operator delete(void*)" << endl;
    }

    void destroy()
    {
        cout << "void destroy()" << endl;
        delete this; //先析构，再调用operator delete
    }

private:
    ~Student()
    {
        delete [] _name; //delete类的成员申请的空间
        cout << "~Student()" << endl;
    }
    int _id;
    char *_name;
};

void test0()
{
    Student *pstu = new Student(1001, "DIO");
    pstu->print();
    pstu->destroy();

    //Student s1(1002, "Jojo");
}

int main()
{
    test0();
    return 0;
}
