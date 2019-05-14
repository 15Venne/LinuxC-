
#include"head.h"

void onConnection(const shared_ptr<Tcpconnection> &conn)
{
    cout << conn->toString() << " has connectre!" << endl;
    conn->send("welcome to server.");
}

void onMessage(const shared_ptr<Tcpconnection> &conn)
{
    cout << "onMessage..." << endl;
    string msg = conn->receive();
    cout << ">> receive msg from client: " << msg << endl;
    //
    //...
    //
    conn->send(msg);
}

void onClose(const shared_ptr<Tcpconnection> &conn)
{
    cout << "onClose..." << endl;
    cout << conn->toString() << "has closed!" << endl;
}

int main()
{
    Acceptor ac("192.168.5.215", 2015);
    ac.ready();
    
    EventLoop loop(ac);
    loop.setConnectionCallback(onConnection);
    loop.setMessageCallback(onMessage);
    loop.setCloseCallback(onClose);

    loop.loop();
    return 0;
}
