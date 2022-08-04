#pragma oncee
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <sys/epoll.h>
#include "TcpSocket.hh"
#include <fcntl.h>
#include <sys/signal.h>
#include <memory>


class Tcpepoll
{
public:
    int epfd;
    int m_fd;
    struct epoll_event ev;
    struct epoll_event evs[1024];
    int evssize;
    int serv_port{9527};
    std::string newmessg;
public:
    Tcpepoll(/* args */);
    ~Tcpepoll();
    int InitSocket(unsigned short port = 9527);
    int sendMsg(std::string msg, int _fd);
    std::string recvMsg(int _fd);

private:
    int readn(char *buf, int size, int _fd);
    int writen(const char *msg, int size, int _fd);
    inline void perr_exit(const char *s);
};
