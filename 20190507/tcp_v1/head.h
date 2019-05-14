
#include<unistd.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>

#include<iostream>
#include<string>
#include<sstream>

using std::cout;
using std::endl;
using std::string;

//----------------------------------------------------------------------
//ip地址与端口
class InetAddress
{
public:
    explicit
    InetAddress(unsigned short);//1
    InetAddress(const string &, unsigned short);//2
    InetAddress(const struct sockaddr_in &);//3

    string ip() const;//4
    unsigned short port() const;//5
    struct sockaddr_in *get_addrPtr()
    {
        return &_addr;
    }
private:
    struct sockaddr_in _addr;
};//

InetAddress::InetAddress(unsigned short port)
{
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;//代表本机地址
}//1

InetAddress::InetAddress(const string &ip, unsigned short port)
{
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}//2

InetAddress::InetAddress(const struct sockaddr_in &addr)
: _addr(addr)
{  }//3

string InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));
}//4
    
unsigned short InetAddress::port() const
{   
    return ntohs(_addr.sin_port);
}//5
//----------------------------------------------------------------------
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
    int getfd()
    {
        return _fd;
    }
    void shutdownWrite()
    {
        shutdown(_fd, SHUT_WR);
    }

    ~Socket()
    {
        close(_fd);
    }
private:
    int _fd;
};//
//-----------------------------------------------------------------------------
//Acceptor类---bind与listen
class Acceptor
{
public:
    Acceptor(unsigned short);//1
    Acceptor(const string &, unsigned short);//2
    void ready();//3
    int accept();//4
private:
    void setReuseAddr(bool);//5
    void setReusePort(bool);//6
    void bind();//7
    void listen();//8
private:
    InetAddress _addr;
    Socket _listenSock;
};//

Acceptor::Acceptor(unsigned short port)
: _addr(port), _listenSock()
{  }//1

Acceptor::Acceptor(const string &ip, unsigned short port)
: _addr(ip, port), _listenSock()
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
    int peerfd = ::accept(_listenSock.getfd(), NULL, NULL);
    if(-1 == peerfd)
    {
        perror("accept");
    }
    return peerfd;
}//4
void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    if(setsockopt(_listenSock.getfd(), 
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  &one,
                  sizeof(one)) < 0)
    {
        perror("setReuseAddr");
    }
}//5
void Acceptor::setReusePort(bool on)
{
    int one = on;
    if(setsockopt(_listenSock.getfd(),
                  SOL_SOCKET,
                  SO_REUSEPORT,
                  &one,
                  sizeof(one)) < 0)
    {
        perror("setReusePort");
    }
}//6
void Acceptor::bind()
{
    int ret = ::bind(_listenSock.getfd(), 
                     (struct sockaddr*)_addr.get_addrPtr(),
                     sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind");
    }
}//7
void Acceptor::listen()
{
    int ret = ::listen(_listenSock.getfd(), 10);
    if(-1 == ret)
    {
        perror("listen");
    }
}//8
//----------------------------------------------------------------------
//SocketIO,读写
class SocketIO
{
public:
    explicit
    SocketIO(int);//1

    int readn(char*, int);//2
    int readline(char*, int);//3
    int writen(const char*, int);//4
private:
    int recvPeek(char*, int);//5 //查看缓冲区数据，给readline用
private:
    int _fd;
};//
SocketIO::SocketIO(int fd)
: _fd(fd)
{  }//1

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
                perror("readn_read");
                break;
            }
        }else if(0 == ret){
            break;
        }else{
            total += ret;
            p += ret;
        }
    }
    return total;
}//2
int SocketIO::readline(char *buf, int maxLen)
{
    int left = maxLen - 1;
    char *p = buf;
    int ret; // 记录'\n'位置
    int total = 0;
    while(left > 0)
    {
        ret = recvPeek(p, left);
        for(int i = 0;i != ret; ++i)// 查找'\n'
        {
            if(p[i] == '\n')
            {
                int sz = i+1;
                readn(p, sz);
                p[sz] = '\0';
                total += sz;
                return total;
            }
        }
        readn(p, ret); // 没找到'\n', 读出数据,循环继续找
        left -= ret;
        p += ret;
        total += ret;
    }
    *p = '\0';
    return total;
}//3

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
                perror("readn_read");
                break;
            }
        }else if(0 == ret){
            break;
        }else{
            total += ret;
            p += ret;
        }
    }
    return total;
}//4

int SocketIO::recvPeek(char *buf, int len)
{
    int ret;
    do{
        ret = ::recv(_fd, buf, len, MSG_PEEK); // 单查看, 不取缓冲区数据
    } while(-1 == ret && errno == EINTR);
    return ret;
}//5
//----------------------------------------------------------------------
//TcpConnection 连接上来的客户
class TcpConnection
{
public:
    TcpConnection(int);//1
    //~TcpConnection();//2
    
    string receive();//3 读取缓冲区到\n为止
    void send(const string &);//4

    string toString() const;//5  //返回服务器ip端口-->客户ip端口的字符串
    void shutdown();//6

private:
    InetAddress getLocalAddr(int);//7
    InetAddress getPeerAddr(int);//8
private:
    Socket _socket;
    SocketIO _socketIo;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite;
};//

TcpConnection::TcpConnection(int fd)
: _socket(fd)
, _socketIo(fd)
, _localAddr(getLocalAddr(fd))
, _peerAddr(getPeerAddr(fd))
, _isShutdownWrite(false)
{  }//1

/* TcpConnection::~TcpConnection() */
/* { */

/* }//2 */

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
    if(!_isShutdownWrite)
    {
        _isShutdownWrite = true;
        _socket.shutdownWrite();
    }
}//6

InetAddress TcpConnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(-1 == getsockname(fd,
                         (struct sockaddr*)&addr,
                         &len))
    {
        perror("getLocalAddr");
    }
    return InetAddress(addr);
}//7

InetAddress TcpConnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(-1 == getpeername(fd,
                         (struct sockaddr*)&addr,
                         &len))
    {
        perror("getPeerAddr");
    }
    return InetAddress(addr);
}//8
//---------------------------------------------------------
//
