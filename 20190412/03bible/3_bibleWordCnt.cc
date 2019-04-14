
#include<string.h>

#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::fstream;

typedef struct wordNode{
    string word;
    int cnt;
}wordNode;

class Dictionary
{
public:
    Dictionary(vector<wordNode> *pwords)
    : _pwords(pwords)
    {
        cout << "Dictionary(vector<wordNode>*)" << endl;
    }
    void read(const string & filename);
    void store(const string & filename);
    void print()
    {
        cout << "void print()" << endl;
        for(auto & elem : *_pwords)
        {
            cout << elem.word << " " << elem.cnt << endl;
        }
    }
private:
    vector<wordNode> *_pwords;
};

bool sortFunc(const wordNode &p1, const wordNode &p2)
{
    return p1.word < p2.word;
}

void Dictionary::read(const string & filename)
{
    cout << "void read(const string &)" << endl;
    ifstream ifs;
    ifs.open(filename);
    if(!ifs.good())
    {
        cout << "ifstream open" << filename << " error!" << endl;
        return;
    }
    string word;
    int flag = 0;
    while(ifs >> word)
    {
        //cout << word << endl;
        if(word[0]>='0' && word[0]<='9')
            continue;
        for(auto & elem : *_pwords)
        {
            if(word == elem.word)
            {
                elem.cnt++;
                flag = 1;   
                break;
            }
        }
        if(0 == flag)
        {
            wordNode wordtmp;
            wordtmp.word = word;
            wordtmp.cnt = 1;
            (*_pwords).push_back(wordtmp);
        }    
        flag = 0;
    }
    sort((*_pwords).begin(), (*_pwords).end(), sortFunc);
    ifs.close();
}
void Dictionary::store(const string & filename)
{
    cout << "void store(const string &)" << endl;
    ofstream ofs(filename);
    if(!ofs)
    {
        cout << "ofstream open " << filename << "error!" << endl;
        return;
    }
    for(size_t idx = 0; idx != (*_pwords).size(); ++idx)
    {
        ofs << (*_pwords)[idx].word << " " << (*_pwords)[idx].cnt << "\n";
    }
    ofs.close();
}

int main()
{
    string filename("/home/venne/Linux/c++/20190412/03bible/The_Holy_Bible.txt");
    vector<wordNode> words;
    words.reserve(12600);
    Dictionary bible(&words);
    bible.read(filename);
    bible.store("/home/venne/Linux/c++/20190412/03bible/Dictionary1.txt");
    //bible.print();
    return 0;
}
