

#include"tcphead.h"
#include"threadpool.h"


Threadpool *gThreadpool = nullptr;

class Quest
{
public:
    Quest(const string &msg, const shared_ptr<TcpConnection> &conn)
    : _msg(msg)
    , _conn(conn)
    {  }

    void process()
    {
        string response = _msg;
        _conn->sendInLoop(response);
    }
private:
    string _msg;
    shared_ptr<TcpConnection> _conn;
};//

void onConnection(const shared_ptr<TcpConnection> &conn)
{
    cout << conn->toString() << "has connected." << endl;
    conn->send("welcome to server.");
}

void onMessage(const shared_ptr<TcpConnection> &conn)
{
    cout << "onMessage..." << endl;
    string msg = conn->receive();
    cout << ">> receive msg from client: " << msg << endl;
    Quest quest(msg, conn);
    gThreadpool->addTask(std::bind(&Quest::process, quest));
}

void onClose(const shared_ptr<TcpConnection> &conn)
{
    cout << "onClose..." << endl;
    cout << conn->toString() << " has closed." << endl;
}


int main()
{
    Threadpool threadpool(4, 10);
    threadpool.start();
    gThreadpool = &threadpool;

    TcpServer ser("192.168.5.215", 2015);
    ser.setConnectionCallback(onConnection);
    ser.setMessageCallback(onMessage);
    ser.setCloseCallback(onClose);
    
    ser.start();

    return 0;
}
