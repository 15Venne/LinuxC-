

#include<iostream>
#include<string>
#include<vector>

using std::vector;
using std::cout;
using std::endl;
using std::string;

class Solution
{
public:
    bool isValid(string s)
    {
        Stack st;
        char tmp;
        int cur=0;
        int ret;
        while(s[cur])
        {
            if(s[cur] == '(' || s[cur] == '[' || s[cur] == '{')
            {
                st.push(s[cur]);
            }
            if(s[cur] == ')' || s[cur] == ']' || s[cur] == '}')
            {   
                ret = st.pop(tmp);
                if(ret == -1)
                {
                    return false;
                }
                if(tmp =='(' && s[cur] != ')')
                    return false;
                else if(tmp == '[' && s[cur] != ']')
                    return false;
                else if(tmp == '{' && s[cur] != '}')
                    return false;
                else
                {  }
            }
            ++cur;
        }
        if(st.empty())
        {
            return true;
        }
        else
            return false;
    }
    class Stack
    {
    public:
        Stack()
        : _top(0)
        { }
        void push(const char &ch)
        {
            _charArr.push_back(ch);
            ++_top;
        }
        int pop(char &ch)
        {
            if(empty())
            { cout << "stack is empty" << endl; return -1; }
            else{
                ch = _charArr[_top-1];
                _charArr.pop_back();
                --_top;
                return 0;
            }
        }
        
        bool empty()
        {
            return _top==0;
        }
    
    private:
        int _top;
        vector<char> _charArr;
    };

private:

};

void test0()
{
    Solution so;
    string s1("()");
    string s2("[shifd(dfd){ddd}]");
    string s3("{ddssdsfa)[]}");
    cout << so.isValid(s1) << endl;
    cout << so.isValid(s2) << endl;
    cout << so.isValid(s3) << endl;
}

int main()
{
    test0();
    return 0;
}
