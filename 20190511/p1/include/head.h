

#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<dirent.h>
#include<pthread.h>


#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<memory>
#include<set>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<algorithm>
#include<queue>

using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::string;
using std::shared_ptr;
using std::set;
using std::pair;
using std::unordered_map;
using std::cin;

//---------------------------------------------------------------------------------------
//conf
class Configuration
{
public:
    static Configuration *getInstance()
    {
        if(!_pInstance)
        {
            _pInstance = new Configuration();
        }
        return _pInstance;
    }
    vector<string> &getFileList()
    {
        return _fileList;
    }
private:
    Configuration();//1
    ~Configuration(){}
    static Configuration *_pInstance;
private:
    string _ip;
    string _port;
    string _mydict;
    string _index;
    vector<string> _fileList;
};//

Configuration::Configuration()
{
    std::ifstream is;
    is.open("../conf/my.conf");
    if(!is.good())
    {
        cout << "open file failed" << endl;
        return;
    }
    string line, key, value;
    while(getline(is, line))
    {
        std::istringstream iss(line);
        while(iss >> key, iss >> value)
        {
            switch(atoi(key.c_str()))
            {
            case 1:
                _ip = value; break;
            case 2:
                _port = value; break;
            case 3:
                _mydict = value; break;
            case 4:
                _index = value; break;
            case 5:
                _fileList.push_back(value);
                break;
            default: break;
            }
        }
    }
    cout << _ip << endl;
    cout << _port << endl;
    cout << _mydict << endl;
    cout << _index << endl;
}

Configuration *Configuration::_pInstance = Configuration::getInstance();
//----------------------------------------------------------
//Dictionary字典
class Dictionary
{
    friend class Task;
public:
    Dictionary(Configuration *);//1

    int wordCnt()
    {
        return _wordCnt;
    }
private:
    void readConf(Configuration *);//2
    void readFile(string &);//3
    void makeDict();//4
    void makeIndexTable();//5
    void stringDeal(string &);//6
private:
    map<string, int> _dic; //统计
    int _wordCnt;
    vector<pair<string, int>> _dict;//字典
    unordered_map<string, set<int>> _indexTable;//索引表
};//

Dictionary::Dictionary(Configuration *p)
: _dic()
, _wordCnt(0)
{
    readConf(p);
    makeDict();
    makeIndexTable();
    cout << _dic.size() << endl;
    cout << _dict.size() << endl;
    cout << _indexTable.size() << endl;
}//1

void Dictionary::readConf(Configuration *p) 
{
    for(auto &filename : p->getFileList())
    {
        readFile(filename);
        cout << filename << endl;
    }
}//2

void Dictionary::readFile(string &filename) 
{
    std::ifstream is;
    is.open(filename);
    if(!is.good())
    {
        cout << "open file failed" << endl;
        return;
    }
    string text;
    while(getline(is, text))
    {
        stringDeal(text);
        string word;
        std::istringstream line(text);
        while(line >> word)
        {
            _dic[word]++;
            _wordCnt++;
            //cout << word << ": " << _dic[word] << endl;
        }
    }
}//3

void Dictionary::makeDict()
{
    for(auto &e : _dic)
    {
        _dict.push_back(make_pair(e.first, e.second));
    }
}//4

void Dictionary::makeIndexTable()
{
    string ind("a");
    int i, j;
    for(j = 0; j < _dict.size(); ++j ) //vector<pair<string, int>>
    {
        for(i = 0; i < _dict[j].first.size(); ++i)
        {
            ind[0] = _dict[j].first[i];
            _indexTable[ind].insert(j);    //unordered_map<string, set<int>> _indexTable;       
        }
    }
}//5

void Dictionary::stringDeal(string &s)
{
    int i = 0;
    while(s[i])
    {
        if(isalpha(s[i]))
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
}//6
//--------------------------------------------------------------------------
//候选词结果 QueryResult
class QueryResult
{
    friend class Task;
    
public:
    QueryResult(int dis, string word, int fre)
    : _distance(dis)
    , _word(word)
    , _frequency(fre)
    {  }
    
    int getDistance() const
    {
        return _distance;
    }
    int getFrequency() const
    {
        return _frequency;
    }

private:
    int _distance;
    string _word;
    int _frequency;
};//

//---------------------------------------------------------------------------

class Compare
{
public:
    bool operator()(const QueryResult &lhs,const QueryResult &rhs) const
    {
        if(lhs.getDistance() > rhs.getDistance())
        {
            return true;
        }else if(lhs.getDistance() < rhs.getDistance()){
            return false;
        }else{
            return lhs.getFrequency() < rhs.getFrequency();          
        }
    }
};//

//Task 任务
class Task
{
public:
    Task(const Dictionary &, string );//1
    
    int calcDistance(const string &);//2
    void proecss(string );//3

private:
    

    int min_of_three(int a, int b, int c)
    {
        return std::min(std::min(a, b), c);       
    }
private:
    const Dictionary &_dic;
    string _queryWord;
    shared_ptr<std::priority_queue<QueryResult, vector<QueryResult>, Compare>> _que;
};//


Task::Task(const Dictionary &dic,string queryWord)
: _dic(dic)
, _queryWord(queryWord)
, _que(new std::priority_queue<QueryResult, vector<QueryResult>, Compare>())
{  };//1

int Task::calcDistance(const string &dictWord)//Levenshtein
{
    int arr[dictWord.size() + 1][_queryWord.size() + 1] = {0};
    int i,j;
    for(i = 1; i <= _queryWord.size(); ++i)
    {
        arr[0][i] = i;
    }
    for(i = 1; i <= dictWord.size(); ++i)
    {
        arr[i][0] = i;
    }
    
    for(i = 1; i <= dictWord.size(); ++i)
    {
        for(j = 1; j <= _queryWord.size(); ++j)
        {
            int flag = 0;
            if(dictWord[i - 1] != _queryWord[j - 1])
            {   flag = 1;  }
            arr[i][j] = min_of_three(arr[i][j - 1], arr[i - 1][j - 1], arr[i - 1][j]) + flag;
        }
    }
    return arr[dictWord.size()][_queryWord.size()];
}//2

void Task::proecss(string queryWord)
{
    _queryWord = queryWord;
    _que.reset(new std::priority_queue<QueryResult, vector<QueryResult>, Compare>());
    int i = 0;
    set<int> tmpset;
    while(_queryWord[i])
    {
        string tmp(1, _queryWord[i]);
        auto idx = _dic._indexTable.find(tmp);
        if(idx != _dic._indexTable.end())
        {
            for(auto & w : idx->second)
            {
                tmpset.insert(w);   
            }
        }
        ++i;
    }
    //cout << tmpset.size() << endl;
    for(auto & w : tmpset)
    {
        string tmpWord = _dic._dict[w].first;
        int tmpfre = _dic._dict[w].second; 
        int tmpdis = calcDistance(tmpWord);
        _que->push(QueryResult(tmpdis, tmpWord, tmpfre));
    }

    for(i = 0; i < 5 && !_que->empty(); ++i)
    {
        cout << _que->top()._word << endl;
        //cout << _que->top()._distance << endl;
        //cout << _que->top()._frequency << endl;
        _que->pop();
    }    
}//3
//------------------------------------------------------------------------------













