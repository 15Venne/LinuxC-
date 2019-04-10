#include<iostream>

using std::cout;//声明机制
using std::endl;

//int num=1000;

namespace wd
{
void display();
}

namespace  tls
{

//int num=11;
void display()
{
    cout<<"tls::display()这个第二"<<endl;
    wd::display();
}
}//end of namespace tls;

namespace wd
{

//int num = 500;
void show()
{
    cout<<"wd::show()这个第一"<<endl;
    tls::display();
}

void display()
{
    cout<<"wd::display()这个第三"<<endl;
}

}//end of namespace wd;

int main()
{
    wd::show();
    return 0;
}
