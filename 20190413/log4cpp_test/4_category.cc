#include<iostream>
#include<log4cpp/Category.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/BasicLayout.hh>
#include<log4cpp/Priority.hh>

using namespace log4cpp;
using std::cout;

int test0()
{
    OstreamAppender *pOsAppender1 = new OstreamAppender("pOsAppender1", &cout);
    pOsAppender1->setLayout(new BasicLayout());

    OstreamAppender *pOsAppender2 = new OstreamAppender("pOsAppender2", &cout);
    pOsAppender2->setLayout(new BasicLayout());

    Category &root = Category::getRoot();
    root.setPriority(Priority::DEBUG);

    Category &sub1 = root.getInstance("sub1");
    sub1.addAppender(pOsAppender1);
    sub1.setPriority(Priority::DEBUG);
    sub1.error("sub error");

    Category &sub2 = root.getInstance("sub2");
    sub2.addAppender(pOsAppender2);
    sub2.setPriority(101);
    sub2.warn("sub2.warning");
    sub2.fatal("sub2 fatal");
    sub2.alert("sub2 alert");
    sub2.crit("sub2 crit");

    Category::shutdown();
    return 0;
}

int main()
{
    test0();
    return 0;
}
