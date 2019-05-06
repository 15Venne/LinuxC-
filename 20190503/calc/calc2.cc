
#include<iostream>
#include<string>
#include<algorithm>
#include<memory>
#include<stack>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::ostream;
using std::copy;
using std::stack;
using std::remove;

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
public:
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
    : _num(0), _s(s), _isNum(true)
    {  
        _num = atoi(s.c_str());
    }

    /* int stringCalc(const string &s) */
    /* { */
    /*     if(_isNum) */
    /*     { return _num; } */
    /*     int i = 0; */

    /* } */
    CalcResult eval() const
    {
        return CalcResult(_num);
    }
private:
    int _num;
    string _s;
    bool _isNum;
};//



class BinaryCalc    
: public Calc_base
{
    friend class Calc;
protected:
    BinaryCalc(const Calc &lhs, const Calc &rhs, string s)
    : _lhs(lhs), _rhs(rhs), _opSym(s)
    {  }
    //BinaryCalc(const string &lhs, const string &rhs);
    Calc _lhs;
    Calc _rhs;
    string _opSym;
};//

class PlusCalc
: public BinaryCalc
{
    friend Calc operator+(const Calc &, const Calc &);
    friend class Calc;
    PlusCalc(const Calc &lhs, const Calc &rhs)
    : BinaryCalc(lhs, rhs, "+")
    {  }
    PlusCalc(const string &lhs, const string &rhs)
    : BinaryCalc(Calc(lhs), Calc(rhs), "+")
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
    friend class Calc;
    SubCalc(const Calc &lhs, const Calc &rhs)
    : BinaryCalc(lhs, rhs, "-")
    {  }
    SubCalc(const string &lhs, const string &rhs)
    : BinaryCalc(Calc(lhs), Calc(rhs), "-")
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


Calc::Calc(const string &s)
{
    bool isNum = true;
    int i;
    string stmp = s;
Loop:
    i = 0;
    while(stmp[i])
    {
        if(!isdigit(stmp[i]))
        {
            isNum = false;
            break;
        }else{
            ++i;
        }
    }
    if(isNum)
    {
        _c = shared_ptr<Calc_base>(new NumCalc(stmp));
    }else{
        i = stmp.size() - 1;
        if(!isdigit(stmp[i]) && stmp[i]!= ')')
        {
            cout << "字符串非法, 无法计算" << endl;
            //终止
        }
        else 
        {
            auto it1 = stmp.end() - 1;
            if(stmp[i] == ')')
            {
                stack<char> st;
                st.push(*it1);
                it1--;
                while(!st.empty() && it1 != stmp.begin())
                {
                    if(*it1 == ')')
                    {
                        st.push(*it1);
                    }else if(*it1 == '(')
                    {
                        st.pop();
                    }
                    it1--;
                }
                if(it1 == stmp.begin())
                {
                    if(*it1 == ')')
                        st.push(*it1);
                    else if(*it1 == '(')
                        st.pop();
                }
                if(!st.empty())
                {
                    cout << "字符串非法, 无法计算" << endl;
                    //终止
                }
                if(it1 == stmp.begin())
                {
                    //auto tmpit = copy(stmp.begin() + 1, stmp.end() - 1, stmp.begin());                           
                    stmp.erase(stmp.end() - 1);
                    stmp.erase(stmp.begin());
                    goto Loop;
                    //stmp.erase(tmpit, stmp.end());
                    //cout << "处理括号后" << endl;
                    //cout << stmp << endl;
                }
            }else
            {
                while(isdigit(*it1))
                {
                    it1--;
                }
            }
            //auto it = stmp.begin();
            //while(it != stmp.end() && isdigit(*it))
            //{
            //    ++it;
            //} 
            
            if(*it1 == '+')
            {
                string rhs(it1 + 1, stmp.end());
                string lhs(stmp.begin(), it1);
                _c = static_cast<shared_ptr<Calc_base>>(new PlusCalc(lhs, rhs));
            }else if(*it1 == '-')
            {
                string rhs(it1 + 1, stmp.end());
                string lhs(stmp.begin(), it1);
                _c = shared_ptr<Calc_base>(new SubCalc(lhs, rhs));
            }
        }
    }
}

void test0()
{
    cout << (Calc("5") + Calc("3") * Calc("6")).eval().getNum() << endl;
    cout << (Calc("0") - ((Calc("3") + Calc("6")) * Calc("5"))).eval().getNum() << endl;
}


void test1()
{
    cout << "加法----------------------" << endl;
    string s1("5+3");
    cout << Calc(s1).eval().getNum() << endl;
    cout << "55 = "<< Calc("55").eval().getNum() << endl;
    cout << "(9+0)+5 = " << Calc("(9+0)+5").eval().getNum() << endl;
    cout << "9+(0+5) = " << Calc("9+(0+5)").eval().getNum() << endl;
    cout << "(9+5)+9+5+(9+5)+9 = " << Calc("(9+5)+9+5+(9+5)+9").eval().getNum() << endl;
}

void test2()
{
    cout << "减法---------------------" << endl;
    cout << "9-5 = " << Calc("9-5").eval().getNum() << endl;
    cout << "9+3-5 = " << Calc("9+3-5").eval().getNum() << endl;
    cout << "9-5+3 = " << Calc("9-5+3").eval().getNum() << endl;
    cout << "9-(5+3) = " << Calc("9-(5+3)").eval().getNum() << endl;
    cout << "(9-5)+3 = " << Calc("(9-5)+3").eval().getNum() << endl;
    cout << "9-5-3 = " << Calc("9-5-3").eval().getNum() << endl;
}

int main()
{
    test0();
    test1();
    test2();
    return 0;
}
