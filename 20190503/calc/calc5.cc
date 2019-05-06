

/*

         class Calc_base                            
        |              |                    friend
class NumCalc       class BinaryCalc        -------                 class Calc                                            
                        |                                    member: shared_ptr<Calc_base> _c 
    class: PlusCalc, SubCalc, MutiCalc, DiviCalc                
                                                                 construtor(string &s)
         construtor(string &lhs, string &rhs)                       { 
                                                                            s
                                                                            |
                                                                         |     |
                                                                         s1    s2
                                                                         |     |
                                                                            |
                                                    _c = shared_ptr<Calc_base>(new (BinaryCalc cons)(s1, s2))
                                                                    }
*/




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
using std::stack;

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
public:
    Calc(const string &);
    CalcResult eval() const
    {
        return _c->eval();
    }
private:
    shared_ptr<Calc_base> _c;
};//


class NumCalc
: public Calc_base
{
    friend class Calc;
private:
    NumCalc(const string &s) 
    : _num(atoi(s.c_str()))
    {  }

    CalcResult eval() const
    {
        return CalcResult(_num);
    }
private:
    int _num;
};//


class BinaryCalc    
: public Calc_base
{
    friend class Calc;
protected:
    BinaryCalc(const Calc &lhs, const Calc &rhs)
    : _lhs(lhs), _rhs(rhs)
    {  }
    Calc _lhs;
    Calc _rhs;
};//

class PlusCalc
: public BinaryCalc
{
    friend class Calc;
    PlusCalc(const string &lhs, const string &rhs)
    : BinaryCalc(Calc(lhs), Calc(rhs))
    {  }
    CalcResult eval() const
    {
        return CalcResult(_lhs.eval().getNum() + _rhs.eval().getNum());
    }
};//


class SubCalc
: public BinaryCalc
{
    friend class Calc;
    SubCalc(const string &lhs, const string &rhs)
    : BinaryCalc(Calc(lhs), Calc(rhs))
    {  }
    CalcResult eval() const
    {
        return CalcResult(_lhs.eval().getNum() - _rhs.eval().getNum());
    }
};//


class MutiCalc
: public BinaryCalc
{
    friend class Calc;
    MutiCalc(const string &lhs, const string &rhs)
    : BinaryCalc(Calc(lhs), Calc(rhs))
    {  }
    CalcResult eval() const
    {
        return CalcResult(_lhs.eval().getNum() * _rhs.eval().getNum());
    }
};//


class DiviCalc
: public BinaryCalc
{
    friend class Calc;
    DiviCalc(const string &lhs, const string &rhs)
    : BinaryCalc(Calc(lhs), Calc(rhs))
    {  }
    CalcResult eval()const
    {
        return CalcResult(_lhs.eval().getNum() / _rhs.eval().getNum());
    }
};//



Calc::Calc(const string &s) // 有70行左右的重复代码，将就不封装了
{
    bool isNum = true;
    int i;
    string stmp = s;
    //static Calc_base *cbase(new NumCalc("0"));
    if(s.size() == 0)
    {
        _c = static_cast<shared_ptr<Calc_base>>(new NumCalc("0"));
        goto End;
    }
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
            _c = static_cast<shared_ptr<Calc_base>>(new NumCalc("0"));
            goto End;
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
                    _c = static_cast<shared_ptr<Calc_base>>(new NumCalc("0"));
                    goto End;
                }
                if(it1 == stmp.begin())
                {
                    stmp.erase(stmp.end() - 1);
                    stmp.erase(stmp.begin());
                    goto Loop;
                }
            }else
            {
                while(isdigit(*it1))
                {
                    it1--;
                }
            }

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
            }else if(*it1 == '*')
            {
                auto it2 = it1;
                while(it2 != stmp.begin())
                {
                    it2--;
                    if(*it2 == ')')
                    {
                        stack<char> st;
                        st.push(*it2);
                        while(it2 != stmp.begin() && !st.empty())
                        {
                            it2--;
                            if(*it2 == ')')
                            {  
                                st.push(*it2); 
                            }
                            else if(*it2 == '(')
                            {
                                st.pop();
                            }
                        }
                        if(it2 != stmp.begin())
                        {
                            it2--;
                        }
                        if(!st.empty())
                        {
                            cout << "字符串非法, 无法计算" << endl;
                            _c = static_cast<shared_ptr<Calc_base>>(new NumCalc("0"));
                            goto End;
                        }
                    }
                    if(isdigit(*it2))
                    {
                       continue; 
                    }else if(*it2 == '+')
                    {   
                        string rhs(it2 + 1, stmp.end());
                        string lhs(stmp.begin(), it2);
                        _c = static_cast<shared_ptr<Calc_base>>(new PlusCalc(lhs, rhs));
                        goto End;
                    }else if(*it2 == '-')
                    {
                        string rhs(it2 + 1, stmp.end());
                        string lhs(stmp.begin(), it2);
                        _c = shared_ptr<Calc_base>(new SubCalc(lhs, rhs));
                        goto End;
                    }
                }
                string rhs(it1 + 1, stmp.end());
                string lhs(stmp.begin(), it1);
                _c = shared_ptr<Calc_base>(new MutiCalc(lhs, rhs));
            }else if(*it1 == '/')
            {
                auto it2 = it1;
                while(it2 != stmp.begin())
                {
                    it2--;
                    if(*it2 == ')')
                    {
                        stack<char> st;
                        st.push(*it2);
                        while(it2 != stmp.begin() && !st.empty())
                        {
                            it2--;
                            if(*it2 == ')')
                            {  
                                st.push(*it2); 
                            }
                            else if(*it2 == '(')
                            {
                                st.pop();
                            }
                        }
                        if(it2 != stmp.begin())
                        {
                            it2--;
                        }
                        if(!st.empty())
                        {
                            cout << "字符串非法, 无法计算" << endl;
                            _c = static_cast<shared_ptr<Calc_base>>(new NumCalc("0"));
                            goto End;
                        }
                    }
                    if(isdigit(*it2))
                    {
                       continue; 
                    }else if(*it2 == '+')
                    {   
                        string rhs(it2 + 1, stmp.end());
                        string lhs(stmp.begin(), it2);
                        _c = static_cast<shared_ptr<Calc_base>>(new PlusCalc(lhs, rhs));
                        goto End;
                    }else if(*it2 == '-')
                    {
                        string rhs(it2 + 1, stmp.end());
                        string lhs(stmp.begin(), it2);
                        _c = shared_ptr<Calc_base>(new SubCalc(lhs, rhs));
                        goto End;
                    }
                }
                string rhs(it1 + 1, stmp.end());
                string lhs(stmp.begin(), it1);
                _c = shared_ptr<Calc_base>(new DiviCalc(lhs, rhs));
            }else
            {
                cout << "字符串非法, 无法计算" << endl;
                _c = static_cast<shared_ptr<Calc_base>>(new NumCalc("0"));
                goto End;
            }
        }
    }
End: 
    {  }
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
    cout << "32d55-() = " << Calc("32d55-()").eval().getNum() << endl;
}

void test3()
{
    cout << "乘法-------------------" << endl;
    cout << "5*4 = " << Calc("5*4").eval().getNum() << endl;
    cout << "3+5*0 = " << Calc("3+5*0").eval().getNum() << endl;
    cout << "(3+5)*5 = " << Calc("(3+5)*5").eval().getNum() << endl;
    cout << "0*(3+5) = " << Calc("0*(3+5)").eval().getNum() << endl;
    cout << "9-5*(4+3)*2 = " << Calc("9-5*(4+3)*2").eval().getNum() << endl;
}

void test4()
{
    cout << "除法-------------------" << endl;
    cout << "6/3 = " << Calc("6/3").eval().getNum() << endl;
    cout << "3/6 = " << Calc("3/6").eval().getNum() << endl;
    cout << "5+9/3-4*(2+3)/5 = " << Calc("5+9/3-4*(2+3)/5").eval().getNum() << endl;
    cout << "(15/3+2)*(7+7*2)/3 = " << Calc("(15/3+2)*(7+7*2)/3").eval().getNum() << endl;
}
int main()
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}
