

#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/epoll.h>

#include<iostream>
#include<string>
#include<memory>
#include<functional>
#include<sstream>
#include<map>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::function;
using std::shared_ptr;
using std::map;
using std::vector;

class Noncopyable
{
protected:
    Noncopyable(){}
    ~Noncopyable(){}
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};//
//-----------------------------------------------------------------------
//InetAddress 地址与端口
class Inetaddress
{
public:
    explicit
    Inetaddress(unsigned short);//1
    Inetaddress(unsigned short, const string &);//2
    Inetaddress(const struct sockaddr_in &);//3

    string ip() const;//4
    unsigned short port() const;//5
    struct sockaddr_in *getInetAddressPtr()
    {
        return &_addr;
    }
private:
    struct sockaddr_in _addr;
};//

Inetaddress::Inetaddress(unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;// 代表本机地址
}//1

Inetaddress::Inetaddress(unsigned short port, const string &ip)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}//2
Inetaddress::Inetaddress(const struct sockaddr_in &addr)
: _addr(addr)
{  }//3

string Inetaddress::ip() const
{
    return string(::inet_ntoa(_addr.sin_addr));
}//4

unsigned short Inetaddress::port() const
{
    return ntohs(_addr.sin_port);
}//5
//------------------------------------------------------------------------------
//套接字Socket
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
    ~Socket()
    {
        ::close(_fd);
    }
    int fd() const
    {
        return _fd;
    }
    void shutdownWrite()
    {
        ::shutdown(_fd, SHUT_WR);
    }
private:
    int _fd;
};//
//----------------------------------------------------------------------------
//SocketIO 
class SocketIO
{
public:
    explicit
    SocketIO(int fd)
    : _fd(fd)
    {  }
    
    int readn(char *,int);//1
    int writen(const char*, int);//2
    int readline(char *, int);//3
private:
    int recvPeek(char*, int);//4
private:
    int _fd;
};//
int SocketIO::readn(char *buf, int len)
{
    int total = 0;
    int ret;
    char *p = buf;
    while(total < len)
    {
        ret = ::read(_fd, p + total, len - total);
        if(-1 == ret)
        {
            if(errno == EINTR)
            {
                continue;
            }else{
                perror("read");
                break;
            }
        }else if( 0 == ret ){
            break;   
        }else{
            total += ret;
        }
    }
    return total;
}//1

int SocketIO::writen(const char *buf, int len)
{
    int total = 0;
    int ret;
    const char *p = buf;
    while(total < len)
    {
        ret = ::write(_fd, p + total, len - total);
        if(-1 == ret)
        {
            if(errno == EINTR)
            {
                continue;
            }else{
                perror("read");
                break;
            }
        }else if( 0 == ret ){
            break;   
        }else{
            total += ret;
        }
    }
    return total;
}//2

int SocketIO::readline(char *buf, int maxLen)
{
    int left = maxLen - 1;
    char *p = buf;
    int ret;
    int total = 0;
    while(left > 0)
    {
        ret = recvPeek(p, left);
        for(int i = 0; i < ret; ++i)
        {
            if(p[i] == '\n')
            {
                readn(p, i + 1);
                total += i+1;
                p += i+1;
                *p = '\0';
                return total;
            }
        }
        //没有发现'\n'
        readn(p, ret);
        left -= ret;
        total += ret;
        p += ret;
    }
    *p = '\0';
    return total;
}//3

int SocketIO::recvPeek(char *buf, int len)
{
    int ret;
    do{
        ret = ::recv(_fd, buf, len, MSG_PEEK);
    }while(ret == -1 && errno == EINTR);
    return ret;
}//4
//---------------------------------------------------------------------------------
//Acceptor
class Acceptor
{
public:
    Acceptor(unsigned short);//1
    Acceptor(const string &, unsigned short);//2
    void ready();//3
    int accept();//4
    int fd () const
    {
        return _listenSock.fd();
    }
private:
    void setReuseAddr(bool);//5
    void setReusePort(bool);//6
    void bind();//7
    void listen();//8
private:
    Inetaddress _addr;
    Socket _listenSock;
};//

Acceptor::Acceptor(unsigned short port)
: _addr(port)
, _listenSock()
{  }//1

Acceptor::Acceptor(const string &ip, unsigned short port)
: _addr(port, ip)
, _listenSock()
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
    int peerfd = ::accept(_listenSock.fd(), NULL, NULL);
    if(-1 == peerfd)
    {
        perror("accept");
    }
    return peerfd;
}//4

void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    int ret;
    ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(ret < 0)
    {
        perror("setReuseAddr::setsockopt");
    }
}//5

void Acceptor::setReusePort(bool on)
{
    int one = on;
    int ret;
    ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));
    if(ret < 0)
    {
        perror("setReusePort::setsockopt");
    }
}//6

void Acceptor::bind()
{
    int ret;
    ret = ::bind(_listenSock.fd(), (struct sockaddr*)_addr.getInetAddressPtr(), sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind");
    }
}//7
void Acceptor::listen()
{
    int ret;
    ret = ::listen(_listenSock.fd(),10);
    if(-1 == ret)
    {
        perror("listen");
    }
}//8
//-------------------------------------------------------------------------------------
//TcpConnection 
class TcpConnection
: public Noncopyable
, public std::enable_shared_from_this<TcpConnection>    
{
public:
    TcpConnection(int fd);//1
    /* ~TcpConnection();//2 */

    string receive();//3
    void send(const string &msg);//4
    string toString() const;//5

    void shutdown();//6

    void setConnectionCallback(const function<void(shared_ptr<TcpConnection>)> &cb)
    {
        _cbConnection = std::move(cb);
    }
    void setMessageCallback(const function<void(shared_ptr<TcpConnection>)> &cb)
    {
        _cbMessage = std::move(cb);
    }
    void setCloseCallback(const function<void(shared_ptr<TcpConnection>)> &cb)
    {
        _cbClose = std::move(cb);
    }
    void handleConnectionCallback()
    {
        if(_cbConnection)
        {
            _cbConnection(shared_from_this());
        }
    }
    void handleMessageCallback()
    {
        if(_cbMessage)
        {
            _cbMessage(shared_from_this());
        }
    }
    void handleCloseCallback()
    {
        if(_cbClose)
        {
            _cbClose(shared_from_this());
        }
    }
private:
    Inetaddress getLocalAddr(int);//7
    Inetaddress getPeerAddr(int);//8
private:
    Socket _socket;
    SocketIO _socketIo;
    Inetaddress _localAddr;
    Inetaddress _peerAddr;
    bool _isShutdownWrite;

    function<void(const shared_ptr<TcpConnection>)> _cbConnection;
    function<void(const shared_ptr<TcpConnection>)> _cbMessage;
    function<void(const shared_ptr<TcpConnection>)> _cbClose;
};//


TcpConnection::TcpConnection(int fd)
: _socket(fd)
, _socketIo(fd)
, _localAddr(getLocalAddr(fd))
, _peerAddr(getPeerAddr(fd))
, _isShutdownWrite(false)
{  }//1

string TcpConnection::receive()
{
    char buf[65536] = {0};
    _socketIo.readline(buf, sizeof(buf));
    return string(buf);
}//3

void TcpConnection::send(const string &msg)
{
    _socketIo.writen(msg.c_str(), msg.size());
}//4

string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() << "-->"
        << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}//5

void TcpConnection::shutdown()
{
    _isShutdownWrite = true;
    _socket.shutdownWrite();
}//6

Inetaddress TcpConnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret;
    ret = getsockname(fd, (struct sockaddr*)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }
    return Inetaddress(addr);
}//7
Inetaddress TcpConnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret;
    ret = getpeername(fd, (struct sockaddr*)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }
    return Inetaddress(addr);
}//8
//----------------------------------------------------------------------------------
//EventLoop-------epoll
class EventLoop
{
public:
    EventLoop(Acceptor &);//1
    void loop();//2
    void unloop();//3

    void setConnectionCallback(function<void(const shared_ptr<TcpConnection>)> &&cb)
    {
        _cbConnection = std::move(cb);
    }
    void setMessageCallback(function<void(const shared_ptr<TcpConnection>)> &&cb)
    {
        _cbMessage = std::move(cb);
    }
    void setCloseCallback(function<void(const shared_ptr<TcpConnection>)> &&cb)
    {
        _cbClose = std::move(cb);
    }
private:
    int createEpollFd();//4
    void waitEpollFd();//5
    void handleNewConnection();//6
    void handleMessage(int);//7

    void addEpollFdRead(int);//8
    void delEpollFdRead(int);//9
    bool isConnectionClosed(int);//10
private:
    int _efd;
    Acceptor &_acceptor;
    vector<struct epoll_event> _events;
    map<int, shared_ptr<TcpConnection>> _conns;
    bool _isLooping;

    function<void(const shared_ptr<TcpConnection>)> _cbConnection;
    function<void(const shared_ptr<TcpConnection>)> _cbMessage;
    function<void(const shared_ptr<TcpConnection>)> _cbClose;
};//

EventLoop::EventLoop(Acceptor &acceptor)
: _efd(createEpollFd())
, _acceptor(acceptor)
, _events(1024)
, _conns()
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

int EventLoop::createEpollFd()
{
    int efd = ::epoll_create1(0);
    if(-1 == efd)
    {
        perror("epoll_create1");
    }
    return efd;
}//4

void EventLoop::waitEpollFd()
{
    int readyNum;
    do{
        readyNum = epoll_wait(_efd, &*_events.begin(), _events.size(), -1);
    }while(readyNum == -1 && errno == EINTR);
    if(-1 == readyNum)
    {
        perror("epoll_wait");
        return;
    }else
    {
        for(int i = 0; i < readyNum; ++i)
        {
            if(_acceptor.fd() == _events[i].data.fd)//新来的连接
            {
                if(_events[i].events & EPOLLIN)
                {
                    handleNewConnection();
                }
            }else{ //来的消息
                if(_events[i].events & EPOLLIN)
                {
                    handleMessage(_events[i].data.fd);
                }
            }
        }
    }

}//5

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollFdRead(peerfd);
    shared_ptr<TcpConnection> conn(new TcpConnection(peerfd));
    conn->setConnectionCallback(_cbConnection);
    conn->setMessageCallback(_cbMessage);
    conn->setCloseCallback(_cbClose);

    _conns.insert(std::make_pair(peerfd, conn));

    conn->handleConnectionCallback();
}//6
void EventLoop::handleMessage(int fd)
{
    bool isClosed = isConnectionClosed(fd);
    auto iter = _conns.find(fd);
    if(iter != _conns.end())
    {
        if(!isClosed)
        {
            iter->second->handleMessageCallback();
        }else{
            delEpollFdRead(fd);
            iter->second->handleCloseCallback();
            _conns.erase(iter);
        }
    }
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
       perror("addEpollFdRead::epoll_ctl");
   }
}//8
void EventLoop::delEpollFdRead(int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    int ret;
    ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &event);
    if(-1 == ret)
    {
        perror("delEpollFdRead::epoll_ctl");
    }
}//9
bool EventLoop::isConnectionClosed(int fd)
{
    int ret;
    do{
        char buf[1024];
        ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
    }while(-1 == ret && EINTR == errno);

    return (ret == 0);
}//10
//------------------------------------------------------------------------------------
//TcpServer(整合Acceptor和Eventloop)
class TcpServer
{
public:
    TcpServer(const string &, unsigned short);//1
    void start();//2

    void setConnectionCallback(function<void(const shared_ptr<TcpConnection>)> &&cb)
    {
        _loop.setConnectionCallback(std::move(cb));
    }
    void setMessageCallback(function<void(const shared_ptr<TcpConnection>)> &&cb)
    {
        _loop.setMessageCallback(std::move(cb));
    }
    void setCloseCallback(function<void(const shared_ptr<TcpConnection>)> &&cb)
    {
        _loop.setCloseCallback(std::move(cb));
    }
private:
    Acceptor _acceptor;
    EventLoop _loop;
};//
TcpServer::TcpServer(const string &ip, unsigned short port)
: _acceptor(ip, port)
, _loop(_acceptor)
{  }//1

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}//2
//------------------------------------------------------------------------------------








