#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include<sys/signal.h>
#define SERV_PORT 9527
class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    ~TcpSocket();
    int connectToHost(std::string ip, unsigned short port);
    int sendMsg(std::string msg);
    std::string recvMsg();
    void perr_exit(const char *s);

private:
    int readn(char *buf, int size);
    int writen(const char *msg, int size);

private:
    int m_fd; // 通信的套接字
};
