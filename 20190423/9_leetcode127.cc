#include<iostream>
#include<vector>
#include<map>
#include<string>

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;

class Solution
{
public:
    
    bool isNeighbour(const string &s1,const string &s2)
    {
        int cnt=0;
        int cur=0;
        while(s1[cur])
        {
            if(s1[cur] != s2[cur])
            {
                ++cnt;
                if(cnt>1)
                    return false;
            }
            ++cur;
        }
        if(cnt <= 1)
            return true;
    }

    int wordCompare(const string &word,const string &endWord, map<string, int> &wordMap,int val)
    {
        for(auto it = wordMap.begin(); it != wordMap.end(); ++it)
        {
            if(isNeighbour(word, it->first) && it->second == 0)
            {
                if(it->first == endWord)
                {
                    return _cnt;
                }else{
                    it->second = val;
                    ++_size;
                    if(_size == _maxSize)
                        return 0;
                }
            }
        }
        return -1;
    }

    int ladderLength(string beginWord, string endWord, vector<string> &wordList)
    {
        _cnt = 1;
        if(beginWord == endWord)
            return _cnt;
        //_cnt++;
        map<string, int> wordMap;
        for(auto it = wordList.begin(); it != wordList.end(); ++it )
        {
            wordMap[*it] = 0;
        }
        _maxSize = wordList.size();
        _size = 0;
        int ret;
        ret = wordCompare(beginWord, endWord, wordMap, _cnt + 1);
        if(ret == 0)
            return 0;
        if(ret != -1)
            return ret+1;
        int sizetmp = 0;
        _cnt++;
        while(_size != _maxSize)
        {
            
            sizetmp = _size;
            for(auto it = wordMap.begin(); it != wordMap.end(); ++it)
            {
                if(it->second == _cnt)
                {
                    ret = wordCompare(it->first, endWord, wordMap, _cnt + 1);
                    if(ret == 0)
                    {
                        return 0;
                    }
                    if(ret != -1)
                    {
                        return ret + 1;
                    }
                }
            }
            ++_cnt;
            if(sizetmp == _size)
            {
                break;
            }
        }

        return 0;
    }
private:
    int _cnt;
    int _maxSize;
    int _size;
};

void test0()
{
    string beginWord("hit");
    string endWord("cog");
    vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog", "hit"};
    vector<string> wordList2 = {"hot", "dot", "dog", "lot", "log"};
    Solution so;
    cout << so.ladderLength(beginWord, endWord, wordList) << endl;
    //cout << so.ladderLength(beginWord, endWord, wordList2) << endl;
}

void test1()
{
    string s1("a");
    string s2("c");
    vector<string> wordList = {"a", "b", "c"};
    Solution so;
    cout << so.ladderLength(s1, s2, wordList) << endl;
}

void test2()
{
    string s1("hot");
    string s2("dog");
    vector<string> b ={"hot", "dog"};
    Solution so;
    cout << so.ladderLength(s1, s2, b) << endl;
}
int main()
{
    test0();
    test1();
    //test2();
    return 0;
}
