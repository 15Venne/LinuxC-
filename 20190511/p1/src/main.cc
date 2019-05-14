
#include"../include/head.h"

/* class Test */
/* { */
/* public: */
/*     Test(Task &task) */
/*     : _task(task) */
/*     {  } */
/*     void testCalcdis(string word) */
/*     { */
/*         cout << _task.calcDistance(word); */
/*     } */
/* private: */
/*     Task &_task; */
/* }; */

void test0()
{
    Configuration *p = Configuration::getInstance();
    Dictionary dic(p);
    cout << dic.wordCnt() << endl;
    string word;
    Task ta(dic, word);
    while(cout << "search: ", fflush(stdout), cin >> word)
    {
        ta.proecss(word);
        cout << endl;
    }

}

/* void test1() */
/* { */
/*     Configuration *p = Configuration::getInstance(); */
/*     Dictionary dic(p); */
/*     string word("iphone"); */
/*     Task ta(dic, word); */
/*     Test te(ta); */
/*     string testWord; */
/*     while(cout << "enter testWord: " << endl, cin >> testWord) */
/*     { */
        /* te.testCalcdis(testWord); */
        /* cout << endl; */
    /* } */
/* } */

int main()
{
    test0();
    //test1();
    return 0;
}
