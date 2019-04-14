#include<iostream>
#include<log4cpp/Category.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/BasicLayout.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/StringQueueAppender.hh>
#include<log4cpp/Appender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/RollingFileAppender.hh>

#include<queue>

using std::string;
using std::queue;
using std::cout;
using std::endl;
using std::ostringstream;

using namespace log4cpp;

int test0()
{
    StringQueueAppender *pQAppender = new StringQueueAppender("pQAppender");
    pQAppender->setLayout(new BasicLayout()); //默认格式
    
    Category &root = Category::getRoot();
    root.addAppender(pQAppender);
    root.setPriority(Priority::DEBUG);

    root.error("Hello log4cpp in a error message!");
    root.warn("Hello log4cpp in a warning message!");

    cout << "Get message from memory queue!" << endl;
    cout << "------------------------------" << endl;

    queue<string>& myStrQ = pQAppender->getQueue();
    while(!myStrQ.empty())
    {
        cout << myStrQ.front();
        myStrQ.pop();
    }

    Category::shutdown();
    return 0;
}

int test1()
{
    PatternLayout *pLayout1 = new PatternLayout();
    pLayout1->setConversionPattern("%d: %p %c %x: %m%n");

    PatternLayout *pLayout2 = new PatternLayout();
    pLayout2->setConversionPattern("%d: %p %c %x: %m%n");

    Appender *pfileAppender = new FileAppender("pfileAppender", "wxb.log");
    pfileAppender->setLayout(pLayout1);

    RollingFileAppender *prollfileAppender = new RollingFileAppender("prollfileAppender", "rollwxb.log", 5*1024, 1);
    prollfileAppender->setLayout(pLayout2);

    Category &root = Category::getRoot().getInstance("RootName");
    root.addAppender(pfileAppender);
    root.addAppender(prollfileAppender);
    root.setPriority(Priority::DEBUG);

    for(int i=0;i<100;++i)
    {
        string strError;
        ostringstream oss;
        oss << i << ":Root Error Message!";
        strError = oss.str();
        root.error(strError);
    }

    Category::shutdown();
    return 0;
}

int main()
{
    //test0();
    test1();
    return 0;
}
