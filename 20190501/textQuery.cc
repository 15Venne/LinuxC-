

#include<iostream>
#include<set>
#include<vector>
#include<string>
#include<map>
#include<memory>
#include<fstream>
#include<sstream>
#include<algorithm>

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;
using std::map;
using std::shared_ptr;
using std::ifstream;
using std::istringstream;
using std::ostream;
using std::make_shared;



class QueryResult;

class TextQuery
{
public:
    using line_no = vector<string>::size_type;
    TextQuery(ifstream &);
    void stringDeal(string &s);
    QueryResult query(const string &) const; //处理输入单词，返回一个QueryResult
private:
    shared_ptr<vector<string>> _file;
    map<string, shared_ptr<set<line_no>>> _wm;
};//

void TextQuery::stringDeal(string &s)
{
    int i=0;
    while(s[i])
    {
        if(isalnum(s[i]))
        {
            if(s[i] >= 'A' && s[i] <= 'Z')
            {
                s[i] += 32;
            }
        }else{
            s[i] = ' ';
        }
        ++i;
    }
}

//构造函数
TextQuery::TextQuery(ifstream & is)
: _file(new vector<string>)
{
    string text;
    while(getline(is, text))
    {
        stringDeal(text);
        _file->push_back(text);
        int n = _file->size() - 1; //行号
        istringstream line(text);
        string word;
        while(line >> word)
        {
            auto &lines = _wm[word];
            if(!lines)
            {
                lines.reset(new set<line_no>);
            }
            lines->insert(n);
        }
    }
}


class QueryResult
{
    friend ostream &print(ostream &, const QueryResult &);
public:
    using line_no = vector<string>::size_type;
    QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f)
    : _sought(s), _lines(p), _file(f)
    {  }
    set<line_no>::iterator begin()
    {
        return _lines->begin();
    }
    set<line_no>::iterator end()
    {
        return _lines->end();
    }
    shared_ptr<vector<string>> get_file()
    {
        return _file;
    }
private:
    string _sought;
    shared_ptr<set<line_no>> _lines;
    shared_ptr<vector<string>> _file;
};//

//处理单词,真正唯一查找的操作， 返回一个结果(获取行号)
QueryResult TextQuery::query(const string &sought) const
{
    static shared_ptr<set<line_no>> nodata(new set<line_no>); //单词不存在用，行数为空的set
    auto loc = _wm.find(sought);
    if(loc == _wm.end())
    {
        return QueryResult(sought, nodata, _file);//未找到
    }else{
        return QueryResult(sought, loc->second, _file);
    }                      //单词     行数     文件内容
}

//打印
ostream &print(ostream &os, const QueryResult &qr)
{
    os << qr._sought << " occurs " << qr._lines->size() << " times" << endl;//s
    for(auto num : *qr._lines)
    {
        os<< "(line " << num + 1 << ") " 
          << *(qr._file->begin() + num) << endl;
    }
    return os;
}

class Query_base
{
    friend class Query;
protected:
    using line_no = TextQuery::line_no; //用于eval
    
    virtual
    ~Query_base() = default;
private:
    virtual 
    QueryResult eval(const TextQuery &) const = 0; //返回与当前Query匹配的QueryResult
    
    virtual 
    string rep() const = 0; //表示查询的一个string
};//

class Query
{
    friend ostream &operator<<(ostream &os, const Query &query);
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query &);
    friend Query operator&(const Query &, const Query &);
public:
    Query(const string &);
    QueryResult eval(const TextQuery &t) const
    {
        return _q->eval(t);
    }
    string rep() const
    {
        return _q->rep();
    }
private:
    Query(shared_ptr<Query_base> query)
    : _q(query)
    {  }
    shared_ptr<Query_base> _q;

};//对外提供接口，同时隐藏了Query_base的继承体系。
 //每个Query对象都含有一个指向Query_base对象的shared_ptr.

ostream &operator<<(ostream &os, const Query &query)
{
    os << query.rep();
    return os;
}

class WordQuery
: public Query_base
{
    friend class Query;
private:
    WordQuery(const string &s)
    : _word(s) 
    {  }

    QueryResult eval(const TextQuery &t) const
    {
        return t.query(_word); //
    }

    string rep() const
    {
        return _word;
    }

private://数据成员, 要查找的单词
    string _word;
};//单词查询的类，全私有，靠友元Query进行构造

inline
Query::Query(const string &s)
: _q(new WordQuery(s))
{  }

class NotQuery
: public Query_base
{
    friend Query operator~(const Query &);
private:
    NotQuery(const Query &q)
    : _query(q)
    {  }

    QueryResult eval(const TextQuery &t) const
    {
        auto result = _query.eval(t);
        auto ret_lines = make_shared<set<line_no>>();

        auto beg = result.begin();
        auto end = result.end();

        auto sz = result.get_file()->size();
        for(size_t n = 0; n != sz; ++n)
        {
            if(beg == end || *beg != n)
            {
                ret_lines->insert(n);
            }else if(beg != end){
                ++beg;
            }
        }
        return QueryResult(rep(), ret_lines, result.get_file());
    }

    string rep() const
    {
        return "~(" + _query.rep() + ")";  
    }

private:
    Query _query;

};//全私有，理同

//重载~
inline Query operator~(const Query &q)
{
    return shared_ptr<Query_base>(new NotQuery(q)); //含隐式转换
}//

class BinaryQuery
: public Query_base
{
protected:
    BinaryQuery(const Query &lhs, const Query &rhs, string s)
    : _lhs(lhs)
    , _rhs(rhs)
    , _opSym(s)
    {  }
    
    string rep() const
    {
        return "(" + _lhs.rep() + " " + _opSym + " " + _rhs.rep() + ")";
    }

    Query _lhs;
    Query _rhs;
    string _opSym; //运算符的名字
};//Query_base----BinaryQuery----and, or

class AndQuery
: public BinaryQuery
{
    friend Query operator &(const Query &, const Query &);
    AndQuery(const Query &lhs, const Query &rhs)
    : BinaryQuery(lhs, rhs, "&")
    {  }

    QueryResult eval(const TextQuery &t) const
    {
        auto left = _lhs.eval(t);
        auto right = _rhs.eval(t);
        auto ret_lines = make_shared<set<line_no>>();
        //标准库算法, 合并两个set
        std::set_intersection(left.begin(), left.end(), 
                              right.begin(), right.end(),
                              inserter(*ret_lines, ret_lines->begin()));
                                   //set本体
        return QueryResult(rep(), ret_lines, left.get_file());
    }
};//

//重载&
inline Query operator&(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}//

class OrQuery
: public BinaryQuery
{
    friend Query operator|(const Query &, const Query &);
    OrQuery(const Query &lhs, const Query &rhs)
    : BinaryQuery(lhs, rhs, "|")
    {  }
    QueryResult eval(const TextQuery &t) const
    {
        auto right = _rhs.eval(t);
        auto left = _lhs.eval(t);

        auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());
        ret_lines->insert(right.begin(), right.end());

        return QueryResult(rep(), ret_lines, left.get_file());
    }
};//

//重载|
inline Query operator|(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}//



void test0()
{
    ifstream is;
    is.open("china_daily.txt");
    if(!is.good())
    {
        cout << "open file failed" << endl;
        return;
    }
    TextQuery tq(is);
    
    print(cout, tq.query("link"));
    cout << endl;
    print(cout, (Query("link") & Query("that")).eval(tq));
    cout << endl;
    print(cout, (~Query("people") & Query("news")).eval(tq));
    cout << endl;
    print(cout, (Query("before")|Query("already")).eval(tq));
}

int main()
{
    test0();
    return 0;
}

