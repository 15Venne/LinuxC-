#include<iostream>
#include<log4cpp/Category.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/BasicLayout.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PatternLayout.hh>

using std::cout;
using namespace log4cpp;
int test0()
{
    OstreamAppender *osAppender = new OstreamAppender("osAppender", &cout);
    osAppender->setLayout(new BasicLayout());

    Category &root = Category::getRoot();
    root.addAppender(osAppender);
    root.setPriority(Priority::DEBUG);

    root.error("Hello log4cpp in a Error Message!");
    root.warn("Hello log4cpp in a Warning Message!");

    Category::shutdown();
    return 0;
}

int test1()
{
    PatternLayout *ptn1 = new PatternLayout();
    ptn1->setConversionPattern("%d %c [%p] %m%n");
                            //时间 ？类型  字符串？
    OstreamAppender *pOsAppender = new OstreamAppender("OstreamAppender", &cout);
    pOsAppender->setLayout(ptn1);

    Category &root =Category::getRoot();
    root.setAppender(pOsAppender);
    root.setPriority(Priority::DEBUG);

    root.alert("this is an alert message.");
    root.crit("this is a crit message.");
    root.error("this is an error message.");
    root.warn("this is a warn message.");
    root.info("this is an info message.");
    root.debug("this is a debug message.");
    root.emerg("this is an emerg message.");
    root.fatal("this is a fatal message.");

    Category::shutdown();

    return 0;
}

int main()
{
    //test0();
    test1();
    return 0;
}
