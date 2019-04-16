#include<iostream>
#include<log4cpp/Category.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/Appender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/PatternLayout.hh>
//输出的日志信息能同时输出到终端和文件
using namespace log4cpp;

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

Category *getFileCategory()
{
    PatternLayout *pLayoutFile = new PatternLayout();
    pLayoutFile->setConversionPattern("%d: [%p] %c %x: %m%n");
    
    PatternLayout *pLayoutCout = new PatternLayout();
    pLayoutCout->setConversionPattern("%d: [%p] %c %x: %m%n");

    Appender *pfileAppender = new FileAppender("pfileAppender", "test.log");
    pfileAppender->setLayout(pLayoutFile);

    OstreamAppender *pOsAppender = new OstreamAppender("pOsAppender", &cout);
    pOsAppender->setLayout(pLayoutCout);

    Category &root = Category::getRoot();
    root.setPriority(Priority::DEBUG);

    Category &subFile = root.getInstance("这里是%c");
    subFile.addAppender(pfileAppender);
    subFile.addAppender(pOsAppender);
    subFile.setPriority(Priority::DEBUG);
    return &subFile;
}

class Mylogger
{
public:
    static Mylogger *getInstance();
    static void Myloggerfree()
    {
        if(_pmylogger)
        {
            Category::shutdown();
            delete _pmylogger;
            _pmylogger = nullptr;
            _pCategory = nullptr;
            cout << "void Myloggerfree()" << endl;
        }
    }
    
    static void warn(const char*);
    static void error(const char*);
    static void debug(const char*);
    static void info(const char*);
private:
    Mylogger()
    {  }
    ~Mylogger()
    {  }
    static Mylogger *_pmylogger;
    static Category *_pCategory;
};

Mylogger *Mylogger::_pmylogger = nullptr;
Category *Mylogger::_pCategory = nullptr;

Mylogger *Mylogger::getInstance()
{
    if(!_pmylogger)
    {
        _pmylogger = new Mylogger();
        _pCategory = getFileCategory();
    }
    return _pmylogger;
}
void Mylogger::warn(const char *msg)
{
    _pCategory->warn(msg);
}
void Mylogger::error(const char *msg)
{
    _pCategory->error(msg);
}
void Mylogger::debug(const char *msg)
{
    _pCategory->debug(msg);
}
void Mylogger::info(const char *msg)
{
    _pCategory->info(msg);
}

int main()
{
    cout << "hello,world" << endl;
    Mylogger *log = Mylogger::getInstance();
    char s1[100]={0};
    sprintf(s1, "hello filename:%s funcname:%s linenum:%d", __FILE__, __func__, __LINE__);
    log->warn(s1);
    log->error(s1);
    log->debug(s1);
    log->info(s1);
    //Category::shutdown();
    //cout << "已shutdown()" << endl;
    log->Myloggerfree();
    return 0;
}
