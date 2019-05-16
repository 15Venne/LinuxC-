

#include"../include/tcphead.h"
#include"../include/threadpool.h"
#include"../include/dicthead.h"
#include"../include/lruCache.h"


Threadpool *gThreadpool = nullptr;
Dictionary dic(Configuration::getInstance());

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
        }else{//相等
            return lhs.getFrequency() < rhs.getFrequency();          
        }
    }
};//

//Task 任务
class Task
{
public:
    Task(const Dictionary &, string, const shared_ptr<TcpConnection> &);//1
    
    int calcDistance(const string &);//2
    string proecss(shared_ptr<Thread> &);//3
private:
    int min_of_three(int a, int b, int c)
    {
        return std::min(std::min(a, b), c);       
    }
private:
    const Dictionary &_dic;
    string _queryWord;
    shared_ptr<std::priority_queue<QueryResult, vector<QueryResult>, Compare>> _que;
    //string _msg;
    shared_ptr<TcpConnection> _conn;
};//

Task::Task(const Dictionary &dic,string queryWord, const shared_ptr<TcpConnection> &conn)
: _dic(dic)
, _queryWord(queryWord)
, _que(new std::priority_queue<QueryResult, vector<QueryResult>, Compare>())
, _conn(conn)
{  
    _queryWord.erase(_queryWord.end() - 1);
};//1

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

string Task::proecss(shared_ptr<Thread> &pt)
{
    //cout << "_queryWord: " << _queryWord << endl;
    //cout << "size=" << _queryWord.size() << endl;
    auto iter = pt->_cache.getCache()->find(_queryWord);
    if(iter != pt->_cache.getCache()->end())
    {
        std::ostringstream oss;
        std::ostringstream ossToCache;
        ossToCache << _queryWord << " ";
        vector<string> tmp;
        tmp = pt->_cache.get(_queryWord);
        for(auto & w : tmp)
        {
            oss << w << '\n';
            ossToCache << w << " ";
        }
        cout << "内存缓存已存在，直接读取" << endl;
        _conn->sendInLoop(oss.str());
        return ossToCache.str();
    }else{
        cout << "缓存不存在，查找字典" << endl;
    }

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

    std::ostringstream oss;
    std::ostringstream ossToCache;
    ossToCache << _queryWord << " ";
    for(i = 0; i < 5 && !_que->empty(); ++i)
    {
        oss << _que->top()._word << '\n';
        ossToCache << _que->top()._word << " ";
        _que->pop();
    }
    _conn->sendInLoop(oss.str());
    return ossToCache.str();
}//3


void onConnection(const shared_ptr<TcpConnection> &conn)
{
    loginfo("%s has connected.", conn->toString().c_str());
    conn->send("welcome to server.");
}

using namespace std::placeholders;
void onMessage(const shared_ptr<TcpConnection> &conn)
{
    //cout << "onMessage..." << endl;
    string msg = conn->receive();
    loginfo(">> receive msg from client: %s", msg.c_str());
    Task task(dic, msg, conn);
    gThreadpool->addTask(std::bind(&Task::proecss, task, _1));
}

void onClose(const shared_ptr<TcpConnection> &conn)
{
    //cout << "onClose..." << endl;
    loginfo("%s has closed.", conn->toString().c_str());
}



int main()
{
    

    Threadpool threadpool(2, 10);
    threadpool.start();
    gThreadpool = &threadpool;

    TcpServer ser("192.168.5.215", 2015);
    ser.setConnectionCallback(onConnection);
    ser.setMessageCallback(onMessage);
    ser.setCloseCallback(onClose);
    
    ser.start();

    return 0;
}
