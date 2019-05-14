

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>

#include<iostream>
#include<string>
#include<memory>
#include<functional>
#include<sstream>
#include<vector>
#include<map>

using std::cout;
using std::endl;
using std::string;
using std::function;
using std::shared_ptr;
using std::vector;
using std::map;

class Noncopyable
{
protected:
    Noncopyable(){}
    ~Noncopyable(){}
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable &operator=(const Noncopyable&) = delete;
};//
//-------------------------------------------------------------------------
//InetAddress 地址，port
class InetAddress
{
public:
    explicit
    InetAddress(unsigned short);//1
    InetAddress(const string &, unsigned short);//2
    InetAddress(const struct sockaddr_in &);//3

    string ip() const;//4
    unsigned short port() const;//5
    struct sockaddr_in *getInetAddressPtr()
    {
        return &_addr;
    }
private:
    struct sockaddr_in _addr;
};//

InetAddress::InetAddress(unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;//代表本机地址
}//1
InetAddress::InetAddress(const string &ip, unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}//2
InetAddress::InetAddress(const struct sockaddr_in &addr)
: _addr(addr)
{  }//3

string InetAddress::ip() const
{
    return string(::inet_ntoa(_addr.sin_addr));
}//4
unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
}//5
//-------------------------------------------------------------------------
//Socket套接字
class Socket
{
public:
    Socket()
    {
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        if(-1 == _fd)
        {
            perror("socket");
        }
    }
    explicit
    Socket(int fd)
    : _fd(fd)
    {  }
    int fd() const
    {
        return _fd;
    }
    void shutdownWrite()
    {
        ::shutdown(_fd, SHUT_WR);
    }
    ~Socket()
    {
        ::close(_fd);
    }
private:
    int _fd;
};//
//--------------------------------------------------------------------------------
//SocketIO 读与写
class SocketIO
{
public:
    explicit
    SocketIO(int fd)
    : _fd(fd)
    {  }
    int readn(char*, int);//1
    int readline(char*, int);//2
    int writen(const char*, int);//3
private:
    int recvPeek(char*, int);//4
private:
    int _fd;
};//
int SocketIO::readn(char *buf, int len)
{
    int total = 0;
    char *p = buf;
    while(total < len)
    {
        int ret = ::read(_fd, p, len - total);
        if(-1 == ret)
        {
            if(EINTR == errno)
            {
                continue;
            }else{
                perror("readn::read");
                break;
            }
        } else if(0 == ret){
            break;
        } else{
            total += ret;
            p += ret;
        }  
    }
    return total;
}//1
int SocketIO::readline(char *buf, int maxLen)
{
    int left = maxLen - 1;
    char *p = buf;
    int ret;
    int total = 0;
    while(left > 0)
    {
        ret = recvPeek(p, left);
        for(int i = 0; i != ret; ++i)
        {
            if(p[i] == '\n')
            {
                int sz = i + 1;
                readn(p, sz);
                p[sz] = '\0';
                total += sz;
                return total;
            }
        }
        readn(p, ret);
        left -= ret;
        p += ret;
        total += ret;
    }
    *p = '\0';
    return total;
}//2

int SocketIO::writen(const char *buf, int len)
{
    int total = 0;
    const char *p = buf;
    while(total < len)
    {
        int ret = ::write(_fd, p, len - total);
        if(-1 == ret)
        {
            if(EINTR == errno)
            {
                continue;
            }else{
                perror("writen::write");
                break;
            }
        }else if(0 == ret)
        {
            break;
        }else{
            total += ret;
            p += ret;
        }
    }
    return total;
}//3

int SocketIO::recvPeek(char *buf, int len)
{
    int ret;
    do{
        ret = ::recv(_fd, buf, len, MSG_PEEK);
    }while(-1 == ret && errno == EINTR);
    return ret;
}//4
//----------------------------------------------------------------------
//Acceptor 
class Acceptor
{
public:
    Acceptor(unsigned short);//1
    Acceptor(const string &ip, unsigned short port);//2
    void ready();//3
    int accept();//4
    int fd() const;//5
private:
    void setReuseAddr(bool on);//6
    void setReusePort(bool on);//7
    void bind();//8
    void listen();//9
private:
    InetAddress _addr;
    Socket _listenSocket;
};//

Acceptor::Acceptor(unsigned short port)
: _addr(port)
, _listenSocket()
{  }//1
Acceptor::Acceptor(const string &ip, unsigned short port)
: _addr(ip, port)
, _listenSocket()
{  }//2
void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}//3
int Acceptor::accept()
{
    int peerfd = ::accept(_listenSocket.fd(), NULL, NULL);
    if(-1 == peerfd)
    {
        perror("accept");
    }
    return peerfd;
}//4
int Acceptor::fd() const
{
    return _listenSocket.fd();
}//5
void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    if(setsockopt(_listenSocket.fd(),
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  &one,
                  sizeof(one)) < 0)
    {
        perror("setReuseAddr::setsockopt");
    }
}//6
void Acceptor::setReusePort(bool on)
{
    int one = on;
    if(setsockopt(_listenSocket.fd(),
                  SOL_SOCKET,
                  SO_REUSEPORT,
                  &one,
                  sizeof(one)) < 0)
    {
        perror("setReusePort::setsockopt");
    }
}//7
void Acceptor::bind()
{
    int ret = ::bind(_listenSocket.fd(),
                     (struct sockaddr*)_addr.getInetAddressPtr(),
                     sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind");
    }
}//8
void Acceptor::listen()
{
    int ret = ::listen(_listenSocket.fd(), 10);
    if(-1 == ret)
    {
        perror("listen");
    }
}//9
//------------------------------------------------------------------------
//Tcpconnection 客户的连接
class Tcpconnection
: public Noncopyable
, public std::enable_shared_from_this<Tcpconnection>
{
public:
    Tcpconnection(int);//1
    /* ~Tcpconnection();//2 */

    string receive();//3
    void send(const string &);//4
    string toString() const;//5
    void shutdown();//6
    //789
    void setConnectionCallback(const function<void (const shared_ptr<Tcpconnection>)> &);
    void setMessageCallback(const function<void(const shared_ptr<Tcpconnection>)> &);
    void setCloseCallback(const function<void(const shared_ptr<Tcpconnection>)> &);
    //10 11 12
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr(int);//13
    InetAddress getPeerAddr(int);//14
private:
    Socket _socket;
    SocketIO _socketIo;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite;

    function<void(const shared_ptr<Tcpconnection>)> _onConnection;//
    function<void(const shared_ptr<Tcpconnection>)> _onMessage;//
    function<void(const shared_ptr<Tcpconnection>)> _onClose;//
};//

Tcpconnection::Tcpconnection(int fd)
: _socket(fd)
, _socketIo(fd)
, _localAddr(getLocalAddr(fd))
, _peerAddr(getPeerAddr(fd))
, _isShutdownWrite(false)
{  }//1

/* Tcpconnection::~Tcpconnection() */
/* {}//2 */

string Tcpconnection::receive()
{
    char buf[65536] = {0};
    _socketIo.readline(buf, sizeof(buf));
    return string(buf);
}//3

void Tcpconnection::send(const string &msg)
{
    _socketIo.writen(msg.c_str(), msg.size());
}//4
string Tcpconnection::toString() const
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() << "-->"
        << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}//5
void Tcpconnection::shutdown()
{
    if(!_isShutdownWrite)
    {
        _isShutdownWrite = true;
        _socket.shutdownWrite();
    }
}//6

void Tcpconnection::setConnectionCallback(const function<void (const shared_ptr<Tcpconnection>)> &cb)
{
    _onConnection = std::move(cb);
}//7
void Tcpconnection::setMessageCallback(const function<void(const shared_ptr<Tcpconnection>)> &cb)
{
    _onMessage = std::move(cb);
}//8
void Tcpconnection::setCloseCallback(const function<void(const shared_ptr<Tcpconnection>)> &cb)
{
    _onClose = std::move(cb);
}//9

void Tcpconnection::handleConnectionCallback()
{
    if(_onConnection)
    {
        _onConnection(shared_from_this());
    }
}//10
void Tcpconnection::handleMessageCallback()
{
    if(_onMessage)
    {
        _onMessage(shared_from_this());
    }
}//11
void Tcpconnection::handleCloseCallback()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}//12

InetAddress Tcpconnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret;
    ret = getsockname(fd, (struct sockaddr*)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}//13

InetAddress Tcpconnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret;
    ret = getpeername(fd, (struct sockaddr*)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }
    return InetAddress(addr);
}//14
//-----------------------------------------------------------------------------------
//EventLoop
class EventLoop
{
public:
    EventLoop(Acceptor &);//1
    void loop();//2
    void unloop();//3
    void setConnectionCallback(function<void(const shared_ptr<Tcpconnection>)> &&cb)
    {
        _onConnection = std::move(cb);
    }
    void setMessageCallback(function<void(const shared_ptr<Tcpconnection>)> &&cb)
    {
        _onMessage = std::move(cb);
    }
    void setCloseCallback(function<void(const shared_ptr<Tcpconnection>)> &&cb)
    {
        _onClose = std::move(cb);
    }
private:
    void waitEpollFd();//4
    void handleNewConnection();//5
    void handleMessage(int);//6
    int createEpollFd();//7

    void addEpollFdRead(int);//8
    void delEpollFdRead(int);//9
    bool isConnectionClosed(int);//10
private:
    int _efd;
    Acceptor &_acceptor;
    vector<struct epoll_event> _eventList;
    bool _isLooping;
    map<int, shared_ptr<Tcpconnection>> _conns;

    function<void(const shared_ptr<Tcpconnection>)> _onConnection;
    function<void(const shared_ptr<Tcpconnection>)> _onMessage;
    function<void(const shared_ptr<Tcpconnection>)> _onClose;
};//

EventLoop::EventLoop(Acceptor &acceptor)
: _efd(createEpollFd())
, _acceptor(acceptor)
, _eventList(1024)
, _isLooping(false)
{
    addEpollFdRead(_acceptor.fd());
}//1

void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}//2
void EventLoop::unloop()
{
    _isLooping = false;
}//3
void EventLoop::waitEpollFd()
{
    int readyNum;
    do{
        readyNum = epoll_wait(_efd, &*_eventList.begin(), _eventList.size(), 3000);
    }while(readyNum == -1 && errno == EINTR);
    if(-1 == readyNum)
    {
        perror("epoll_wait");//
        return;
    }else if(0 == readyNum){
        cout << ">> epoll_wait timeout! " << endl;// 超时
    }else{
        /* if(readyNum == _eventList.size()) */
        /* { */
        /*     _eventList.resize(2*readyNum); */
        /* } */
        for(int i = 0;i < readyNum; ++i)
        {
            int fd = _eventList[i].data.fd;
            if(fd == _acceptor.fd())//处理新连接
            {
                if(_eventList[i].events & EPOLLIN)
                {
                    handleNewConnection();
                }
            }else{
                if(_eventList[i].events & EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }
    }
}//4
void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollFdRead(peerfd);
    shared_ptr<Tcpconnection> conn(new Tcpconnection(peerfd));
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);

    _conns.insert(std::make_pair(peerfd, conn));
    conn->handleConnectionCallback();
}//5

void EventLoop::handleMessage(int fd)
{
    bool isClosed = isConnectionClosed(fd);
    auto iter = _conns.find(fd);
    if(iter == _conns.end())
    {
        cout << "can't find fd " << endl;
        return;
    }
    if(!isClosed)
    {
        iter->second->handleMessageCallback();
    }else{
        delEpollFdRead(fd);
        iter->second->handleCloseCallback();
        _conns.erase(iter);
    }
}//6

int EventLoop::createEpollFd()
{
    int ret;
    ret = ::epoll_create1(0);//create与create1
    if(-1 == ret)
    {
        perror("epoll_create1");
    }
    return ret;
}//7

void EventLoop::addEpollFdRead(int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret;
    ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &event);
    if(-1 == ret)
    {
        perror("epoll_ctl");
    }
}//8

void EventLoop::delEpollFdRead(int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret;
    ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &event);
    if(-1 == ret)
    {
        perror("epoll_ctl");
    }
}//9
bool EventLoop::isConnectionClosed(int fd)
{
    int ret;
    do{
        char buf[1024];
        ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
    }while(ret == -1 && errno == EINTR);

    return (ret == 0);
}//10





