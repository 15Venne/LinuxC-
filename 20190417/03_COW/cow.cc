
#include<string.h>

#include<iostream>

using std::cout;
using std::endl;

#define REFCNT (*(int*)(_pstr - 4))

class String
{
    class CharProxy
    {
    public:
        CharProxy(String &self, int idx)
        : _self(self)
        , _idx(idx)
        {  }

        char & operator=(const char &ch)
        {
            cout << "char &operator=(const char &)" << endl;
            if(_idx >= 0 && _idx < _self.size())
            {
                if(_self._pstr[_idx] == ch)
                {
                    return _self._pstr[_idx];
                }
                if(_self.refcnt() > 1)
                {
                    _self.refcntDecrease();
                    char *tmp = new char[_self.size() + 5]() + 4;
                    strcpy(tmp, _self._pstr);

                    _self._pstr = tmp;
                    _self.refcntInit();
                }
                _self._pstr[_idx] = ch;
                return _self._pstr[_idx];
            }
            else
            {
                cout <<  "越界" << endl;
                static char nullchar = '\0';
                return nullchar;
            }
        }

        operator char()
        {
            return _self._pstr[_idx];
        }
        
    private:
        String &_self;
        int _idx;
    };//end of class CharProxy
public:
    String()      
    : _pstr(new char[5]() + 4) //4(int) + 1
    {
        refcntInit();
    }

    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 5]() + 4)
    {
        refcntInit();
        strcpy(_pstr, pstr);
    }

    String(const String &rhs)
    : _pstr(rhs._pstr)
    {
        refcntIncrease();
    }

    String & operator=(const String &rhs)
    {
        cout << "String &operator=(const String &)" << endl;
        if(this != &rhs)
        {
            release();
            _pstr = rhs._pstr;
            refcntIncrease();
        }
        return *this;
    }

    String & operator=(const char*pstr)
    {
        cout << "String &operator=(const char*)" << endl;
        if(strcmp(pstr, _pstr) != 0)
        {
            release();
            _pstr = new char[strlen(pstr) + 5]() + 4;
            refcntInit();
            strcpy(_pstr, pstr);
        }
        return *this;
    }


    int size() const
    { return strlen(_pstr); }

    const char *c_str()
    { return _pstr; }

    void release()
    {
        refcntDecrease();
        if(0 == REFCNT)
        {
            delete [] (_pstr - 4);
            cout << "void release()" << endl;
        }
    }

    int refcnt()
    {
        return REFCNT;
    }

    CharProxy operator[](int idx)
    {
        cout << "CharProxy operator[](int)" << endl;
        return CharProxy(*this, idx); 
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs)
    {
        os << rhs._pstr;
        return os;
    }

    ~String()
    { release(); }

    
private:
    void refcntInit()
    { REFCNT = 1; }
    
    void refcntIncrease()
    { ++REFCNT; }
    
    void refcntDecrease()
    { --REFCNT; }

private:
    char * _pstr; // 4 + size()
};


int test0()
{
    String s = "hello";
    cout << "s = " << s << endl;
    s[1] = 'd';
    String s1 = "world";
    char ch = 'p';
    //s[1] = s[4];
    s[1]  = ch;
    //s = "world";
    //s = s1;
    cout << "s = " << s << endl;
    return 0;
}

int main()
{
    String s1;
    cout << "s1 = " << s1 << endl;
    String s2 = "hello,world";
    String s3 = s2;
    String s4("wangdao");
    s4 = s2;
    
    cout << "s2 = " << s2 << endl
         << "s3 = " << s3 << endl
         << "s4 = " << s4 << endl;
    cout << "s2's refcnt = " << s2.refcnt() << endl
         << "s3's refcnt = " << s3.refcnt() << endl
         << "s4's refcnt = " << s4.refcnt() << endl;
    printf("s2's address = %p\n", s2.c_str());
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    //test0();
    //
    s4[0] = 'X';
    cout << endl << "写操作之后" << endl;
    cout << "s2 = " << s2 << endl
         << "s3 = " << s3 << endl
         << "s4 = " << s4 << endl;
    cout << "s2's refcnt = " << s2.refcnt() << endl
         << "s3's refcnt = " << s3.refcnt() << endl
         << "s4's refcnt = " << s4.refcnt() << endl;
    printf("s2's address = %p\n", s2.c_str());
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    //
    cout << endl << "读操作之后" << endl;
    cout << "s2[0]" << s2[0] << endl;

    cout << "s2 = " << s2 << endl
         << "s3 = " << s3 << endl
         << "s4 = " << s4 << endl;
    cout << "s2's refcnt = " << s2.refcnt() << endl
         << "s3's refcnt = " << s3.refcnt() << endl
         << "s4's refcnt = " << s4.refcnt() << endl;
    printf("s2's address = %p\n", s2.c_str());
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    return 0;
}





