
#include "../include/Tcpepoll1.2.hh"
#include "../include/TcpSocket.hh"
#include "../include/myredis.hpp"
#include "../include/threadpool.hpp"
#include "../include/long.hpp"

void promgsignal(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);
}

int main()
{
    srand((unsigned int)time(NULL));
    Tcpepoll tep;

    promgsignal();
    tep.InitSocket();
    ThreadPool pool(20);

    while (true)
    {
        int nreads = epoll_wait(tep.epfd, tep.evs, tep.evssize, -1);
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        for (int i = 0; i < nreads; ++i)
        {
            int fd = tep.evs[i].data.fd;
            if (fd == tep.m_fd)
            {
                int cfd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
                char ipbuf[64];
                cout << endl;
                std::cout << " " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)) << " " << ntohs(client_addr.sin_port) << std::endl;

                onile_id.insert(cfd); //将在线的插入进去
                fcntl(cfd, F_SETFL, O_NONBLOCK);

                struct epoll_event newev;
                newev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                newev.data.fd = cfd;
                epoll_ctl(tep.epfd, EPOLL_CTL_ADD, cfd, &newev);
            }
            else
            {

                std::string msg = forwandr::recvMsg(fd);
                // cout << " msg " << msg << endl;
                int whatodo = atoi(msg.c_str());
                // cout << "whatodo" << whatodo << endl;
                int len = msg.length();
                // cout << " len " << len << endl;
                if (len == 0)
                {

                    for (auto iter = map_id.begin(); iter != map_id.end();)
                    {
                        if (iter->second == fd)
                        {
                            cout << " uid " << iter->first << endl;
                            iter = map_id.erase(iter);
                        }
                        else
                            ++iter;
                    }
                    onile_id.erase(fd);
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);

                    close(fd);
                }

                if (whatodo == WO_YAO_ZHU_CE)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = clientTologin;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == WO_YAO_DENG_LU)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = clientTologed;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }

                if (whatodo == Add_Frind)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = addfriend;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == CHECK_NEW_APPLICTION)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = lookapplictionwith;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == SHOW_FRIENDS)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = SHOWFRIENDS;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == DEL_FRIENDS)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = DELFRIENDS;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }

                if (whatodo == APPLICT_CHAT)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = applictchat;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == FLLLLLLLLLLL)
                {

                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = fflushmessage;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == BLOCK_FRIENDS)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = blockfriends;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == CREATE_GROUP)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = CREATEGROUP;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == TRASFILES)
                {

                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = ThransFILE;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == CHECKHISTORYCHAT)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = recvcharhistory;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == Recvive_File)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = recviefile;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == LOOK_GROUP)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = Look_Group;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == ADD_GROUP)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = addgroup;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == SET_GM)
                {

                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = setGm;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if (whatodo == CHAT_GROUP)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = chatgroup;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
                if(whatodo==UNblock_frine)
                {
                     int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = UNblock;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }

                if (whatodo == EXIT_GROUP)
                {
                    int arg[] = {tep.evs[i].data.fd, tep.epfd};
                    epoll_ctl(tep.epfd, EPOLL_CTL_DEL, fd, NULL);
                    Task task;
                    task.function = exitgroup;
                    task.arg = arg;
                    msg.clear();
                    pool.addTask(task);
                }
            }
        }
    }
}