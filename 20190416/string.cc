#include<string.h>

#include<iostream>
using std::cout;
using std::endl;
using std::cin;
class myString
{
public:
    myString()
    : _pstr(new char[1]())
    {
        cout << "myString()" << endl;
    }
    
    myString(const char *pstr);
    myString(const myString &rhs);
    myString &operator=(const myString &rhs);
    myString &operator=(const char*);
    myString &operator+=(const myString &);
    myString &operator+=(const char*);

    char &operator[](std::size_t index);
    const char &operator[](std::size_t index) const;
    std::size_t size() const;
    const char* c_str() const; //?
    
    friend bool operator==(const myString &, const myString &);
    friend bool operator!=(const myString &, const myString &);
    friend bool operator<(const myString &, const myString &);
    friend bool operator>(const myString &, const myString &);
    friend bool operator<=(const myString &, const myString &);
    friend bool operator>=(const myString &, const myString &);

    friend std::ostream &operator<<(std::ostream &os, const myString &s);
    friend std::istream &operator>>(std::istream &is, myString &s);
    
    ~myString()
    {
        if(_pstr)
        {
            delete [] _pstr;
            _pstr = nullptr;
        }
        cout << "~myString()" << endl;
    }

    void print() const
    {
        if(_pstr)
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

myString & myString::operator=(const myString &rhs)
{
    cout << "myString &operator=(const myString &)" << endl;
    if(this != &rhs)
    {
        if(_pstr)
        {
            delete [] _pstr;
        }
        _pstr = new char[strlen(rhs._pstr)+1]();
        strcpy(_pstr, rhs._pstr);
    }
    return *this;
}

myString &myString::operator=(const char *ch)
{
    cout << "myString &operator=(const char*)" << endl;
    if(strcmp(_pstr, ch) != 0)
    {
        if(_pstr)
        {
            delete [] _pstr;
        }
        _pstr = new char[strlen(ch)+1]();
        strcpy(_pstr, ch);
    }
    return *this;
}

myString &myString::operator+=(const myString &s)
{
    cout << "myString &operator+=(const myString &)" << endl;
    char *tmp = new char[strlen(_pstr)+1]();
    if(_pstr)
    {
        strcpy(tmp, _pstr);
        delete [] _pstr;
    }
    _pstr = new char[strlen(tmp) + strlen(s._pstr) + 1]();
    strcpy(_pstr, tmp);
    strcat(_pstr, s._pstr);
    free(tmp);
    tmp = nullptr;
    return *this;
}

myString &myString::operator+=(const char *ch)
{
    cout << "myString &operator+=(const char*)" << endl;
    char *tmp = new char[strlen(_pstr)+1]();
    if(_pstr)
    {
        strcpy(tmp, _pstr);
        delete [] _pstr;
    }
    _pstr = new char[strlen(tmp) + strlen(ch) + 1]();
    strcpy(_pstr, tmp);
    strcat(_pstr, ch);
    free(tmp);
    tmp = nullptr;
    return *this;
}

char &myString::operator[](std::size_t index)
{
    cout << "char &operator[](std::size_t)" << endl;
    return _pstr[index];
}

const char &myString::operator[](std::size_t index) const
{
    cout << "const char &operator[](std::size_t) const" << endl;
    return _pstr[index];
}

std::size_t myString::size() const
{
    cout << "std::size_t size() const" << endl;
    return strlen(_pstr);
}

const char* myString::c_str() const //?
{

}

bool operator==(const myString &s1, const myString &s2)
{
    cout << "bool operator==(const myString &, const myString &)" << endl;
    if(0 == strcmp(s1._pstr, s2._pstr))
    {
        return true;
    }
    else
        return false;
}

bool operator!=(const myString &s1, const myString &s2)
{    
    cout << "bool operator!=(const myString &, const myString &)" << endl;
    /* if(0 == strcmp(s1._pstr, s2._pstr)) */
    /* { */
    /*     return false; */
    /* } */
    /* else */
        /* return true; */
    if(s1 == s2)
    { return false; }
    else
        return true;
}

bool operator<(const myString &s1, const myString &s2)
{
    cout << "bool operator<(const myString &, const myString &)" << endl;
    if(strcmp(s1._pstr, s2._pstr) < 0)
    { return true; }
    else
        return false;
}

bool operator>(const myString &s1, const myString &s2)
{
    cout << "bool operator>(const myString &, const myString &)" << endl;
    if(strcmp(s1._pstr, s2._pstr) > 0)
    { return true; }
    else 
        return false;
}

bool operator<=(const myString &s1, const myString &s2)
{
    cout << "bool operator<=(const myString &, const myString &)" << endl;
    if(s1 > s2)
    { return false; }
    else
        return true;
}

bool operator>=(const myString &s1, const myString &s2)
{
    cout << "bool operator>=(const myString &, const myString &)" << endl;
    if(s1 < s2)
    { return false; }
    else
        return true;
}

std::ostream &operator<<(std::ostream &os, const myString &s)
{
    cout << "std::ostream &operator<<(std::ostream &, const myString &)" << endl;
    if(s._pstr)
    {
        os << s._pstr;
    }
    else
        cout << "_pstr is nullptr" << endl;
    return os;
}

std::istream &operator>>(std::istream &is, myString &s)
{
    cout << "std::istream &operator>>(std::istream &, myString &)" << endl;
    char *tmp = new char[100]();//..
    is >> tmp;
    s = tmp;
    free(tmp);
    tmp = nullptr;
    return is;
}

myString operator+(const myString &s1, const myString &s2)
{
    cout << "myString operator+(const myString &, const myString &)" << endl;
    myString s3 = s1;
    s3 += s2;
    return s3;
}

myString operator+(const myString &s, const char *ch)
{
    cout << "myString operator+(const myString &, const char*)" << endl;
    myString s3 = s;
    s3 += ch;
    return s3;
}

myString operator+(const char *ch, const myString &s)
{
    cout << "myString operator+(const char*, const myString &)" << endl;
    myString s3 = ch;
    s3 += s;
    return s3;
}

int test0()
{
    myString str1;
    str1.print();  //打印nullptr，cout异常
    //cout.clear();
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

int test1()
{
    myString s1("Hello");
    myString s2(s1);
    myString s3; 
    s3 = s2;
    myString s4;
    s4 = "world";
    cout << "s1:" << s1 << endl 
         << "s2:" << s2 << endl
         << "s3:" << s3 << endl
         << "s4:" << s4 << endl;
    s1 += s4;
    s2 += "world";
    cout << "s1:" << s1 << endl  
         << "s2:" << s2 << endl;
    cout << "s3[1]:" << s3[1] << endl;
    cout << "s4.size():" << s4.size() << endl
         << "s2.size():" << s2.size() << endl;
    cout << "s1==s2?:" << (s1==s2) << endl
         << "s3!=s4?:" << (s3!=s4) << endl
         << "s3>=s2?:" << (s3>=s2) << endl
         << "s1<=s4?:" << (s1<=s4) << endl;
    myString s5;
    s5 = s3 + s4;
    cout << "s5:" << s5 << endl;
    s5 = s1 + "world";
    cout << "s5:" << s5 << endl;
    s5 = "hello" + s4;
    cout  << "s5:" << s5 << endl 
          << "s1:" << s1 << endl
          << "s2:" << s2 << endl
          << "s3:" << s3 << endl
          << "s4:" << s4 << endl;
    cin >> s5;
    cout << s5 << endl;
    return 0;
}

int main()
{
    //test0();
    test1();
    return 0;
}
