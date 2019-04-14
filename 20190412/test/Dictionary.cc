#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::getline;
using std::istringstream;



typedef struct Node
{
    string word;
    int number;
    Node *next;

    Node()
    :word("")
    ,number(0)
    ,next(nullptr)
    {}

    //~Node(){cout<<"~Node()"<<endl;}

}*pNode;

class LinkList
{
public:
    LinkList()
    : _head(nullptr)
    , _tail(nullptr)
    {}
    bool IsEmpty()
    {
        if(nullptr==_head)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //~LinkList(){cout<<"~LinkList"<<endl;}
    void Insert(string word)
    {
        pNode pnew=new Node();
        pNode pcur=_head;

        pnew->word=word;
        if(IsEmpty())
        {
            _head=pnew;
            _tail=pnew;
        }
        else
        {
            //cout << "t5 " << pcur->word << endl;
            while(pcur)
            {
                //cout<<"t6 "<<pcur->word<<endl;
                if(word==pcur->word)
                {
                    //cout<<"t4 "<<pcur->word<<endl;
                    ++pcur->number;
                    return;
                }
                //cout<<"t7"<<endl;
                pcur=pcur->next;
                //cout<<"t8"<<endl;
            }

            if(nullptr==pcur)
            {
                cout<<pnew->word<<pnew->number<<endl;
                pnew->number=1;
                _tail->next=pnew;
                _tail=pnew;
                return;
            }
        }

    }
    //void Delete();
    void PrintList()
    {
        int total1=0;
        int total2=0;
        if(!IsEmpty())
        {
            pNode pcur=_head;
            cout<<pcur->word<<" "<<pcur->number<<endl;
            //pcur=pcur->next;
            //cout<<pcur->word<<" "<<pcur->number<<endl;
            while(pcur->next)
            {
                cout<<pcur->word<<" "<<pcur->number<<endl;
                ++total1;
                total2+=pcur->number;
                pcur=pcur->next;
            }
            cout<<endl;

            cout<<"单词个数："<<total1<<endl
                <<"总计词数："<<total2<<endl;
        }
    }
private:
    pNode _head;
    pNode _tail;

};

class Dictionary
{
public:
    //string IsWord(const string &line);
    void read(const string &filename);
    void store(const string &filename);
private:
    LinkList _list;
    //string word;
};


//string Dictionary::IsWord(const string &line)
//{
//    for(unsigned int i=0;i<line.length();++i)
//    {
//        //if('A'>line[i] && line[i])
//    }
//}

void Dictionary::read(const string &filename)
{
    string line,word;
    ifstream ifs;
    ifs.open(filename);
    if(!ifs.good())
    {
        cout<<"iFILE "<<filename<<" error!"<<endl;
        return;
    }

    cout<<"Start to read file"<<endl;
    while(getline(ifs,line))
    {
        //cout<<"t3"<<word<<endl;
        istringstream iss(line);
        while(!iss.eof())
        {
            iss >> word;
            if('1' <=word[0] && '9'>=word[0])
            {
                continue;
            }
            //cout<< word <<endl;
            _list.Insert(word);
        }
    }
    cout<<"File read over"<<endl;
    //_list.PrintList();//把打印内容写入到文件中
    ifs.close();
}

void Dictionary::store(const string &filename)
{
    ofstream ofs;
    ofs.open(filename);
    if(!ofs.good())
    {
        cout<<"oFILE "<<filename<<" error!"<<endl;
        return;
    }
    //_list.PrintList();//把打印内容写入到文件中
    
    ofs.close();
}

int main()
{
    string file1,file2;
    file1="test.txt";
    file2="result.txt";

    cout <<"Word Statistics"<<endl;
    Dictionary d1;
    //cout <<"t1"<<endl;
    d1.read(file1);
    //cout <<"t2"<<endl;
    d1.store(file2);
    return 0;
}


