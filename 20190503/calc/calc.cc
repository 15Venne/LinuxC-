
#include<iostream>
#include<string>
#include<algorithm>
#include<memory>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::ostream;

class CalcResult
{
public:
    CalcResult(int num)
    : _num(num)
    {  }
    int getNum()
    {
        return _num;
    }
private:
    int _num;
};//

class Calc_base
{
    friend class Calc;
protected:
    virtual
    ~Calc_base() = default;
private:
    virtual CalcResult eval() const = 0;
};//

class Calc
{
    friend Calc operator+(const Calc &, const Calc &);
    friend Calc operator-(const Calc &, const Calc &);
    friend Calc operator*(const Calc &, const Calc &);
    friend Calc operator/(const Calc &, const Calc &);
public:
    Calc(const string &);
    CalcResult eval() const
    {
        return _c->eval();
    }
private:
    Calc(shared_ptr<Calc_base> ca)
    : _c(ca)
    {  }
    shared_ptr<Calc_base> _c;
};//


class NumCalc
: public Calc_base
{
    friend class Calc;
private:
    NumCalc(const string &s) 
    {  
        _num = atoi(s.c_str());
    }
    CalcResult eval() const
    {
        return CalcResult(_num);
    }
private:
    int _num;
};//

inline
Calc::Calc(const string &s)
: _c(new NumCalc(s))
{  }

class BinaryCalc    
: public Calc_base
{
protected:
    BinaryCalc(const Calc &lhs, const Calc &rhs, string s)
    : _lhs(lhs), _rhs(rhs), _opSym(s)
    {  }

    Calc _lhs;
    Calc _rhs;
    string _opSym;
};//

class PlusCalc
: public BinaryCalc
{
    friend Calc operator+(const Calc &, const Calc &);
    PlusCalc(const Calc &lhs, const Calc &rhs)
    : BinaryCalc(lhs, rhs, "+")
    {  }
    CalcResult eval() const
    {
        return CalcResult(_lhs.eval().getNum() + _rhs.eval().getNum());
    }
};//

inline
Calc operator+(const Calc &lhs, const Calc &rhs)
{
    return shared_ptr<Calc_base>(new PlusCalc(lhs, rhs));
}

class SubCalc
: public BinaryCalc
{
    friend Calc operator-(const Calc &, const Calc &);
    SubCalc(const Calc &lhs, const Calc &rhs)
    : BinaryCalc(lhs, rhs, "-")
    {  }
    CalcResult eval() const
    {
        return CalcResult(_lhs.eval().getNum() - _rhs.eval().getNum());
    }
};//

inline
Calc operator-(const Calc &lhs, const Calc &rhs)
{
    return shared_ptr<Calc_base>(new SubCalc(lhs, rhs));
}

class MutiCalc
: public BinaryCalc
{
    friend Calc operator*(const Calc &, const Calc &);
    MutiCalc(const Calc &lhs, const Calc &rhs)
    : BinaryCalc(lhs, rhs, "*")
    {  }
    CalcResult eval() const
    {
        return CalcResult(_lhs.eval().getNum() * _rhs.eval().getNum());
    }
};//

inline
Calc operator*(const Calc &lhs, const Calc &rhs)
{
    return shared_ptr<Calc_base>(new MutiCalc(lhs, rhs));
}

class DiviCalc
: public BinaryCalc
{
    friend Calc operator/(const Calc &, const Calc &);
    DiviCalc(const Calc &lhs, const Calc &rhs)
    : BinaryCalc(lhs, rhs, "/")
    {  }
    CalcResult eval()const
    {
        return CalcResult(_lhs.eval().getNum() / _rhs.eval().getNum());
    }
};//

inline
Calc operator/(const Calc &lhs, const Calc &rhs)
{
    return shared_ptr<Calc_base>(new DiviCalc(lhs, rhs));
}


void test0()
{
    cout << (Calc("5") + Calc("3") * Calc("6")).eval().getNum() << endl;
    cout << (Calc("0") - ((Calc("3") + Calc("6")) * Calc("5"))).eval().getNum() << endl;
}

int myCalc(const string &s)
{
    
}

void test1()
{
    string s1("5+3*6");
    string s2("0-(3+6)*5");
    cout << myCalc(s1) << endl;
    cout << myCalc(s2) << endl;
}

int main()
{
    test0();
    return 0;
}
