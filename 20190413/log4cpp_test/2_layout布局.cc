#include<iostream>
#include<log4cpp/Category.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PatternLayout.hh>
using std::cout;
using namespace log4cpp;

int test0()
{
    PatternLayout *pLayout = new PatternLayout();
    pLayout->setConversionPattern("%d: %c [%p] %x:%m%n"); // 设置日志的输出格式
    //%d 日期进一步格式:%d{%H:%M:%S,%I} 或者 %d{%d %m %Y %H:%M:%S,%I}
    //%c Category
    //%p 优先级
    //%x NDC    ??
    //%m 消息
    //%n 换行符
    //%r 自从layout被创建后的毫秒数
    //%R 离1970.1.1总秒数
    //%u 进程开始到目前为止的时钟周期书
    OstreamAppender *pOsAppender = new OstreamAppender("osAppender", &cout);
    //OstreamAppender 输出到一个ostream类
    //FileAppender 输出到文件
    //RollingFileAppender 输出到回卷文件，即当文件到达某个大小后回卷
    //StringQueueAppender 内存队列
    //Win32DebugAppender 发送到缺省系统调试器
    //NTEventLogAppender 发送到win事件日志
    pOsAppender->setLayout(pLayout);
    
//Category、Appender和Layout三者的关系如下：
//系统中可以有多个Category，它们都是继承自同一个根，
//每个Category负责记录自己的日志;每个Category可以添加多个Appender，
//每个Appender指定了一个日志的目的地，例如文件、字符流或者Windows日志，
//当Category记录一条日志时，该日志被写入所有附加到此Category的Appender;
//每个Append都包含一个Layout，该Layout定义了这个Appender上日志的格式
    Category &root = Category::getRoot();
    Category &infoCategory = root.getInstance("infoCategory"); //
    infoCategory.addAppender(pOsAppender);
    infoCategory.setPriority(Priority::INFO);

    infoCategory.info("system is running.");
    infoCategory.warn("system has a warning.");
    infoCategory.error("system has an error, can't find a file.");
    infoCategory.fatal("system has a fatal error, must be shutdown.");
    infoCategory.info("system shutdown, you can find some information in systemlog");

    Category::shutdown();

    return 0;
}

int main()
{
    test0();
    return 0;
}
