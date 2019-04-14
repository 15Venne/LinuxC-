//实现只能生成栈对象的代码
#include<string.h>

#include<iostream>
//只能生成栈对象，new语句error，把operator new放入private
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

    ~Student()
    {
        delete [] _name;
        cout << "~Student()" << endl;
    }
private:
    void *operator new(size_t sz) //私有 无法new
    {
        cout << "void *operator new(size_t)" << endl;
    }
    void operator delete(void *p)
    {
        cout << "void operator delete(void*)" << endl;
    }
    int _id;
    char *_name;
};

void test0()
{
    Student s1(1001, "DIO");
    s1.print();

    //Student *ps = new Student(1002, "Jojo");//error

}

int main()
{
    test0();
    return 0;
}
