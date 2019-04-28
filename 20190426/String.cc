
#include<string.h>

#include<iostream>

using std::cout;
using std::endl;

class String
{
public:
    String()
    : _pstr(new char[1]())
    {
        cout << "String()" << endl;
    }

    String(const char * pstr)
    : _pstr(new char[strlen(pstr) + 1]())
    {
        strcpy(_pstr, pstr);
        cout << "String(const char *)" << endl;
    }

    String(const String &rhs)
    : _pstr(new char[strlen(rhs._pstr) + 1]())
    {
        strcpy(_pstr, rhs._pstr);
        cout << "String(const String &)" << endl;
    }

    String(String && rhs)
    {
        _pstr = rhs._pstr;
        rhs._pstr = nullptr;
        cout << "String(String &&)" << endl;
    }

    String &operator=(const String &rhs)
    {
        cout << "String &operator=(const String &)" << endl;
        if(this != &rhs)
        {
            delete [] _pstr;
            _pstr = new char[strlen(rhs._pstr) + 1]();
            strcpy(_pstr, rhs._pstr);
        }
        return *this;
    }

    String &operator=(String &&rhs)
    {
        cout << "String &operator=(String &&)" << endl;
        if(this != &rhs)
        {
            delete [] _pstr;
            _pstr = rhs._pstr;
            rhs._pstr = nullptr;
        }
        return *this;
    }


    ~String()
    {
        if(_pstr)
        {
            delete [] _pstr;
        }
        _pstr = nullptr;
        cout << "~String()" << endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs);

private:
    char *_pstr;
};//

std::ostream &operator<<(std::ostream &os, const String &rhs)
{
    os << rhs._pstr;
    return os;
}

void test0()
{
    String s1("hello");
    cout << s1 << endl;
    String s2 = "world";
    cout << s2 << endl;
    String s3;
    s3 = s1;
    cout << s3 << endl;
    String s4(s2);
    cout << s4 << endl;
    
    s3 = std::move(s2);
    cout << s3 << endl;

}

int main()
{
    test0();
    return 0;
}
