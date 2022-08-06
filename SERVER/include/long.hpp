#pragma once
#include "user.hpp"
#include "myredis.hpp"
#include "Forwandr.hpp"
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <mutex>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <chrono>
#include <sys/sendfile.h>
#define WO_YAO_ZHU_CE 87880
#define WO_YAO_DENG_LU 87881
#define BU_cun_ZAI 87882
#define Mi_MA_CUO_Wu 87883
#define Add_Frind 87884

#define SAME_UID 87886
#define Agree_to_add 87887
#define Reject_to_add 87888
#define NO_this_user 87889
#define WAIT_FOR_MINUTE 87900
#define HAVE_APPLICTION 87901
#define CHECK_NEW_APPLICTION 87902
#define CHAT_WITH_FRIENDS 87903
#define SHOW_FRIENDS 87904
#define DEL_FRIENDS 87905
#define Del_success 87906
#define APPLICT_CHAT 87907
#define ALIVE_LOGined 87908
#define FLUSH_message 87909
#define READY_TO_LEARN 87910
#define EXIT_PR_CHART 87911
#define BLOCK_FRIENDS 87912
#define BLOCK_SUCCESS 87913
#define TRASFILES 87914
#define NEW_MESSAGE 87915
#define CREATE_GROUP 87916
#define Repeatlog 87917
#define NONALIVE 87918
#define SAME_CHAT 87919
#define Recvive_File 87920
#define CHECKHISTORYCHAT 87921
#define LOOK_GROUP 87922
#define ADD_GROUP 87923
#define SET_GM 87924

#define CHEKGROUPPEOPLE 87925
#define CHAT_GROUP 97926
#define EXIT_GROUP 97928

#define UNblock_frine 97929
#define FLLLLLLLLLLL 97930

std ::set<int> onile_id;
std::map<std::string, int> map_id{};
int alarmfd = -1;
std::mutex m;

bool IsAllNumString(const std::string &str)
{
    for (const auto &s_char : str)
    {
        if (s_char < '0' || s_char > '9')
        {
            return false;
        }
    }
    return true;
}
void clientTologin(void *arg)
{

    forJson user;

    struct epoll_event temp;
    int *arr = (int *)arg;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    // epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp);

    string buf = forwandr::recvMsg(fd);

    string storebuf;
    int len = buf.size();

    user.parseJson(buf);

    Redis redis;
    redis.connect();
    if (1 == redis.hashexists("pinfo", user.uid))
    {
        forwandr::sendMsg(fd, "SAME_UID");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    redis.hsetValue("pinfo", user.uid, buf);
    redis.hsetValue("namespce", user.name, buf);

    redis.hsetValue("id-name-spce", user.uid, user.name);

    redis.saddvalue("idinfo", user.uid);
    forwandr::sendMsg(fd, "WO_YAO_ZHU_CE");
    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
}
void clientTologed(void *arg)
{
    forJson user;
    Redis redis;
    struct epoll_event temp;
    int *arr = (int *)arg;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    // epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp); //从树上去除该套接字

    string buf = forwandr::recvMsg(fd);

    cout << " buf  " << buf << endl;

    int len = buf.size();
    std::string loguid;
    std::string logpasswd;
    std::string logname;

    user.parseJson(buf);
    loguid = user.uid;
    logpasswd = user.passwd;
    logname = user.name;

    if (map_id.find(loguid) != map_id.end())
    {
        forwandr::sendMsg(fd, "Repeatlog");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    redis.connect();
    if (0 == redis.hashexists("pinfo", loguid)) // redis.hashexists("pinfo", user.uid))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {

        auto str = (redis.gethash("pinfo", loguid));
        user.parseJson(str);
        if (user.passwd == logpasswd)
        {
            //离线的时候记得删除
            map_id.insert(pair<string, int>(user.uid, fd));

            cout << "uid 登陆" << user.uid << endl;

            forwandr::sendMsg(fd, "WO_YAO_DENG_LU");

            redis.hsetValue("UserChatStatus", user.uid, "false");
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

            return;
        }
        else
        {

            forwandr::sendMsg(fd, "Mi_MA_CUO_Wu");
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

            return;
        }
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    cout << "登陆线程结束127" << endl;
    return;
}

void addfriend(void *arg)
{

    Redis redis;
    redis.connect();
    forJson user;
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    // Json::CharReaderBuilder readerBuilder;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)arg;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    // epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp); //从树上去除该套接字

    auto friendidwithsend = forwandr::recvMsg(fd);
    r.parse(friendidwithsend, root);
    int i = 0;
    auto sendid = root[i++].asString();
    auto addedid = root[i++].asString();

    if (0 == redis.sismember("idinfo", addedid))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    auto str = redis.gethash("pinfo", addedid);
    auto addedstr = addeduser.parseJson(str);

    redis.saddvalue(addedid + "friendappliction", sendid);
    forwandr::sendMsg(fd, "WAIT_FOR_MINUTE");

    auto checkfd = map_id.find(addedid);

    //存入被加人的未决事件列表
    redis.saddvalue(addedid + "pendinglist", sendid + "申请加您为好友");

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void lookapplictionwith(void *arg)
{
    Redis redis;
    redis.connect();
    forJson user;
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)arg;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    auto friendappliction = sendid + "friendappliction";

    cout << friendappliction << endl;

    auto len = redis.getsetnumber(friendappliction);
    int freelen = len;
    if (len == 0)
    {
        forwandr::sendMsg(fd, "no_user_appliction");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        forwandr::sendMsg(fd, "HAVE_APPLICTION");
    }

    forwandr::sendMsg(fd, to_string(len));
    auto getstr = redis.smembers(friendappliction);
    for (int i = 0; len > 0; ++i)
    {
        if (len < 0 || len == 0)
        {
            break;
        }

        string friendstr = getstr[i]->str;
        if (len > 0)
        {
            forwandr::sendMsg(fd, getstr[i]->str);
        }
        ms = forwandr::recvMsg(fd);

        if (ms == "yes")
        {
            redis.saddvalue(sendid + "friend", getstr[i]->str);
            redis.saddvalue(friendstr + "friend", sendid);
            redis.sremvalue(friendappliction, getstr[i]->str);
        }
        else
        {

            redis.sremvalue(friendappliction, getstr[i]->str);
            len = redis.getsetnumber(friendappliction);
            printf("287              len = %d \n", len);
            continue;
        }
        len = redis.getsetnumber(friendappliction);

        printf("292              len = %d \n", len);
    }

    // for (int i = 0; i < freelen; i++)
    //     free(getstr[i]);
    // free(getstr);

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

    return;
}

void SHOWFRIENDS(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    cout << " ms336 " << ms << endl;
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    auto friendspace = sendid + "friend";

    int number = redis.getsetnumber(friendspace);
    int freenumber = number;
    if (number == 0)
    {
        forwandr::sendMsg(fd, "NO_FRIEND");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

        return;
    }

    auto str = redis.smembers(friendspace);

    forwandr::sendMsg(fd, to_string(number));

    for (int i = 0; i < number; ++i)
    {

        auto strname = redis.gethash("pinfo", str[i]->str);

        user.parseJson(strname);

        if (map_id.find(user.uid) != map_id.end())
        {
            forwandr::sendMsg(fd, user.name);
        }
        else
        {
            forwandr::sendMsg(fd, "#" + user.name);
        }
    }

    for (int i = 0; i < freenumber; i++)
        free(str[i]);
    free(str);
    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

    return;
}

void DELFRIENDS(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    auto friendspace = sendid + "friend";

    auto sermsname = forwandr::recvMsg(fd);

    auto de = redis.gethash("namespce", sermsname);

    user.parseJson(de);
    cout << "user udi " << user.uid << endl;
    cout << "friendspcae" << friendspace << "    " << user.uid << endl;
    if ((redis.sismember(friendspace, user.uid) == 0))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {

        auto delname = redis.gethash("namespce", sermsname);

        user.parseJson(delname);

        cout << friendspace << endl;

        redis.sremvalue(friendspace, user.uid);
        redis.sremvalue(user.uid + "friend", sendid);

        forwandr::sendMsg(fd, "Del_success");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void applictchat(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    cout << "sendid" << sendid << endl;
    auto sermsname = forwandr::recvMsg(fd);
    cout << sermsname << endl;

    if (redis.hashexists("namespce", sermsname) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    auto friendname = redis.gethash("namespce", sermsname);

    user.parseJson(friendname);

    auto recvefid = user.uid;

    auto blocksp = recvefid + "blocklist";

    cout << " block " << blocksp << endl;

    if (redis.sismember(blocksp, sendid) == 1)
    {
        forwandr::sendMsg(fd, "block");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    //必须加好友才能私聊
    auto sendfd = map_id.find(sendid)->second;
    auto recvfd = map_id.find(recvefid)->second;

    if ((redis.sismember(sendid + "friend", user.uid) == 0) || (redis.sismember(sendid + "blocklist", user.uid) == 1))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        forwandr::sendMsg(fd, "READY_TO_LEARN");
        redis.hsetValue("UserChatStatus", sendid, "trueone");
    }

    if (redis.hashexists("UserChatStatus", recvefid) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    auto StatusFlag = redis.gethash("UserChatStatus", recvefid);

    cout << " Status Flag  recvfid  " << StatusFlag << endl;

    auto checkiter = map_id.find(recvefid);
    auto StatusFlagtwo = redis.gethash("UserChatStatus", recvefid);

    if (StatusFlag == "false" || checkiter == map_id.end() || StatusFlag == "truethree")

    {
        forwandr::sendMsg(sendfd, "NONALIVE");
        while (true)
        {
            auto sendfdms = forwandr::recvMsg(sendfd);

            if (sendfdms == "quit")
            {
                forwandr::sendMsg(sendfd, "quit");
                redis.hsetValue("UserChatStatus", sendid, "false");
                redis.saddvalue(recvefid + "pendinglist", sendid + "发来新消息 ");

                cout << "1111111" << endl;
                StatusFlag = redis.gethash("UserChatStatus", sendid);

                redis.saddvalue(sendid + recvefid + "charhistory", sendfdms);
                redis.saddvalue(recvefid + sendid + "charhistory", sendfdms);
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

                return;
            }
            redis.saddvalue(sendid + recvefid + "charhistory", sendfdms);
            redis.saddvalue(recvefid + sendid + "charhistory", sendfdms);

            if (checkiter != map_id.end())
            {
                redis.saddvalue(recvefid + "pendinglist", sendid + "发来新消息 ");
            }
            if (redis.gethash("UserChatStatus", recvefid) == "trueone")
            {
                redis.hsetValue("UserChatStatus", sendid, "truethree");
                redis.hsetValue("UserChatStatus", recvefid, "truethree");
                forwandr::sendMsg(recvfd, "SAME_CHAT");
                cout << " break " << endl;
                break;
            }
            if (redis.gethash("UserChatStatus", sendid) == "truetwo")
            {
                redis.hsetValue("UserChatStatus", sendid, "truethree");
                redis.hsetValue("UserChatStatus", recvefid, "truethree");
                forwandr::sendMsg(recvfd, "SAME_CHAT");
                cout << " break " << endl;
                break;
            }
        }
    }

    while (true)
    {

        auto sendfdms = forwandr::recvMsg(sendfd);

        if (sendfdms == "quit")
        {
            forwandr::sendMsg(sendfd, "quit");
            // forwandr::sendMsg(recvfd, "quit");
            sendfdms.clear();
            redis.hsetValue("UserChatStatus", sendid, "false");
            redis.hsetValue("UserChatStatus", recvefid, "false");

            cout << "false false" << endl;
            break;
        }
        forwandr::sendMsg(recvfd, sendfdms);

        redis.saddvalue(sendid + recvefid + "charhistory", sendid + sendfdms);
        redis.saddvalue(recvefid + sendid + "charhistory", sendid + sendfdms);
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void fflushmessage(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);

    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    int number = 0;
    auto pendingevent = sendid + "pendinglist";
    auto len = redis.getsetnumber(pendingevent);
    int freelen = len;
    while (true)
    {
        auto meeees = forwandr::recvMsg(fd);
        // cout << " mess " << meeees << endl;
        int leneg = meeees.length();

        // cout << " length " << leneg << endl;

        if (leneg == 0)
        {
            cout << " break" << endl;

            break;
        }
        while (true)
        {

            if (leneg > 0)
            {

                len = redis.getsetnumber(pendingevent);
                freelen = len;
                if (len == 0)
                {
                    // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 休眠 1 秒

                    forwandr::sendMsg(fd, to_string(len));
                    // cout << " number " << number << endl;

                    break;
                }

                forwandr::sendMsg(fd, to_string(len));
                auto getstr = redis.smembers(pendingevent);
                for (int i = 0; len > 0; ++i)
                {

                    if (len > 0)
                    {
                        forwandr::sendMsg(fd, getstr[i]->str);
                    }
                    redis.sremvalue(pendingevent, getstr[i]->str);
                    len = redis.getsetnumber(pendingevent);
                    printf("len = %d \n", len);
                }
            }
        }
    }

    // for (int i = 0; i < freelen; i++)
    //     free(getstr[i]);
    // free(getstr);
    cout << "        665      " << endl;

    close(fd);
    // epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

    return;
}

void blockfriends(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;

    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    auto friendspace = sendid + "friend";

    auto sermsname = forwandr::recvMsg(fd);

    auto blockname = redis.gethash("namespce", sermsname);

    user.parseJson(blockname);

    cout << friendspace << endl;

    if ((redis.sismember(friendspace, user.uid) == 0))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {

        redis.saddvalue(sendid + "blocklist", user.uid);

        forwandr::sendMsg(fd, "BLOCK_SUCCESS ");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void ThransFILE(void *args)
{

    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    string file_name;
    string file_path;
    char buf[BUFSIZ];

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    cout << "sendid" << sendid << endl;
    auto sermsname = forwandr::recvMsg(fd);
    cout << sermsname << endl;

    if (redis.hashexists("namespce", sermsname) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    auto friendname = redis.gethash("namespce", sermsname);

    user.parseJson(friendname);

    auto recvefid = user.uid;

    if ((redis.sismember(sendid + "friend", user.uid) == 0) || (redis.sismember(sendid + "blocklist", user.uid) == 1))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        forwandr::sendMsg(fd, "ok");
        auto ms = forwandr::recvMsg(fd);
        file_name = ms;

        cout << file_name << endl;
        file_path = "./file_buf/" + file_name;

        DIR *dir;
        if ((dir = opendir("./file_buf")) == NULL)
        {
            mkdir("./file_buf", 0777);
        }
        else
        {
            closedir(dir);
        }

        //创建文件
        FILE *fp = fopen(file_path.c_str(), "wb");
        if (fp == NULL)
        {
            perror(" fopen  error");
            exit(1);
        }

        string ssize;
        int count = 0;
        ssize = forwandr::recvMsg(fd);

        off_t size = stoll(ssize);
        off_t sum = size;

        while (size > 0)
        {
            cout << "size " << size << endl;
            if (sizeof(buf) < size)
            {
                count = forwandr::readn(fd, buf, size);
            }
            else
            {
                count = forwandr::readn(fd, buf, size);
            }
            if (count < 0)
            {
                cout << "count <0 " << count << endl;
                continue;
            }
            size -= count;
            fwrite(buf, count, 1, fp);
            cout << "size " << size << endl;
            cout << "count " << count << endl;
        }

        redis.hsetValue(recvefid + "FIlEspce", recvefid, file_path);

        redis.saddvalue(recvefid + "pendinglist", sendid + "向您发送文件");

        fclose(fp);
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void recviefile(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    string file_name;
    string file_path;
    char buf[BUFSIZ];

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    cout << "sendid" << sendid << endl;

    auto len = redis.hashlen(sendid + "FIlEspce");

    cout << " len =  " << len << endl;
    if (len == 0)
    {

        forwandr::sendMsg(fd, "NOFILE");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        auto pathname = redis.gethash(sendid + "FIlEspce", sendid);
        redis.hashdel(sendid + "FIlEspce", sendid);

        cout << "path name " << pathname << endl;

        struct stat info;
        if (stat(pathname.c_str(), &info) == -1)
        {
            printf("---非法的路径名---\n");
            return;
        }

        forwandr::sendMsg(fd, pathname);

        auto ms = forwandr::recvMsg(fd);
        if (ms == "yes")
        {
            int fp = open(pathname.c_str(), O_RDWR, S_IRUSR | S_IWUSR | O_LARGEFILE);

            forwandr::sendMsg(fd, to_string(info.st_size));

            off_t ret = 0;
            off_t sum = info.st_size;

            while (true)
            {
                ret = sendfile(fd, fp, NULL, info.st_size);
                if (ret == 0)
                {
                    break;
                }
                else
                {
                    if (ret != -1)
                    {
                        cout << ret << endl;
                    }
                }
            }
        }
        else
        {

            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
}

void recvcharhistory(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    string file_name;
    string file_path;
    char buf[BUFSIZ];

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    cout << "sendid" << sendid << endl;
    auto sermsname = forwandr::recvMsg(fd);
    cout << sermsname << endl;

    if (redis.hashexists("namespce", sermsname) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    auto friendname = redis.gethash("namespce", sermsname);

    user.parseJson(friendname);

    auto recvefid = user.uid;

    auto len = redis.getsetnumber(sendid + recvefid + "charhistory");
    int freelen = len;

    if ((redis.sismember(sendid + "friend", user.uid) == 0) || (redis.sismember(sendid + "blocklist", user.uid) == 1))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {

        cout << " len " << len << endl;
        if (len == 0)
        {
            forwandr::sendMsg(fd, to_string(len));
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }

        forwandr::sendMsg(fd, to_string(len));

        auto getstr = redis.smembers(sendid + recvefid + "charhistory");

        for (int i = 0; len > 0; ++i, --len)
        {

            forwandr::sendMsg(fd, getstr[i]->str);

            printf("len = %d \n", len);
        }

        cout << "  967967967  " << endl;
        // for (int i = 0; i < freelen; i++)
        //     free(getstr[i]);
        // free(getstr);

        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
}

void CREATEGROUP(void *args)
{

    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    cout << " sendid " << sendid << endl;
    string grop_id = to_string(rand() % 1000000 + 1000000);

    ms = forwandr::recvMsg(fd);
    cout << "111111" << endl;
    if (ms == "yes")
    {
        forwandr::sendMsg(fd, grop_id);
        redis.saddvalue("groupidspace", grop_id);
        redis.saddvalue(sendid + "groupspace", grop_id);
        redis.saddvalue(grop_id + "spacefoundid", sendid);
        redis.hsetValue(grop_id + "space", sendid, "master");
    }
    else
    {
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void Look_Group(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();

    auto groupspace = sendid + "groupspace";

    int number = redis.getsetnumber(groupspace);
    int freenumber = number;
    if (number == 0)
    {
        forwandr::sendMsg(fd, "NO_GROUP");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        forwandr::sendMsg(fd, "number");

        forwandr::sendMsg(fd, to_string(number));
        auto getstr = redis.smembers(sendid + "groupspace");

        for (int i = 0; number > 0; ++i, --number)
        {

            forwandr::sendMsg(fd, getstr[i]->str);

            printf("number = %d \n", number);
        }

        // for (int i = 0; i < freenumber; i++)
        //     free(getstr[i]);
        // free(getstr);

        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
}

void addgroup(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    // Json::CharReaderBuilder readerBuilder;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    // epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp); //从树上去除该套接字

    auto groupidwithsend = forwandr::recvMsg(fd);
    r.parse(groupidwithsend, root);
    int i = 0;
    auto sendid = root[i++].asString();
    auto groupid = root[i++].asString();

    cout << " sendid " << sendid << endl;
    cout << " groupid " << groupid << endl;

    if (0 == redis.sismember("groupidspace", groupid))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {

        redis.saddvalue(groupid + "groupappliction", sendid);

        forwandr::sendMsg(fd, "WAIT_FOR_MINUTE");

        auto getstr = redis.smembers(groupid + "spacefoundid");
        auto len = redis.hashlen(groupid + "space");
        int freelne = len;
        cout << " len " << len << endl;
        for (int i = 0; i < len; i++)
        {
            auto StatusFlag = redis.gethash(groupid + "space", getstr[i]->str);
            cout << " getstr " << getstr[i]->str << endl;

            cout << " flag " << StatusFlag << endl;
            if (StatusFlag == "GM" || StatusFlag == "master")
            {
                string namesp = getstr[i]->str;
                cout << namesp << endl;
                redis.saddvalue(namesp + "pendinglist", sendid + "申请加入群聊" + groupid);
            }
        }
        // for (int i = 0; i < freelne; i++)
        //     free(getstr[i]);
        // free(getstr);

        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
}

void setGm(void *atgs)
{
    forJson user;
    Redis redis;
    redis.connect();
    forJson addeduser;
    Json::Value root;
    std::string Storstr;
    // Json::CharReaderBuilder readerBuilder;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)atgs;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    // epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp); //从树上去除该套接字

    auto groupidwithsend = forwandr::recvMsg(fd);
    r.parse(groupidwithsend, root);
    int i = 0;
    auto sendid = root[i++].asString();
    auto groupid = root[i++].asString();

    cout << " sendid " << sendid << endl;
    cout << " groupid " << groupid << endl;
    if (0 == redis.sismember("groupidspace", groupid) || (redis.sismember(groupid + "spacefoundid", sendid)) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        forwandr::sendMsg(fd, "ok");
        cout << "ok" << endl;
    }

    auto ms = forwandr::recvMsg(fd);
    cout << " ms " << ms << endl;

    if (ms == "SET_GM_GM")
    {
        auto infomation = redis.gethash(groupid + "space", sendid);

        if (infomation != "master")
        {
            forwandr::sendMsg(fd, "Nodefiend");
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }
        else
        {
            forwandr::sendMsg(fd, "ok");
            cout << "ok" << endl;

            auto GMidWithid = forwandr::recvMsg(fd);
            r.parse(GMidWithid, root);
            int i = 0;
            auto sendid = root[i++].asString();
            auto GMID = root[i++].asString();

            cout << " GMID " << GMID << endl;

            if (redis.sismember(sendid + "friend", user.uid) == 0 && redis.hashexists(groupid + "space", GMID) == 0)
            {
                forwandr::sendMsg(fd, "BU_cun_ZAI");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            else
            {
                redis.hsetValue(groupid + "space", GMID, "GM");
                forwandr::sendMsg(fd, "ok");
                redis.saddvalue(GMID + "pendinglist", "您被设为" + groupid + "管理");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
        }
    }
    else if (ms == "LOOK_ALLPEOPLE")
    {

        auto len = redis.getsetnumber(groupid + "spacefoundid");
        int freelen = len;
        if (len == 0)
        {

            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }

        forwandr::sendMsg(fd, to_string(len));

        auto getstr = redis.smembers(groupid + "spacefoundid");

        for (int i = 0; len > 0; ++i, --len)
        {
            auto str = redis.gethash("pinfo", getstr[i]->str);
            user.parseJson(str);
            auto infomation = redis.gethash(groupid + "space", user.uid);
            auto newname = user.name + " : ";

            forwandr::sendMsg(fd, newname + getstr[i]->str + infomation);

            printf("len = %d \n", len);
        }

        // for (int i = 0; i < freelen; i++)
        //     free(getstr[i]);
        // free(getstr);

        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else if (ms == "LOOK_appliction")
    {
        auto infomation = redis.gethash(groupid + "space", sendid);

        cout << " information " << infomation << endl;
        if (infomation == "master")
        {
            cout << infomation << endl;

            auto len = redis.getsetnumber(groupid + "groupappliction");

            int freelen2 = len;
            if (len == 0)
            {
                forwandr::sendMsg(fd, "no_user_appliction");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            else
            {
                forwandr::sendMsg(fd, to_string(len));

                auto getstr = redis.smembers(groupid + "groupappliction");
                string newname;
                for (int i = 0; len > 0; ++i)
                {

                    if (len < 0 || len == 0)
                    {
                        break;
                    }

                    newname = getstr[i]->str;
                    if (len > 0)
                    {
                        forwandr::sendMsg(fd, getstr[i]->str);
                    }
                    ms = forwandr::recvMsg(fd);
                    if (ms == "yes")
                    {

                        redis.hsetValue(groupid + "space", getstr[i]->str, "people");
                        redis.saddvalue(groupid + "spacefoundid", getstr[i]->str);
                        redis.saddvalue(newname + "groupspace", groupid);
                        redis.sremvalue(groupid + "groupappliction", getstr[i]->str);
                    }
                    else
                    {
                        redis.sremvalue(groupid + "groupappliction", getstr[i]->str);
                        len = redis.getsetnumber(groupid + "groupappliction");
                        printf("287              len = %d \n", len);
                        continue;
                    }
                    len = redis.getsetnumber(groupid + "groupappliction");
                }
                // for (int i = 0; i < freelen2; i++)
                //     free(getstr[i]);
                // free(getstr);
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

                return;
            }
        }
        else if (infomation == "GM")
        {
            cout << infomation << endl;

            auto len = redis.getsetnumber(groupid + "groupappliction");

            int freelen2 = len;
            if (len == 0)
            {
                forwandr::sendMsg(fd, "no_user_appliction");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            else
            {
                forwandr::sendMsg(fd, to_string(len));

                auto getstr = redis.smembers(groupid + "groupappliction");
                string newname;
                for (int i = 0; len > 0; ++i)
                {

                    if (len < 0 || len == 0)
                    {
                        break;
                    }

                    newname = getstr[i]->str;
                    if (len > 0)
                    {
                        forwandr::sendMsg(fd, getstr[i]->str);
                    }
                    ms = forwandr::recvMsg(fd);
                    if (ms == "yes")
                    {

                        redis.hsetValue(groupid + "space", getstr[i]->str, "people");
                        redis.saddvalue(groupid + "spacefoundid", getstr[i]->str);
                        redis.saddvalue(newname + "groupspace", groupid);
                        redis.sremvalue(groupid + "groupappliction", getstr[i]->str);
                    }
                    else
                    {
                        redis.sremvalue(groupid + "groupappliction", getstr[i]->str);
                        len = redis.getsetnumber(groupid + "groupappliction");
                        printf("287              len = %d \n", len);
                        continue;
                    }
                    len = redis.getsetnumber(groupid + "groupappliction");
                }
                // for (int i = 0; i < freelen2; i++)
                //     free(getstr[i]);
                // free(getstr);
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

                return;
            }
        }
        else
        {
            forwandr::sendMsg(fd, "Nodefiend");

            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }
    }
    else if (ms == "KIK_PERSON")
    {

        auto infomation = redis.gethash(groupid + "space", sendid);

        cout << " information " << infomation << endl;
        if (infomation == "master")
        {
            auto deluid = forwandr::recvMsg(fd);

            infomation = redis.gethash(groupid + "space", deluid);

            cout << " deluid " << deluid << endl;
            if ((redis.sismember(groupid + "spacefoundid", deluid) == 0))
            {
                forwandr::sendMsg(fd, "BU_cun_ZAI");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            else

                redis.sremvalue(groupid + "spacefoundid", deluid);
            redis.sremvalue(deluid + "groupspace", groupid);
            redis.hashdel(groupid + "space", deluid);

            forwandr::sendMsg(fd, "Del_success");
            redis.saddvalue(deluid + "pendinglist", "您被踢出群聊" + groupid);
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }
        else if (infomation == "GM")
        {

            auto deluid = forwandr::recvMsg(fd);

            infomation = redis.gethash(groupid + "space", deluid);

            if (infomation == "master")
            {
                forwandr::sendMsg(fd, "Nodefiend");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            cout << " deluid " << deluid << endl;
            if ((redis.sismember(groupid + "spacefoundid", deluid) == 0))
            {
                forwandr::sendMsg(fd, "BU_cun_ZAI");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            else

                redis.sremvalue(groupid + "spacefoundid", deluid);
            redis.sremvalue(deluid + "groupspace", groupid);
            redis.hashdel(groupid + "space", deluid);

            forwandr::sendMsg(fd, "Del_success");
            redis.saddvalue(deluid + "pendinglist", "您被踢出群聊" + groupid);
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }
        else
        {
            forwandr::sendMsg(fd, "Nodefiend");
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }

        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else if (ms == "disvlove")
    {

        auto ms = forwandr::recvMsg(fd);

        if (ms == "yes")
        {
            auto infomation = redis.gethash(groupid + "space", sendid);
            cout << " information " << infomation << endl;
            if (infomation != "master")
            {
                forwandr::sendMsg(fd, "Nodefiend");
                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
                return;
            }
            else
            {
                redis.sremvalue(sendid + "groupspace", groupid);
                redis.sremvalue("groupidspace", groupid);

                string newname;
                auto getstr = redis.smembers(groupid + "spacefoundid");
                auto len = redis.getsetnumber(groupid + "spacefoundid");
                int freelen = len;

                int findlen = len;
                for (int i = 0; findlen > 0; ++i, --findlen)
                {
                    string newname = getstr[i]->str;
                    redis.sremvalue(newname + "groupspace", groupid);

                    cout << " newname + groupspace " << newname + "groupspace  " << groupid << endl;

                    len = redis.getsetnumber(groupid + "spacefoundid");
                    cout << "len " << len << endl;
                    newname.clear();
                }

                for (int i = 0; len > 0; ++i)
                {

                    redis.sremvalue(groupid + "spacefoundid", getstr[i]->str);

                    len = redis.getsetnumber(groupid + "spacefoundid");
                }

                len = redis.hashlen(groupid + "space");
                for (int i = 0; len > 0; ++i)
                {

                    cout << " str " << getstr[i]->str << endl;
                    redis.hashdel(groupid + "space", getstr[i]->str);

                    len = redis.hashlen(groupid + "space");
                }
                forwandr::sendMsg(fd, "disloveSUeccc");

                epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

                return;
            }
        }
        else
        {
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
            return;
        }
    }
}

void chatgroup(void *Args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)Args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    auto groupid = root[i++].asString();

    int sendfd = map_id.find(sendid)->second;

    if (redis.sismember(groupid + "spacefoundid", sendid) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {
        forwandr::sendMsg(fd, "READY_TO_LEARN");
        redis.hsetValue("UserChatStatus", sendid, "true");
    }

    auto getstr = redis.smembers(groupid + "spacefoundid");
    auto len = redis.getsetnumber(groupid + "spacefoundid");

    auto StatusFlag = redis.gethash("UserChatStatus", sendid);

    while (true)
    {

        if (StatusFlag == "true")
        {
            cout << " break" << endl;
            break;
        }
        auto sendfdms = forwandr::recvMsg(sendfd);

        if (sendfdms == "quit")
        {

            forwandr::sendMsg(sendfd, "quit");

            redis.hsetValue("UserChatStatus", sendid, "false");

            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

            return;
        }
        redis.saddvalue(groupid + "charhistory", sendfdms);
    }

    while (true)
    {
        auto sendfdms = forwandr::recvMsg(sendfd);

        if (sendfdms == "quit")
        {

            forwandr::sendMsg(sendfd, "quit");
            redis.hsetValue("UserChatStatus", sendid, "false");
            break;
        }
        else
        {

            for (int count = 0; count < len; count++)
            {

                auto checkiter = map_id.find(getstr[count]->str);
                auto StatusFlag = redis.gethash("UserChatStatus", getstr[count]->str);

                if (StatusFlag == "false" || checkiter == map_id.end())
                {
                    continue;
                }

                if (checkiter->second != sendfd)
                    forwandr::sendMsg(checkiter->second, sendfdms);
            }
        }

        redis.saddvalue(groupid + "charhistory", sendid + sendfdms);
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}

void exitgroup(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;
    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    auto groupid = root[i++].asString();

    auto groupspcae = sendid + "groupspace";

    // //    redis.saddvalue("groupidspace", grop_id);
    //     redis.saddvalue(sendid + "groupspace", grop_id);
    //     redis.saddvalue(grop_id + "spacefoundid", sendid);
    //     redis.hsetValue(grop_id + "space", sendid, "master");

    cout << "sendid " << sendid << endl;
    cout << "groupid " << groupid << endl;
    cout << "groupidspcae " << groupspcae << endl;

    if (0 == redis.hashexists(groupid + "space", sendid) || 0 == redis.sismember("groupidspace", groupid) || 0 == redis.sismember(groupspcae, groupid))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    else
    {
        auto infomation = redis.gethash(groupid + "space", sendid);
        cout << " information " << infomation << endl;
        if (infomation != "master")
        {
            redis.sremvalue(groupspcae, groupid);
            redis.sremvalue(groupid + "spacefoundid", sendid);

            redis.hashdel(groupid + "space", sendid);

            forwandr::sendMsg(fd, "ok");
        }
        else
        {
            redis.sremvalue(sendid + "groupspace", groupid);
            redis.sremvalue("groupidspace", groupid);

            string newname;
            auto getstr = redis.smembers(groupid + "spacefoundid");
            auto len = redis.getsetnumber(groupid + "spacefoundid");
            int freelen = len;

            for (int i = 0; len > 0; ++i)
            {

                redis.sremvalue(groupid + "spacefoundid", getstr[i]->str);

                len = redis.getsetnumber(groupid + "spacefoundid");
                cout << " len  1262" << len << endl;
            }

            len = redis.hashlen(groupid + "space");
            for (int i = 0; len > 0; ++i)
            {
                cout << " len 1269" << len << endl;
                cout << " str " << getstr[i]->str << endl;
                redis.hashdel(groupid + "space", getstr[i]->str);

                len = redis.hashlen(groupid + "space");
            }
            forwandr::sendMsg(fd, "okkkkkkkk");
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上

            return;
        }
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
}

void UNblock(void *args)
{
    forJson user;
    Redis redis;
    redis.connect();
    Json::Value root;
    std::string Storstr;
    Json::Reader r;

    struct epoll_event temp;
    int *arr = (int *)args;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    auto ms = forwandr::recvMsg(fd);
    int i = 0;
    r.parse(ms, root);
    auto sendid = root[i++].asString();
    auto friendspace = sendid + "blocklist";

    auto sermsname = forwandr::recvMsg(fd);

    if (redis.hashexists("namespce", sermsname) == 0)
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    auto blockname = redis.gethash("namespce", sermsname);

    user.parseJson(blockname);

    cout << friendspace << " : " << user.uid << endl;

    if ((redis.sismember(friendspace, user.uid) == 0))
    {
        forwandr::sendMsg(fd, "BU_cun_ZAI");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }
    else
    {

        redis.sremvalue(sendid + "blocklist", user.uid);

        forwandr::sendMsg(fd, "UNBLOCK");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
        return;
    }

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp); //再添加回树上
    return;
}