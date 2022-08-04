#pragma once
#include <iostream>
#include <stdio.h>
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
#include <mutex>
namespace forwandr
{
    int readn(int m_fd, char *buf, int size)
    {
        int nread = 0;
        int count = size;
        char *p = buf;

        while (count > 0)
        {
            if ((nread = read(m_fd, p, count)) > 0)
            {
                p += nread;
                count -= nread;
            }
            else if (nread == 0)
            {
                break;
            }
            else
            {
                if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
                {
                    // printf("%s\n", errno == EAGAIN ? "EAGAIN" : "EWOULDBLOCK");
                    continue;
                }
                std::cerr << errno << std::endl;
            }
        }
        return size;
    }

    int writen(int m_fd, const char *msg, int size)
    {
        int nwrite = 0;
        int count = size;
        const char *p = msg;
        while (count > 0)
        {
            if ((nwrite = write(m_fd, p, count)) > 0)
            {
                p += nwrite;
                count -= nwrite;
            }
            else if (nwrite == 0)
            {
                break;
            }
            else
            {
                if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
                {
                    // printf("%s\n", errno == EAGAIN ? "EAGAIN" : "EWOULDBLOCK");
                    continue;
                }
                std::cerr << errno << std::endl;
            }
        }
        return size;
    }
    int sendMsg(int m_fd, std::string msg)
    {
        std::mutex m;
        std::lock_guard<std::mutex> lock(m);
        char *data = new char[msg.size() + 4];
        int biglen = htonl(msg.size());
        memcpy(data, &biglen, 4);
        memcpy(data + 4, msg.data(), msg.size());

        int ret = writen(m_fd, data, msg.size() + 4);
        delete[] data;
        return ret;
    }
    std::string recvMsg(int m_fd)
    {
        // 先读包头
        int len = 0;
        std::mutex m;
        std::lock_guard<std::mutex> lock(m);
        readn(m_fd, (char *)&len, 4);
        len = ntohl(len);

        char *buf = new char[len + 4];
        int ret = readn(m_fd, buf, len);
        if (ret != len)
        {
            return std::string();
        }
        buf[len] = '\0';
        std::string retstr(buf);
        delete[] buf;
        return retstr;
    }

} // namespace forwandr
