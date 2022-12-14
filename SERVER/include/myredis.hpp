#ifndef __REDIS_HANDLER_H__
#define __REDIS_HANDLER_H__

#include <hiredis/hiredis.h>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

enum
{
    M_REDIS_OK = 0,          //执行成功
    M_CONNECT_FAIL = -1,     //连接redis失败
    M_CONTEXT_ERROR = -2,    // RedisContext返回错误
    M_REPLY_ERROR = -3,      // redisReply错误
    M_EXE_COMMAND_ERROR = -4 // redis命令执行错误
};

class Redis
{
public:
    Redis();
    ~Redis();
    int connect(const string &addr, int port, const string &pwd); //连接redis数据库：addr：IP地址，port：端口号，pwd：密码(默认为空)
    int disConnect();                                             //断开连接
    int setValue(const string &key, const string &value);         //添加或修改键值对，成功返回0，失败<0
    string getValue(const string &key);                           //获取键对应的值，成功返回0，失败<0
    int delKey(const string &key);                                //删除键，成功返回影响的行数，失败<0

    int hsetValue(const string &key, const string &field, const string &value); //插入哈希表
    int hashexists(const string &key, const string &field);                     //查看是否存在，存在返回1，不存在返回0
    string gethash(const string &key, const string &field);                     //获取对应的hash_value
    int hashdel(const string &key, const string &field);                        //从哈希表删除指定的元素

    int saddvalue(const string &key, const string &value); //插入到集合
    int sismember(const string &key, const string &value); //查看数据是否存在
    int sremvalue(const string &key, const string &value); //将数据从set中移出
    string getsetmenm(const string &key);
    int getsetnumber(const string &key);
    int Lpush(const string &key, const string &value);
    string Lpop(const string &key);
    int Len(const string &key);
    redisReply **smembers(const string &key);
    int hashlen(const string &key);

private:
    string m_addr;        // IP地址
    int m_port;           //端口号
    string m_pwd;         //密码
    redisContext *pm_rct; // redis结构体
    redisReply *pm_rr;    //返回结构体
    string error_msg;     //错误信息

    // int connectAuth(const string &pwd); //使用密码登录
    int handleReply(void *value = NULL, redisReply ***array = NULL); //处理返回的结果
};

Redis::Redis()
{
    m_addr = "";
    m_port = 0;
    m_pwd = "";
    pm_rct = NULL;
    pm_rr = NULL;
    error_msg = "";
}
Redis::~Redis()
{
    disConnect();
    pm_rct = NULL;
    pm_rr = NULL;
}
//连接数据库
//失败返回M_CONNECT_FAIL
int Redis::connect(const string &addr = "127.0.0.1", int port = 6379, const string &pwd = "")
{
    m_addr = addr;
    m_port = port;
    m_pwd = pwd;
    pm_rct = redisConnect(m_addr.c_str(), m_port);
    if (pm_rct->err)
    {
        error_msg = pm_rct->errstr;
        return M_CONNECT_FAIL;
    }
    return M_REDIS_OK;
}
//断开链接
int Redis::disConnect()
{
    freeReplyObject(pm_rr);
    redisFree(pm_rct);
    cout << "  free redis " << endl;
    return 1;
}
int Redis::setValue(const string &key, const string &value)
{
    string cmd = "set  " + key + "  " + value;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->type;
}
string Redis::getValue(const string &key) //获取键对应的值，成功返回0，失败<0
{
    string cmd = "get  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->str;
}
int Redis::delKey(const string &key)
{
    string cmd = "del  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->type;
}

int Redis::hsetValue(const string &key, const string &field, const string &value) //插入哈希表
{

    string cmd = "hset  " + key + " " + field + " " + value;
    std::cout << cmd << std::endl;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    std::cout << pm_rr->type << std::endl;
    return pm_rr->type;
}
int Redis::hashexists(const string &key, const string &field) //查看是否存在，存在返回1，不存在返回0
{
    string cmd = "hexists  " + key + "  " + field;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->integer;
}
string Redis::gethash(const string &key, const string &field) //获取对应的hash_value
{
    string cmd = "hget  " + key + "  " + field;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->str;
}
int Redis::hashdel(const string &key, const string &field) //从哈希表删除指定的元素
{
    string cmd = "hdel  " + key + "  " + field;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->type;
}

int Redis::hashlen(const string &key) //从哈希表获取长度
{
    string cmd = "hlen  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->integer;
}

int Redis::saddvalue(const string &key, const string &value) //插入到集合
{
    string cmd = "sadd  " + key + "  " + value;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->type;
}
int Redis::sismember(const string &key, const string &value) //查看数据是否存在
{
    string cmd = "sismember  " + key + "  " + value;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->integer;
}
string Redis::getsetmenm(const string &key)
{
    string cmd = "SMEMBERS  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->str;
}

int Redis::getsetnumber(const string &key)
{
    string cmd = "SCARD  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->integer;
}

int Redis::sremvalue(const string &key, const string &value) //将数据从set中移出
{
    string cmd = "srem  " + key + "  " + value;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->type;
}

int Redis::Lpush(const string &key, const string &value) //把数据插入a部列表
{
    string cmd = "LPUSH  " + key + "  " + value;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->integer;
}
string Redis::Lpop(const string &key) //把数据从头部弹出
{
    string cmd = " Lpop  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->str;
}

int Redis::Len(const string &key)
{
    string cmd = "LLEN  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->integer;
}

redisReply **Redis::smembers(const string &key)
{
    string cmd = "smembers  " + key;
    pm_rr = (redisReply *)redisCommand(pm_rct, cmd.c_str());
    return pm_rr->element;
}

#endif