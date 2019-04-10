#include<iostream>
#include<string.h>

using std::cout;
using std::endl;

class myString
{
public:
    myString()
    : _pstr(nullptr)
    {
        cout << "myString()" << endl;
    }
    myString(const char *pstr);
    myString(const myString &rhs);
    //myString &operator = (const myString &rhs);
    ~myString()
    {
        if(_pstr)
        {
            delete [] _pstr;
            _pstr = nullptr;
        }
        cout << "~myString()" << endl;
    }

    void print()
    {
     //   if(_pstr)
            cout << _pstr << endl;  
    }

private:
    char * _pstr;
};

myString::myString(const char *pstr)
: _pstr(new char[strlen(pstr)+1]())
{
    strcpy(_pstr, pstr);
    cout << "myString(const char*)" << endl;
}

myString::myString(const myString &rhs)
: _pstr(new char[strlen(rhs._pstr)+1]())
{
    strcpy(_pstr, rhs._pstr);
    cout << "myString(const myString &)" << endl;
}

int main()
{
    myString str1;
    str1.print();
    cout.clear();
    cout << "123" << endl;
    myString str2 = "Hello,world";
    myString str3("wangdao");

    str2.print();
    str3.print();

    myString str4 = str3;
    str4.print();

    str4 = str2;
    str4.print();

    return 0;
}
