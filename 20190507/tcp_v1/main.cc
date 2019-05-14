

#include"head.h"

void test0()
{
    Acceptor acceptor("192.168.5.215", 2015);
    acceptor.ready();

    TcpConnection conn(acceptor.accept());//accept()返回的是fd
    cout << conn.toString() << "has connected." << endl;//打印连接信息
    conn.send("Welcome to server.");
    cout << "> pls client data: " << endl;
    cout << conn.receive() << endl;
}

int main()
{
    test0();
    return 0;
}
