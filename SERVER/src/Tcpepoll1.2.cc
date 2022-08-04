#include "../include/Tcpepoll1.2.hh"
#include<netinet/tcp.h>
Tcpepoll::Tcpepoll()
{
    signal(SIGPIPE, SIG_IGN);
    epfd = 0;
    m_fd = 0;
    evssize = sizeof(evs) / sizeof(evs[0]);
}
Tcpepoll::~Tcpepoll()
{
    if (m_fd > 0)
        close(m_fd);
    close(epfd);
}
inline void Tcpepoll::perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}

int Tcpepoll::InitSocket(unsigned short port)
{

    signal(SIGPIPE, SIG_IGN);
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_port = port;
    std::string ip = "127.0.0.1";
    serv_addr.sin_family = AF_INET;                      // 地址族协议  ipv4
    serv_addr.sin_port = htons(serv_port);               // 本地端口， 需要转换为大端
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr); //  是用本机的任意IP

    int opt = 1;
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //端口复用

    int keepalive = 1;   // 开启TCP KeepAlive功能
    int keepidle = 27; // tcp_keepalive_time
    int keepcnt = 3;     // tcp_keepalive_probes
    int keepintvl = 3;  // tcp_keepalive_intvl

    setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
    setsockopt(m_fd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepidle, sizeof(keepidle));
    setsockopt(m_fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcnt, sizeof(keepcnt));
    setsockopt(m_fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepintvl, sizeof(keepintvl));

    int ret = bind(m_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        perr_exit("bind error");

    // 设置监听
    ret = listen(m_fd, 128);
    if (ret == -1)
        perr_exit("listen error");

    epfd = epoll_create(100);
    if (epfd == -1)
        perr_exit("epoll_creat error");

    ev.events = EPOLLIN | EPOLLET | EPOLLHUP;
    ev.data.fd = m_fd;
    evssize = sizeof(evs) / sizeof(evs[0]);
    if ((ret = epoll_ctl(epfd, EPOLL_CTL_ADD, m_fd, &ev)) == -1)
        perr_exit("epoll_ctl error");
    return epfd;
}
