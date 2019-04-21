
#include<string.h>

#include<iostream>
#include<map>
#include<fstream>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::ifstream;
using std::ofstream;
using std::fstream;

class Dictionary
{
public:
    Dictionary()
    : _pwords(new map<string, int>())
    {
        cout << "Dictionary(map<string, int> *)" << endl;
    }

    void read(const string &filename)
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
        while(ifs >> word)
        {
            if(word[0] >= '0' && word[0] <= '9')
                continue;
            ++(*_pwords)[word];
        }
        ifs.close();
    }

    void store(const string &filename)
    {
        cout << "void store(const string &)" << endl;
        ofstream ofs(filename);
        if(!ofs)
        {
            cout << "ofstream open " << filename << " error!" << endl;
            return;
        }
        for(const auto &w : *_pwords)
        {
            ofs << w.first << " " << w.second << "\n";
        }
        ofs.close();
    }

    void print()
    {

    }

    ~Dictionary()
    {
        delete _pwords;
    }
private:
    map<string, int> *_pwords;
};


int main()
{
    string filename("The_Holy_Bible.txt");
    Dictionary dic;
    dic.read(filename);
    dic.store("Dictionary.txt");
    return 0;
}













