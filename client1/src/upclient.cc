#include "../include/TcpSocket.hh"
#include <fstream>
#include <iostream>
#include <sys/signal.h>
#include <iostream>
#include "json/json.h"
#include <fstream>
#include "../include/user.hpp"
#include <cstdlib>
#include "../include/TcpSocket.hh"
#include "../include/threadpool.hpp"
#include "../include/Forwandr.hpp"
#include <time.h>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
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
void promgsignal(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);
}
void menu()
{
    std::cout << "---------------------" << std::endl;
    std::cout << "-                   -" << std::endl;
    std::cout << "-      1.登陆        -" << std::endl;
    std::cout << "-      2.注册        -" << std::endl;
    std::cout << "-                   -" << std::endl;
    std::cout << "---------------------" << std::endl;
}
void loginaftermenu()
{
    std::cout << "----------------------\033[32m欢迎\033[0m-------------------------" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "-                                                           - " << std::endl;
    std::cout << "- \033[32m1.私聊\033[0m         \033[32m2.进入群聊\033[0m                                 - " << std::endl;
    std::cout << "- \033[32m3.加好友\033[0m         \033[32m4.查看好友申请\033[0m                           - " << std::endl;
    std::cout << "- \033[32m5.展示好友列表\033[0m     \033[32m6.查看历史消息\033[0m                          -" << std::endl;
    std::cout << "- \033[32m7.删除好友\033[0m        \033[32m8.屏蔽好友\033[0m                              - " << std::endl;
    std::cout << "- \033[32m9.解除屏蔽\033[0m         \033[32m10.给好友发送文件\033[0m                         - " << std::endl;
    std::cout << "- \033[32m11.创建群聊\033[0m      \033[32m12.管理群聊\033[0m                                 - " << std::endl;
    std::cout << "- \033[32m13.退出群聊\033[0m      \033[32m14.查看群聊\033[0m                                        -" << std::endl;
    std::cout << "- \033[32m15.加入群聊\033[0m         \033[32m16.接收好友文件\033[0m                                             -" << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
}
int num = 100;
void *charwitfriend(void *args)
{

    int *d = (int *)args;
    int fd = *d;

    while (true)
    {

        auto ms = forwandr::recvMsg(fd);

        if (ms == "NONALIVE")
        {

            cout << "对方不在聊天框中进入留言模式" << endl;
            ms.clear();
        }
        if (ms == "SAME_CHAT")
        {
            cout << "对方进入聊天" << endl;
            ms.clear();
            continue;
        }

        if (ms == "quit")
        {
            cout << "对方退出聊天" << endl;
            ms.clear();
            pthread_exit(&num);
        }
        printf("\n%s\n", ms.c_str());
        ms.clear();
    }

    return NULL;
}

void *fflushmessage(void *args)
{
    string uid = (char *)args;
    TcpSocket tcp2;
    tcp2.connectToHost("127.0.0.1", 9527);
    forJson user;
    auto sendid = user.sendid(uid);
    int len;
    tcp2.sendMsg(to_string(FLLLLLLLLLLL));
    tcp2.sendMsg(sendid);
    while (true)
    {

        while (true)
        {

            tcp2.sendMsg("FLUSH_message");

            len = atoi(tcp2.recvMsg().c_str());
            if (len == 0)
            {
                // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 休眠 1 秒
                // tcp2.sendMsg("FLUSH_message");
                continue;
            }
            else
            {
                break;
            }
        }

        while (len)
        {
            auto ms = tcp2.recvMsg();

            printf("\n%s\n", ms.c_str());

            ms.clear();
            len--;
        }
    }
    return NULL;
}

bool IsAllNumString(const std::string str)
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

int main()
{
    printf("pid : %d\n", getpid());
    TcpSocket tcp;
    promgsignal();
    tcp.connectToHost("127.0.0.1", 9527);
    std::string choice;
    std::string forlog;
    std::string fromjson;

    std::string uid;
    std::string name;
    std::string passwd;
    std::string email;
    std::string frienduid;
    std::string groupid;
    int truechoice;
    int fd = tcp.m_fd;

    forJson user;

    string newmsg;

    while (true)
    {
        menu();

        std::cout << "请输入选择" << std::endl;
        std::cin >> choice;
        if (cin.eof())
        {
            cout << "遇到文件结尾" << endl;

            return 0;
        }
        cin.clear();
        cin.ignore(INT32_MAX, '\n');

        truechoice = atoi(choice.c_str());

        if (truechoice != 1 && truechoice != 2)
        {
            continue;
        }
        else if (truechoice == 1) //登陆
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(WO_YAO_DENG_LU));
            while (true)
            {
                std::cout << "请输入账号 （ 必须全为数字且大于5位）" << std::endl;
                std::cin >> uid;
                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.ignore(INT32_MAX, '\n');

                if (!IsAllNumString(uid) || uid.length() < 5)
                {
                    cout << "账号必须为数字或者长度小于5位" << endl;
                    uid.clear();
                    continue;
                }

                std::cout << "请输入昵称" << std::endl;
                std::cin >> name;
                cin.clear();
                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.ignore(INT32_MAX, '\n');

                std::cout << "请输入密码" << std::endl;
                std::cin >> passwd;
                cin.clear();
                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.ignore(INT32_MAX, '\n');

                if (!IsAllNumString(passwd) || uid.length() < 5)
                {
                    cout << "密码不能全为数字且长度必须大于5位" << endl;
                    passwd.clear();
                    uid.clear();
                    cin.clear();
                    continue;
                }

                break;
            }

            auto forlog = user.forlogin(uid, name, passwd);

            tcp.sendMsg(forlog);

            auto ms = tcp.recvMsg();

            if (ms == "BU_cun_ZAI")
            {
                ms.clear();
                std::cout << "账号不存在请注册" << std::endl;
                std::cout << "-------------------------" << std::endl;
                forlog.clear();
                continue;
            }
            else if (ms == "WO_YAO_DENG_LU")
            {
                std::cout << "登陆成功" << std::endl;
                std::cout << "-------------------------" << std::endl;
                break;
            }
            else if (ms == "Mi_MA_CUO_Wu")
            {
                ms.clear();
                std::cout << "密码错误" << std::endl;
                std::cout << "-------------------------" << std::endl;
                forlog.clear();

                continue;
            }
            else if (ms == "Repeatlog")
            {
                ms.clear();
                std::cout << "账号已登陆或者昵称输入错误" << std::endl;
                std::cout << "-------------------------" << std::endl;
                forlog.clear();
                continue;
            }
        }
        else if (truechoice == 2) //注册
        {

            tcp.sendMsg(std::to_string(WO_YAO_ZHU_CE));

            while (true)
            {
                std::cout << "请输入账号 （ 必须全为数字且大于5位）" << std::endl;

                std::cin >> uid;

                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.clear();
                cin.ignore(INT32_MAX, '\n');

                if (!IsAllNumString(uid) || uid.length() < 5)
                {
                    cout << "账号必须为数字或者长度大于5位" << endl;
                    uid.clear();
                    continue;
                }

                std::cout << "请输入昵称" << std::endl;
                std::cin >> name;
                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.clear();
                cin.ignore(INT32_MAX, '\n');

                std::cout << "请输入密码" << std::endl;
                std::cin >> passwd;

                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.clear();
                cin.ignore(INT32_MAX, '\n');

                if (!IsAllNumString(passwd) || uid.length() < 5)
                {
                    cout << "密码不能全为数字且长度必须大于5位" << endl;
                    passwd.clear();
                    uid.clear();
                    continue;
                }
                std::cout << "请输入邮箱" << std::endl;
                std::cin >> email;
                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.clear();
                cin.ignore(INT32_MAX, '\n');

                break;
            }

            auto str = user.createJson(uid, name, passwd, email);
            tcp.sendMsg(str);

            auto ms = tcp.recvMsg();
            if (ms == "WO_YAO_ZHU_CE")
                std::cout << "注册成功" << std::endl;
            else if (ms == "SAME_UID")
            {
                std::cout << "账号已经存在，请重新注册或者登录" << std::endl;
                str.clear();
                ms.clear();
                continue;
            }
        }
    }
    choice.clear();

    pthread_t tid, tid2;
    pthread_create(&tid, NULL, fflushmessage, (void *)uid.c_str());
    while (true)
    {

        loginaftermenu();
        std::cout << "请输入选择" << std::endl;

        std::cin >> choice;
        if (cin.eof())
        {
            cout << "遇到文件结尾" << endl;
            return 0;
        }
        cin.ignore(INT32_MAX, '\n');
        truechoice = atoi(choice.c_str());

        if (truechoice == 1)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(APPLICT_CHAT));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string friendname;
            std::cout << "-------------------------" << std::endl;
            cout << "请输入要聊天的好友名称" << endl;
            cin >> friendname;

            tcp.sendMsg(friendname);

            auto ms = tcp.recvMsg();

            if (ms == "BU_cun_ZAI")
            {
                cout << "不能与非好友进行聊天" << endl;
                ms.clear();
                continue;
            }
            else if (ms == "block")
            {
                cout << "已被屏蔽无法聊天" << endl;
                ms.clear();
                continue;
            }
            else if (ms == "READY_TO_LEARN")
            {
                pthread_t tid3;
                pthread_create(&tid3, NULL, charwitfriend, (void *)&fd);

                std::cout << "--------------开始聊天-----------" << std::endl;
                string mems, message;
                string newname = name;
                void *ptr;
                while (true)
                {

                    cin >> mems;
                    if (mems == "quit")
                    {
                        tcp.sendMsg("quit");
                        break;
                    }
                    message = newname + ":" + mems;
                    tcp.sendMsg(message);
                    cin.clear();
                    mems.clear();
                    message.clear();
                }

                pthread_join(tid3, &ptr);

                cin.clear();
                mems.clear();
                newname.clear();
                ms.clear();
                std::cout << "-------------------------" << std::endl;
            }
        }
        else if (truechoice == 2)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(CHAT_GROUP));
            cout << "请输入要聊天的群组" << endl;
            cin >> groupid;

            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            auto addmsg = user.foraddgroup(uid, groupid);
            tcp.sendMsg(addmsg);

            auto ms = tcp.recvMsg();

            if (ms == "BU_cun_ZAI")
            {
                cout << "不能与未加入群组进行聊天" << endl;
                ms.clear();
                continue;
            }
            else if (ms == "block")
            {
                cout << "您已经被拉入黑名单" << endl;
                ms.clear();
                continue;
            }

            else if (ms == "READY_TO_LEARN")
            {
                pthread_t tid3;
                pthread_create(&tid3, NULL, charwitfriend, (void *)&fd);

                std::cout << "--------------开始聊天-----------" << std::endl;
                string mems, message;
                string newname = name;
                void *ptr;
                while (true)
                {

                    cin >> mems;
                    if (mems == "quit")
                    {
                        tcp.sendMsg("quit");
                        break;
                    }
                    message = newname + ":" + mems;
                    tcp.sendMsg(message);
                    cin.clear();
                    mems.clear();
                    message.clear();
                }
                pthread_join(tid3, &ptr);

                cin.clear();
                mems.clear();
                newname.clear();
                std::cout << "-------------------------" << std::endl;
            }
        }
        else if (truechoice == 3)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(Add_Frind));
            std::cout << "要加好友的uid" << std::endl;
            std::cin >> frienduid;

            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            auto addmsg = user.foraddfriend(uid, frienduid);
            tcp.sendMsg(addmsg);
            auto ms = tcp.recvMsg();
            if (ms == "BU_cun_ZAI")
            {
                std::cout << "好友uid不存在" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "WAIT_FOR_MINUTE")
            {
                std::cout << "申请已发送请等待对方同意" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 4)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(CHECK_NEW_APPLICTION));

            auto sendid = user.sendid(uid);
            tcp.sendMsg(sendid);

            auto ms = tcp.recvMsg();
            if (ms == "HAVE_APPLICTION")
            {

                int len = atoi(tcp.recvMsg().c_str());

                printf("%d\n", len);
                while (len)
                {

                    ms = tcp.recvMsg();

                    cout << ms << "发来好友申请 yes/no" << endl;
                    string key;
                    cin >> key;
                    if (key == "yes")
                    {
                        tcp.sendMsg(key);
                        std::cout << "-------------------------" << std::endl;
                    }
                    else
                    {
                        tcp.sendMsg(key);
                        std::cout << "-------------------------" << std::endl;
                    }

                    len--;
                }
                ms.clear();
            }
            else
            {

                cout << "没有好友申请" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }

        else if (truechoice == 5)
        {
            tcp.sendMsg(std::to_string(SHOW_FRIENDS));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            cout << "-----好友列表-----------" << endl;
            auto ms = tcp.recvMsg();

            if (ms == "NO_FRIEND")
            {
                cout << "无好友" << endl;
                ms.clear();
                continue;
            }

            len = atoi(ms.c_str());
            for (int i = 0; i < len; ++i)
            {
                ms = tcp.recvMsg();
                if (ms.find("#") == -1)
                {
                    cout << "|" << ms << "\033[32m(在线)\033[0m" << endl;
                    ms.clear();
                }
                else
                {

                    cout << "|" << ms << endl;
                    ms.clear();
                }
            }
            ms.clear();
            cout << "-----------------" << endl;
        }
        else if (truechoice == 6)
        {
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(CHECKHISTORYCHAT));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string friendname;

            cout << "请输入要查看那位好友的聊天记录" << endl;
            cin >> friendname;

            tcp.sendMsg(friendname);

            auto ms = tcp.recvMsg();
            cout << "ms670 " << ms << endl;
            if (ms == "BU_cun_ZAI")
            {
                cout << "好友不存在或者输入错误" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else
            {

                int len = atoi(ms.c_str());
                if (len == 0)
                {
                    printf("暂时没有聊天记录");
                    ms.clear();
                    continue;
                }

                for (int i = 0; i < len; ++i)
                {
                    ms = tcp.recvMsg();

                    printf("%s\n", ms.c_str());
                }
                cout << "-----------------" << endl;
                ms.clear();
            }
        }
        else if (truechoice == 7)
        {
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(DEL_FRIENDS));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string delname;
            std::cout << "-------------------------" << std::endl;
            cout << "请输入要删除的好友名称" << endl;
            cin >> delname;
            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');
            tcp.sendMsg(delname);
            auto ms = tcp.recvMsg();

            if (ms == "BU_cun_ZAI")
            {
                cout << "不能删除非好友或者输入错误" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "Del_success")
            {
                cout << "删除成功" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 8)
        {
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(BLOCK_FRIENDS));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string blockname;
            std::cout << "-------------------------" << std::endl;
            cout << "请输入要屏蔽的好友名称" << endl;
            cin >> blockname;
            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            tcp.sendMsg(blockname);
            auto ms = tcp.recvMsg();

            cout << " ms748 " << ms << endl;
            if (ms == "BU_cun_ZAI")
            {
                cout << "不能屏蔽非好友或者输入错误" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "BLOCK_SUCCESS ")
            {
                cout << "屏蔽成功" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 9)
        {
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(UNblock_frine));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string unblockname;
            std::cout << "-------------------------" << std::endl;
            cout << "请输入要解除屏蔽的好友名称" << endl;
            cin >> unblockname;
            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            tcp.sendMsg(unblockname);
            auto ms = tcp.recvMsg();

            cout << " ms803 " << ms << endl;
            if (ms == "BU_cun_ZAI")
            {
                cout << "不能解除非屏蔽好友或者输入错误" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "UNBLOCK")
            {
                cout << "解除屏蔽成功" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 10)
        {
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(TRASFILES));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string friendname;

            cout << "请输入要发送的好友名称" << endl;
            cin >> friendname;

            tcp.sendMsg(friendname);

            auto ms = tcp.recvMsg();

            cout << " ms " << ms << endl;
            if (ms == "BU_cun_ZAI")
            {
                cout << "好友不存在或者输入错误" << endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "ok")
            {
                char *file_name;
                char buf[BUFSIZ];
                char file_path[BUFSIZ];
            flag:
                printf("请输入要发送的文件路径: ");
                cin >> file_path;
                if (cin.eof())
                {
                    cout << "读到文件结尾" << endl;
                    return 0;
                }
                cin.ignore(INT32_MAX, '\n');
                //判断路径名是否正确
                struct stat info;
                if (stat(file_path, &info) == -1)
                {
                    printf("---非法的路径名---\n");
                    goto flag;
                }

                cout << info.st_size << endl;
                //获取文件名
                file_name = basename(file_path);

                //把文件名发送给接收端
                strcpy(buf, file_name);
                string p = buf;

                tcp.sendMsg(p);

                //打开文件
                int fp = open(file_path, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR | O_LARGEFILE);
                off_t ret = 0;
                tcp.sendMsg(to_string(info.st_size));

                cout << " info.st_size " << info.st_size << endl;

                cout << "发送文件等待中......" << endl;
                while (1)
                {
                    ret = sendfile(fd, fp, NULL, info.st_size);
                    cout << " ret " << endl;
                    if (ret == 0)
                    {
                        cout << "发送完毕等待对方接受" << endl;

                        break;
                    }
                    else
                    {
                        cout << ret << endl;
                    }
                }
                close(fp);

                cout << " 896 close" << endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 11)
        {
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(CREATE_GROUP));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            string blockname;
            std::cout << "-------------------------" << std::endl;
            cout << "是否要创建群聊 yes or no" << endl;
            string key;
            cin >> key;
            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');
            if (key == "yes")
            {
                tcp.sendMsg(key);
            }
            else if (key == "no")
            {
                tcp.sendMsg(key);

                continue;
            }
            else
            {
                cout << "输入错误" << endl;
                continue;
            }
            auto msg = tcp.recvMsg();
            cout << "所创建的群号为" << endl;
            cout << msg << endl;
            msg.clear();
            std::cout << "-------------------------" << std::endl;
        }
        else if (truechoice == 12)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(SET_GM));
            cout << "请输入您要管理的群号" << endl;
            std::cin >> groupid;

            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            auto addmsg = user.foraddgroup(uid, groupid);
            tcp.sendMsg(addmsg);

            auto ms = tcp.recvMsg();

            if (ms == "BU_cun_ZAI")
            {
                cout << "输入错误或您没有管理该群的权限" << endl;
                groupid.clear();
                ms.clear();
                continue;
            }
            else if (ms == "ok")
            {
                // 设置管理员，群主

                //群猪和管理元的共同权限
                cout << "1,添加管理员" << endl;
                cout << "2,查看群成员" << endl;
                cout << "3,踢人" << endl;
                cout << "4,查看群聊申请" << endl;
                cout << "5,解散群聊" << endl;
                cout << "请输入选择" << endl;
                string nowchoice;
                cin >> nowchoice;

                if (cin.eof())
                {
                    cout << "遇到文件结尾" << endl;
                    return 0;
                }
                cin.ignore(INT32_MAX, '\n');

                int nowtruechoice = atoi(nowchoice.c_str());

                if (nowtruechoice == 1)
                {
                    tcp.sendMsg("SET_GM_GM");

                    auto ms = tcp.recvMsg();
                    if (ms == "Nodefiend")
                    {
                        cout << "您不是群主，无权操作" << endl;
                        continue;
                    }
                    else
                    {
                        string GMid;
                        cout << "请输入您要添加的管理员id" << endl;
                        std::cin >> GMid;

                        if (cin.eof())
                        {
                            cout << "遇到文件结尾" << endl;
                            return 0;
                        }
                        cin.ignore(INT32_MAX, '\n');

                        auto addmsg = user.foraddfriend(uid, GMid);
                        tcp.sendMsg(addmsg);

                        auto ms = tcp.recvMsg();

                        cout << " ms " << endl;
                        if (ms == "BU_cun_ZAI")
                        {
                            cout << "id输入错误 或者没有加入群组" << endl;
                            ms.clear();
                            continue;
                        }
                        else if (ms == "ok")
                        {
                            cout << "添加成功" << endl;
                            ms.clear();
                            continue;
                        }
                    }
                }
                else if (nowtruechoice == 2)
                {
                    tcp.sendMsg("LOOK_ALLPEOPLE");
                    cout << "-----------------" << endl;

                    auto ms = tcp.recvMsg();
                    cout << " ms973  " << ms << endl;
                    int len = atoi(ms.c_str());

                    if (len == 0)
                    {
                        printf("暂时没有成员添加");
                        ms.clear();
                        continue;
                    }

                    for (int i = 0; i < len; ++i)
                    {
                        ms = tcp.recvMsg();

                        printf("%s\n", ms.c_str());
                    }
                    cout << "-----------------" << endl;
                }
                else if (nowtruechoice == 3)
                {
                    cout << "-----------------" << endl;
                    tcp.sendMsg("KIK_PERSON");

                    string deluid;
                    std::cout << "-------------------------" << std::endl;
                    cout << "请输入要踢出去的成员uid" << endl;
                    cin >> deluid;
                    if (cin.eof())
                    {
                        cout << "遇到文件结尾" << endl;
                        return 0;
                    }
                    cin.ignore(INT32_MAX, '\n');
                    tcp.sendMsg(deluid);
                    auto ms = tcp.recvMsg();
                    cout << " ms1058 " << ms << endl;
                    if (ms == "BU_cun_ZAI")
                    {
                        cout << "不能删除非该群组成员或者输入错误" << endl;
                        std::cout << "-------------------------" << std::endl;
                        ms.clear();
                        continue;
                    }
                    else if (ms == "Del_success")
                    {
                        cout << "删除成功" << endl;
                        std::cout << "-------------------------" << std::endl;
                        ms.clear();
                        continue;
                    }
                    else if (ms == "Nodefiend")
                    {
                        cout << "您不是群主，无权操作" << endl;
                        continue;
                    }
                }
                else if (nowtruechoice == 4)
                {

                    cout << "-----------------" << endl;
                    tcp.sendMsg("LOOK_appliction");
                    auto ms = tcp.recvMsg();
                    if (ms == "Nodefiend")
                    {
                        cout << "您不是群主或者管理员，无权操作" << endl;
                        ms.clear();
                        continue;
                    }
                    else if (ms == "no_user_appliction")
                    {
                        cout << "无申请" << endl;
                        ms.clear();
                        continue;
                    }
                    else
                    {
                        int len = atoi(ms.c_str());

                        printf("%d\n", len);
                        while (len)
                        {

                            ms = tcp.recvMsg();

                            cout << ms << "申请加入群组 yes/no" << endl;
                            string key;
                            cin >> key;
                            if (key == "yes")
                            {
                                tcp.sendMsg(key);
                                std::cout << "-------------------------" << std::endl;
                            }
                            else
                            {
                                tcp.sendMsg(key);
                                std::cout << "-------------------------" << std::endl;
                            }

                            len--;
                        }
                    }
                }
                else if (nowtruechoice == 5)
                {
                    tcp.sendMsg("disvlove");

                    cout << ms << "是否解散群聊 yes/no" << endl;
                    string key;
                    cin >> key;

                    if (key == "yes")
                    {
                        tcp.sendMsg(key);
                        std::cout << "-------------------------" << std::endl;
                    }
                    else
                    {
                        tcp.sendMsg(key);
                        std::cout << "-------------------------" << std::endl;
                    }
                    auto ms = tcp.recvMsg();
                    cout << " ms1144 " << ms << endl;
                    if (ms == "Nodefiend")
                    {
                        cout << "您不是群主，无权操作" << endl;
                        ms.clear();
                        continue;
                    }
                    else if (ms == "disloveSUeccc")
                    {
                        cout << "成功解散" << endl;
                        ms.clear();
                        continue;
                    }
                }

                else if (nowtruechoice != 1 || nowtruechoice != 2 || nowtruechoice != 3 || nowtruechoice != 4 || nowtruechoice != 5)
                {
                    nowchoice.clear();
                    continue;
                }
            }
        }
        else if (truechoice == 13)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(EXIT_GROUP));
            std::cout << "要退出的群组的uid" << std::endl;
            std::cin >> groupid;

            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            auto addmsg = user.foraddgroup(uid, groupid);
            tcp.sendMsg(addmsg);

            auto ms = tcp.recvMsg();
            if (ms == "BU_cun_ZAI")
            {
                std::cout << "未加入群组或者输入错误" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "ok")
            {
                std::cout << "退出成功" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "okkkkkkkk")
            {
                std::cout << "您是群主已成功解散群聊" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 14)
        {
            tcp.sendMsg(std::to_string(LOOK_GROUP));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);
            cout << "-----群组列表-----------" << endl;
            auto ms = tcp.recvMsg();
            cout << " ms 1096 " << ms << endl;
            if (ms == "NO_GROUP")
            {
                cout << "暂时没有群组" << endl;
                ms.clear();
                continue;
            }
            else if (ms == "number")
            {

                len = atoi(tcp.recvMsg().c_str());

                for (int i = 0; i < len; ++i)
                {
                    ms = tcp.recvMsg();

                    printf("%s\n", ms.c_str());
                }
                ms.clear();
                cout << "-----------------" << endl;
            }
            else
            {
                ms.clear();
                continue;
            }
        }

        else if (truechoice == 15)
        {
            std::cout << "-------------------------" << std::endl;
            tcp.sendMsg(std::to_string(ADD_GROUP));
            std::cout << "要加群组的uid" << std::endl;
            std::cin >> groupid;

            if (cin.eof())
            {
                cout << "遇到文件结尾" << endl;
                return 0;
            }
            cin.ignore(INT32_MAX, '\n');

            auto addmsg = user.foraddgroup(uid, groupid);
            tcp.sendMsg(addmsg);
            auto ms = tcp.recvMsg();

            if (ms == "BU_cun_ZAI")
            {
                std::cout << "群组uid不存在" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
            else if (ms == "WAIT_FOR_MINUTE")
            {
                std::cout << "申请已发送请等待管理员或者群主同意" << std::endl;
                std::cout << "-------------------------" << std::endl;
                ms.clear();
                continue;
            }
        }
        else if (truechoice == 16)
        {
            string file_name;
            string file_path;
            char buf[BUFSIZ];

            int n;
            cout << "-----------------" << endl;
            tcp.sendMsg(std::to_string(Recvive_File));
            auto sendid = user.sendid(uid);
            int len = 0;
            tcp.sendMsg(sendid);

            auto ms = tcp.recvMsg();

            if (ms == "NOFILE")
            {
                ms.clear();
                cout << "没有可以接受的文件" << endl;
                continue;
            }

            cout << "是否接受 yes/no" << endl;
            string key;

            cin >> key;
            if (key == "yes")
            {
                tcp.sendMsg(key);

                file_name = ms;
                file_path = file_name;

                DIR *dir;
                if ((dir = opendir("./file_buf")) == NULL)
                {
                    mkdir("./file_buf", 0777);
                }
                else
                {
                    closedir(dir);
                }

                cout << "  file ptah  " << file_path << endl;

                FILE *fp = fopen(file_path.c_str(), "wb");
                if (fp == NULL)
                {
                    perror("open error 806");
                    exit(1);
                }

                std::cout << "接收中........." << std::endl;
                string ssize;
                ssize = tcp.recvMsg();

                cout << "ssize " << ssize << endl;
                off_t size = stoll(ssize);
                off_t sum = size;

                while (size > 0)
                {
                    if (sizeof(buf) < size)
                    {
                        n = forwandr::readn(fd, buf, sizeof(buf));
                    }
                    else
                    {
                        n = forwandr::readn(fd, buf, size);
                    }
                    if (n < 0)
                    {
                        continue;
                    }

                    cout << " size " << size << endl;
                    size -= n;
                    fwrite(buf, n, 1, fp);
                }

                cout << "文件接收完毕" << endl;
                fclose(fp);
            }
            else
            {
                tcp.sendMsg(key);
                cout << "已成功拒绝接收" << endl;
                std::cout << "-------------------------" << std::endl;
                continue;
            }
        }
    }
}