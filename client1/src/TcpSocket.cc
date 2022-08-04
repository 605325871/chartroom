#include "../include/TcpSocket.hh"
TcpSocket::TcpSocket()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::TcpSocket(int socket)
{
    m_fd = socket;
}
TcpSocket::~TcpSocket()
{
    if (m_fd > 0)
        close(m_fd);
}
void TcpSocket::perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}
int TcpSocket::connectToHost(std::string ip, unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    inet_pton(AF_INET, ip.data(), &saddr.sin_addr.s_addr);
    int ret = connect(m_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
        perr_exit("connect error");
    return ret;
}

int TcpSocket::readn(char *buf, int size)
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
        else if(nread ==0)
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

int TcpSocket::writen(const char *msg, int size)
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
        else if(nwrite ==0)
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
int TcpSocket::sendMsg(std::string msg)
{
    char *data = new char[msg.size() + 4];
    int biglen = htonl(msg.size());
    memcpy(data, &biglen, 4);
    memcpy(data + 4, msg.data(), msg.size());

    int ret = writen(data, msg.size() + 4);
    delete[] data;
    return ret;
}

inline void printStr(const std::string &s) {
    auto size = s.size();
    printf("length=%zu\n", size);
    uint32_t offset = 0;
    while (offset + 16 <= size) {
        const char *data = s.data() + offset;
        fprintf(stderr,
                "%08x %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
                "%02hhx %02hhx %02hhx %02hhx "
                "%02hhx %02hhx %02hhx \t\t",
                offset, data[0], data[1], data[2], data[3], data[4], data[5], data[6],
                data[7], data[8], data[9], data[10], data[11], data[12], data[13],
                data[14], data[15]);
        for (int i = 0; i < 16; i++) {
            putc(data[i], stderr);
        }
        putc('\n', stderr);
        offset += 16;
    }
    if (offset < size) {
        const auto *data = s.data() + offset;
        fprintf(stderr, "%08x ", offset);
        int i;
        for (i = 0; i < size - offset; i++) {
            fprintf(stderr, "%02hhx ", data[i]);
        }
        for (; i < 16; i++) {
            fprintf(stderr, "   ");
        }
        fprintf(stderr, "\t\t");
        for (int i = 0; i < size - offset; i++) {
            putc(data[i], stderr);
        }
        putc('\n', stderr);
    }
}
std::string TcpSocket::recvMsg()
{
    // 先读包头
    int len = 0;
    readn((char *)&len, 4);
    len = ntohl(len);
    std::string length = std::to_string(len);
    // printStr(length);

    char* buf = new char[len + 4];
    int ret = readn(buf, len);
    if (ret != len)
    {
        return std::string();
    }
    buf[len] = '\0';
    std::string retstr(buf);
    // printStr(retstr);
    delete[] buf;
    return retstr;
}


