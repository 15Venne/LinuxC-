

/*
    读取文件统计，
    输入单词，查询该单词出现次数，行数，行内容



  */


#include<iostream>
#include<vector>
#include<set>
#include<fstream>
#include<map>
#include<sstream>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::set;
using std::map;
using std::ifstream;
using std::istringstream;


class TextQuery
{
public:
    void readFile(const string &filename);
    void query(const string &word);
private:
    vector<string> line; //存储文件内容在vector里
    map<string, set<int>> word2Line;//行号
    map<string, int> dict; //词频
};

void TextQuery::readFile(const string &filename)
{
    cout << "begin to read file..." << endl;
    ifstream ifs;
    ifs.open(filename);
    if(!ifs.good())
    {
        cout << "ifstream open " << filename << " error!" << endl;
        return;
    }
    string gline;
    int linecnt = 0;
    while(std::getline(ifs, gline))
    {
        
        istringstream iss(gline);
        line.push_back(iss.str());
        string word;
        while(iss >> word)
        {
            if(word[0] >= '0' && word[0] <= '9')
                continue;
            ++dict[word];
            word2Line[word].insert(linecnt);
        }
        ++linecnt;
    }
    
    ifs.close();
    cout << "read file completed..." << endl;
}

void TextQuery::query(const string &word)
{
    if(dict.find(word) == dict.end()) //没有此单词，返回
    {
        cout << "can't find this word" << endl << endl;
        return;
    }
    cout << "[" << word << "]" << " occurs " << dict[word]
         << (dict[word] > 1 ? " times":" time") << endl; //打印词频
    for(set<int>::iterator it = word2Line[word].begin(); it != word2Line[word].end(); ++it)  
    {
        cout << "    (" << *it + 1 << ") ";  //打印行号
        cout << line[*it] << endl;   //打印行内容
    }
    cout << endl;
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("argc error\n");
    }

    //string queryWorld;
    string queryWorld;
    TextQuery tq;
    tq.readFile(argv[1]);
    while(cout << "enter a word to search : ", fflush(stdout), cin >> queryWorld)
    {
        tq.query(queryWorld);
    }
    
    return 0;
}
